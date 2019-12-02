#include "StreamDemux.h"
#ifdef SDL
int StreamDemux::thread_exit = 0;
int StreamDemux::thread_pause = 0;
#endif

HIAI_REGISTER_DATA_TYPE("VideoImageInfoT", VideoImageInfoT);
HIAI_REGISTER_DATA_TYPE("VideoImageParaT", VideoImageParaT);
HIAI_REGISTER_DATA_TYPE("OutputT", OutputT);
HIAI_REGISTER_DATA_TYPE("DetectionEngineTransT", DetectionEngineTransT);

StreamDemux::StreamDemux()
{
	stream_msg = new StreamMessage;
}

StreamDemux::~StreamDemux()
{
#ifdef SDL
	SDL_Quit();
#endif
	delete stream_msg;
	av_freep(&video_dst_data[0]);
	av_frame_free(&frame);
	av_bsf_free(&bsf_ctx);
	avcodec_free_context(&codec);
	avformat_close_input(&input_fmt);
}

#ifdef ATLAS300
HIAI_StatusT StreamDemux::Init(const hiai::AIConfig &config, const vector<hiai::AIModelDescription> &model_desc) {
    HIAI_ENGINE_LOG("Start process!");
    for (int index = 0; index < config.items_size(); ++index) {
        const ::hiai::AIConfigItem &item = config.items(index);
        if (item.name() == "stream_url") {
            stream_url = item.value();
            continue;
        }
        if (item.name() == "channel_id") {
            channel_id = std::atoi(item.value().c_str());
            continue;
        }
    }
    std::cout << "stream url:" << stream_url << std::endl;
    HIAI_ENGINE_LOG("End process!");
    return HIAI_OK;
}

HIAI_IMPL_ENGINE_PROCESS("StreamDemux", StreamDemux, INPUT_SIZE) {
	av_log_set_level(AV_LOG_INFO);
    av_register_all();
    avformat_network_init();
    pthread_t tid;
    if (StreamInit() < 0) {
        return -1;
    }
    pthread_create(&tid, NULL, DemuxStream2AtlasPacket, (void*)this);
    return HIAI_OK;
}

void StreamDemux::SendImageData(shared_ptr<VideoImageParaT> &video_image_data) {
    HIAI_StatusT hiai_ret = HIAI_OK;

    if (video_image_data == nullptr) { // the queue is empty and return
        printf("video_image_data is null.\n");
        return;
    }

    do {
        hiai_ret = SendData(0, "VideoImageParaT", static_pointer_cast<void>(video_image_data));
        if (hiai_ret == HIAI_QUEUE_FULL) { // check queue is full
            HIAI_ENGINE_LOG("The queue is full when send image data, sleep 10ms");
            usleep(10000); // sleep 10 ms
        }
    } while (hiai_ret == HIAI_QUEUE_FULL); // loop while queue is full

    if (hiai_ret != HIAI_OK) { // check send data is failed
        HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT, "Send data failed! error code: %d", hiai_ret);
    }
}

void StreamDemux::SendImageData(shared_ptr<StreamRawData>& video_image_data) {
	int hiai_ret = 0;

	if (video_image_data == nullptr) { // the queue is empty and return
		return;
	}

	do {
		hiai_ret = SendData(0, "StreamRawData", static_pointer_cast<void>(video_image_data));
		if (hiai_ret == HIAI_QUEUE_FULL) { // check queue is full
			HIAI_ENGINE_LOG("The queue is full when send image data, sleep 10ms");
            usleep(10000); // sleep 10 ms
		}
	} while (hiai_ret == HIAI_QUEUE_FULL); // loop while queue is full

	if (hiai_ret != 0) { // check send data is failed
		HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT, "Send data failed! error code: %d", hiai_ret);
	}
}

HIAI_StatusT StreamDemux::PrepareVideoImageParaT(AVPacket& av_packet, std::shared_ptr<VideoImageParaT>& video_image_para){
    video_image_para = make_shared<VideoImageParaT>();
    video_image_para->video_image_info.channel_id = channel_id;
    video_image_para->video_image_info.channel_name = stream_url;
    video_image_para->video_image_info.is_finished = false;
    video_image_para->video_image_info.video_type = kH264;

    if (av_packet.size <= 0 || av_packet.size > kAllowedMaxImageMemory) {
      return HIAI_ERROR;
    }

    uint8_t* vdec_in_buffer = new (nothrow) uint8_t[av_packet.size];
    int memcpy_result = memcpy_s(vdec_in_buffer, av_packet.size, av_packet.data, av_packet.size);
    if (memcpy_result != EOK) { // check memcpy_s result
      HIAI_ENGINE_LOG( HIAI_ENGINE_RUN_ARGS_NOT_RIGHT, "Fail to copy av_packet data to vdec buffer, memcpy_s result:%d", memcpy_result);
      delete[] vdec_in_buffer;
      return HIAI_ERROR;
    }

    video_image_para->img.data.reset(vdec_in_buffer, default_delete<uint8_t[]>());
    video_image_para->img.size = av_packet.size;

    return HIAI_OK;
}

HIAI_StatusT StreamDemux::PrepareStreamRawData(AVPacket& av_packet, std::shared_ptr<StreamRawData>& video_image_data) {
	uint8_t* buffer;
	RawDataBufferHigh dataBuffer;
	int ret = hiai::HIAIMemory::HIAI_DMalloc(av_packet.size, (void*&)buffer, hiai::MALLOC_DEFAULT_TIME_OUT, hiai::HIAI_MEMORY_ATTR_MANUAL_FREE);
    if (HIAI_OK != ret) {
        printf("channel %d HIAI_DMalloc buffer faild\n", channel_id);
        return HIAI_ERROR;
	}

	memcpy_s(buffer, av_packet.size, av_packet.data, av_packet.size);
	dataBuffer.data = std::shared_ptr<uint8_t>(buffer, hiai::HIAIMemory::HIAI_DFree);
	dataBuffer.len_of_byte = av_packet.size;
	video_image_data = std::make_shared<StreamRawData>();
	video_image_data->buf = dataBuffer;
	video_image_data->info.channelId = channel_id;
	video_image_data->info.format = 0;
	video_image_data->info.isEOS = 0;
    return HIAI_OK;
}

void* StreamDemux::DemuxStream2AtlasPacket(void* arg)
{
    StreamDemux* _this = (StreamDemux*)arg;
    int video_index = _this->video_index;
    AVFormatContext* input_fmt = _this->input_fmt;
    AVBSFContext* bsf_ctx = _this->bsf_ctx;
    int ret;
    AVPacket pkt;
    av_init_packet(&pkt);
    unsigned long mSec = 1;
    if (_this->isRtspStream(input_fmt->url)) {
        mSec = 1;
    }
    else {
        mSec = 40;
    }
    while(!_this->isStop){
        milliseconds_sleep(mSec);
        while (1) {
            ret = av_read_frame(input_fmt, &pkt);
            if (ret == AVERROR_EOF) {
                std::cout << "av_read_frame failed, error:" << ret << std::endl;
                if (_this->ReopenAVStream(input_fmt) < 0) {
                    _this->isStop = 1;
                    break;
                }
                continue;
            }
            if (pkt.stream_index == video_index)
                break;
        }
        ret = _this->AddDecodeBsfInfo(bsf_ctx, pkt);
        if (ret != 0) {
            av_packet_unref(&pkt);
            av_init_packet(&pkt);
            std::cout << "AddDecodeBsfInfo failed" << std::endl;
            continue;
        }
//        std::shared_ptr<VideoImageParaT> output;
//        if (_this->PrepareVideoImageParaT(pkt, output) != HIAI_OK) {
//            av_packet_unref(&pkt);
//            av_init_packet(&pkt);
//            std::cout << "PrepareImage failed" << std::endl;
//            continue;
//        }

        std::shared_ptr<StreamRawData> output;
        if (_this->PrepareStreamRawData(pkt, output) != HIAI_OK) {
            std::cout << "PrepareImage failed" << std::endl;
            continue;
        }
        _this->SendImageData(output);
        av_packet_unref(&pkt);
        av_init_packet(&pkt);
    }
}
#else
int StreamDemux::Init(std::string& stream_url, int channel_id) {
	this->stream_url = stream_url;
	this->channel_id = channel_id;

	if (stream_msg->H264StreamMessageQueueInit(25) < 0) {
		return -1;
	}
	if (StreamInit() < 0) {
		return -1;
	}
	return 0;
}

int StreamDemux::Process() {
#ifdef SDL
	if (SdlInit(codec, sdlTexture, sdlRect, sdlRenderer) < 0) {
		return -1;
	}
#endif
	//StreamDemux::DemuxStream2Packet((void*)this);
	pthread_t tid;
	pthread_create(&tid, NULL, DemuxStream2Packet, (void*)this);
	//std::thread thread(&StreamDemux::DemuxStream2Packet, this);
}
#endif


int StreamDemux::AddDecodeBsfInfo(AVBSFContext* bsf_ctx, AVPacket& pkt){
    int ret = 0;
    ret = av_bsf_send_packet(bsf_ctx, &pkt);
    if (ret != 0) {
        std::cout << "av_bsf_send_packet failed" << std::endl;
        return ret;
    }
    ret = av_bsf_receive_packet(bsf_ctx, &pkt);
    if (ret != 0) {
        std::cout << "av_bsf_receive_packet failed" << std::endl;
        return ret;
    }
    return ret;
}

int StreamDemux::DecodePacket(AVCodecContext* codec, AVPacket& pkt, AVFrame* frame){
    int ret = 0;
    ret = avcodec_send_packet(codec, &pkt);
    if (ret != 0) {
        std::cout << "avcodec_send_packet failed" << std::endl;
        return ret;
    }
    ret = avcodec_receive_frame(codec, frame);
    if (ret != 0) {
        std::cout << "avcodec_receive_frame failed" << std::endl;
        return ret;
    }
    return ret;
}

void StreamDemux::GetImageFromFrame(uint8_t *dst_data[4], int dst_linesizes[4], AVFrame* frame){
    av_image_copy(dst_data, dst_linesizes, (const uint8_t**)(frame->data), frame->linesize, codec->pix_fmt, frame->width, frame->height);
}

void* StreamDemux::DemuxStream2Packet(void* arg){
	StreamDemux* _this = (StreamDemux*)arg;
	int video_index = _this->video_index;
	AVFormatContext* input_fmt = _this->input_fmt;
	AVBSFContext* bsf_ctx = _this->bsf_ctx;
	int ret;
    AVPacket pkt;
    av_init_packet(&pkt);
	unsigned long mSec = 1;
	if (_this->isRtspStream(input_fmt->url)) {
		mSec = 1;
	}
	else {
		mSec = 40;
	}
    while(!_this->isStop){
		milliseconds_sleep(mSec);
        while (1) {
            ret = av_read_frame(input_fmt, &pkt);
            if (ret == AVERROR_EOF) {
                std::cout << "av_read_frame failed, error:" << ret << std::endl;
                if (_this->ReopenAVStream(input_fmt) < 0) {
					_this->isStop = 1;
                    break;
                }
                continue;
            }
            if (pkt.stream_index == video_index)
                break;
        }
        ret = _this->AddDecodeBsfInfo(bsf_ctx, pkt);
        if (ret != 0) {
            std::cout << "AddDecodeBsfInfo failed" << std::endl;
            continue;
        }
		_this->stream_msg->H264StreamMessageQueueSend(pkt);
		//if (isSendMessage) {
		//	stream_msg->H264StreamMessageQueueSend(pkt);
		//}
    }
}

void StreamDemux::DemuxStream2YUV()
{
	int ret;
	AVPacket pkt;
	av_init_packet(&pkt);
	while (!isStop) {
		while (1) {
			ret = av_read_frame(input_fmt, &pkt);
			if (ret == AVERROR_EOF) {
				std::cout << "av_read_frame failed, error:" << ret << std::endl;
				if (ReopenAVStream(input_fmt) < 0) {
					isStop = 1;
					break;
				}
				continue;
			}
			if (pkt.stream_index == video_index)
				break;
		}
		ret = AddDecodeBsfInfo(bsf_ctx, pkt);
		if (ret != 0) {
			std::cout << "AddDecodeBsfInfo failed" << std::endl;
			continue;
		}
        ret = DecodePacket(codec, pkt, frame);
        if (ret != 0) {
            std::cout << "DecodePacket failed" << std::endl;
            continue;
        }
        GetImageFromFrame(video_dst_data, video_dst_linesize, frame);

#ifdef SDL
		SDLShowYUV(video_dst_data, video_dst_linesize);
#endif
	}
}

int StreamDemux::GetDemuxStream(int vencchn, unsigned char*& pdataOut, unsigned int& framesize, void* pArg)
{
	StreamDemux* _this = (StreamDemux*)pArg;
	if (_this == nullptr) return -1;
	return _this->stream_msg->H264StreamMessageQueueRecv(pdataOut, framesize);;
}

int StreamDemux::StartDemuxStream(int vencchn, void* pArg)
{
	StreamDemux* _this = (StreamDemux*)pArg;
	_this->stream_msg->H264StreamMessageRefresh();
	return 0;
}

int StreamDemux::StreamInit(){
    int ret = 0;
    ret = AVStreamInit(input_fmt, video_type, video_index, stream_url.c_str());
    if ( ret != 0 ) {
        goto EXIT1;
    }
    input_fmt->max_analyze_duration = AV_TIME_BASE * 10;
    input_fmt->probesize = input_fmt->probesize * 2;
    std::cout << "probesize:" << input_fmt->probesize << ", max_analyze_duration:" << input_fmt->max_analyze_duration << std::endl;

    ret = AVDecoderH264Init(codec, input_fmt, video_index);
    if ( ret != 0 ) {
        std::cout << "AVDecoderH264Init failed." << std::endl;
        goto EXIT2;
    }
    ret = AVH264BsfInit(bsf_ctx, codec, video_index);
    if ( ret != 0 ) {
        std::cout << "AVH264BsfInit failed." << std::endl;
        goto EXIT3;
    }
    //申请输出视频帧的空间内存，解码后的视频帧需要经过转换才能播放使用
    ret = av_image_alloc(video_dst_data, video_dst_linesize, codec->width, codec->height, codec->pix_fmt, 1);
    if ( ret < 0 ) {
        std::cout << "av_image_alloc failed" << std::endl;
        goto EXIT3;
    }
    frame = av_frame_alloc();
    return 0;
EXIT4:
    av_freep(&video_dst_data[0]);
    av_frame_free(&frame);
    av_bsf_free(&bsf_ctx);
EXIT3:
    avcodec_free_context(&codec);
EXIT2:
    avformat_close_input(&input_fmt);
EXIT1:
    return -1;
}

bool StreamDemux::isRtspStream(const char* url) {
    if (strncmp(url, "rtsp://", 7) == 0) {
        return true;
    }
    return false;
}

void StreamDemux::setRtspDict(AVDictionary*& options) {
    //av_dict_set(&options, "buffer_size", "3110400", 0);	//tcp无效
    av_dict_set(&options, "rtsp_transport", "tcp", 0);
    //av_dict_set(&options, "max_delay", "10000", 0);
    av_dict_set(&options, "stimeout", "10000", 0);
    //av_dict_set(&options, "pkt_size", "10485760", 0	//tcp无效
    av_dict_set(&options, "reorder_queue_size", "0", 0);
}

int StreamDemux::ReopenAVStream(AVFormatContext*& fmt) {
    int index;
    std::string url = fmt->url;
    AVDictionary* options = NULL;
    AVInputFormat* input_fromat = fmt->iformat;
    avformat_close_input(&fmt);
    fmt = nullptr;
    if (isRtspStream(url.c_str())) {
        setRtspDict(options);
    }
    int ret = avformat_open_input(&fmt, url.c_str(), input_fromat, &options);
    if (ret != 0) {
        std::cout << "avformat_open_input failed" << std::endl;
        return -1;
    }
    ret = avformat_find_stream_info(fmt, NULL);
    if (ret != 0) {
        std::cout << "avformat_find_stream_info failed" << std::endl;
        avformat_close_input(&fmt);
        return -1;
    }
    std::cout << "reopen succeed." << std::endl;
    return 0;
}

int StreamDemux::AVStreamInit(AVFormatContext*& fmt, AVCodecID& codec_id, int& video_index, const char* filename) {
    //打开视频流
    AVDictionary* options = NULL;
    if (isRtspStream(filename)) {
        std::cout << "open "<<filename<<" resource" << std::endl;
        setRtspDict(options);
    }

    int ret = avformat_open_input(&fmt, filename, NULL, &options);
    if (ret != 0) {
        std::cout << "avformat_open_input failed" << std::endl;
        return -1;
    }
    /************************查找输入视频流信息**********************************/
    ret = avformat_find_stream_info(fmt, NULL);
    if (ret != 0) {
        std::cout << "avformat_find_stream_info failed" << std::endl;
        avformat_close_input(&fmt);
        return -1;
    }
    //查找视频流索引
    video_index = av_find_best_stream(fmt, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    std::cout << "video index = " << ret << std::endl;
    //获取视频流的编码格式
    codec_id = fmt->streams[video_index]->codecpar->codec_id;
    if (video_index == AV_CODEC_ID_H264) {
        std::cout <<"codec_id:" << codec_id << ", video type:" << codec_id << " AV_CODEC_ID_H264" << std::endl;
    }
    return 0;
}
/*
 * 功能：根据编解码器的参数，创建H264的比特流过滤器，视频帧送入解码器之前，需要先添加解码头信息
 */
int StreamDemux::AVH264BsfInit(AVBSFContext*& bsf_ctx, const AVCodecContext* codec, int video_index) {
    const AVBitStreamFilter* video_filter;
    video_filter = av_bsf_get_by_name("h264_mp4toannexb");
    if (video_filter == nullptr) {
        std::cout << "can not find video_filter" << std::endl;
        return -1;
    }
    if (av_bsf_alloc(video_filter, &bsf_ctx) != 0) {
        std::cout << "av_bsf_alloc failed" << std::endl;
        return -1;
    }
    //必须设置解码器参数，否则会出错
    if (avcodec_parameters_from_context(bsf_ctx->par_in, codec) < 0) {
        std::cout << "avcodec_parameters_copy failed" << std::endl;
        return -1;
    }

    bsf_ctx->time_base_in = codec->time_base;
    //以上信息设置完以后，再初始化
    if (av_bsf_init(bsf_ctx) != 0) {
        std::cout << "av_bsf_alloc failed" << std::endl;
        av_bsf_free(&bsf_ctx);
        return -1;
    }
    return 0;
}

/*
 * 功能：根据输入的视频流格式，创建解码器
 */
int StreamDemux::AVDecoderInit(AVCodecContext*& codec, AVFormatContext* fmt, AVCodecID video_type, int video_index) {
    AVCodec* decoder;

    decoder = avcodec_find_decoder(video_type);
    if (decoder == nullptr) {
        std::cout << "avcodec_find_decoder failed" << std::endl;
        return -1;
    }
    codec = avcodec_alloc_context3(decoder);
    if (codec == nullptr) {
        std::cout << "avcodec_alloc_context3 failed" << std::endl;
        return -1;
    }
    if (avcodec_parameters_to_context(codec, fmt->streams[video_index]->codecpar) < 0) {
        std::cout << "avcodec_parameters_to_context failed" << std::endl;
        avcodec_free_context(&codec);
        return -1;
    }
    codec->gop_size = 25;
    AVRational timebase = { 1,25 };
    codec->time_base = timebase;
    if (avcodec_open2(codec, decoder, NULL) != 0) {
        std::cout << "avcodec_find_decoder failed" << std::endl;
        avcodec_free_context(&codec);
        return -1;
    }

    std::cout << "width:" << codec->width << " ";
    std::cout << "height:" << codec->height << " ";
    std::cout << "gop_size:" << codec->gop_size << " ";
    std::cout << "time_base:" << codec->time_base.num <<","<< codec->time_base.den << " ";
    std::cout << "pix_fmt:" << av_get_pix_fmt_name(codec->pix_fmt) << " ";
    std::cout << "max_b_frames:" << codec->max_b_frames << std::endl;

    return 0;
}
int StreamDemux::AVDecoderH264Init(AVCodecContext*& codec, AVFormatContext* fmt, int video_index) {
    AVCodec* decoder;
    decoder = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (decoder == nullptr) {
        std::cout << "avcodec_find_decoder failed" << std::endl;
        return -1;
    }
    codec = avcodec_alloc_context3(decoder);
    if (codec == nullptr) {
        std::cout << "avcodec_alloc_context3 failed" << std::endl;
        return -1;
    }
    if (avcodec_parameters_to_context(codec, fmt->streams[video_index]->codecpar) < 0) {
        std::cout << "avcodec_parameters_to_context failed" << std::endl;
        avcodec_free_context(&codec);
        return -1;
    }
    codec->gop_size = 25;
    AVRational timebase = { 1,25 };
    codec->time_base = timebase;
    if (avcodec_open2(codec, decoder, NULL) != 0) {
        std::cout << "avcodec_find_decoder failed" << std::endl;
        avcodec_free_context(&codec);
        return -1;
    }

    std::cout << "width:" << codec->width << " ";
    std::cout << "height:" << codec->height << " ";
    std::cout << "gop_size:" << codec->gop_size << " ";
    std::cout << "time_base:" << codec->time_base.num << "," << codec->time_base.den << " ";
    std::cout << "pix_fmt:" << av_get_pix_fmt_name(codec->pix_fmt) << " ";
    std::cout << "max_b_frames:" << codec->max_b_frames << std::endl;

    return 0;
}

#ifdef SDL
int StreamDemux::sfp_refresh_thread(void* opaque) {
	thread_exit = 0;
	thread_pause = 0;

	while (!thread_exit) {
		if (!thread_pause) {
			SDL_Event event;
			event.type = SFM_REFRESH_EVENT;
			SDL_PushEvent(&event);
		}
		SDL_Delay(40);
	}
	thread_exit = 0;
	thread_pause = 0;
	//Break
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}
void StreamDemux::SDLShowYUV(uint8_t* video_dst_data[4], int video_dst_linesize[4]){
    SDL_UpdateYUVTexture(sdlTexture, &sdlRect, video_dst_data[0], video_dst_linesize[0],
        video_dst_data[1], video_dst_linesize[1], video_dst_data[2], video_dst_linesize[2]);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);
}

/*
 * 功能：初始化SDL
 */
int StreamDemux::SdlInit(AVCodecContext* codec, SDL_Texture*& sdlTexture, SDL_Rect& sdlRect, SDL_Renderer*& sdlRenderer) {
    int screen_w = 0, screen_h = 0;
    SDL_Window* screen;
	SDL_Thread* video_tid;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }
    screen_w = codec->width;
    screen_h = codec->height;
    screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screen_w, screen_h,SDL_WINDOW_OPENGL);
    if (!screen) {
        printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return -1;
    }
    sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, codec->width, codec->height);
    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = screen_w;
    sdlRect.h = screen_h;

	video_tid = SDL_CreateThread(sfp_refresh_thread, NULL, NULL);
    return 0;
}
#endif

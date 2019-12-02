#ifndef STREAMDEMUX_H
#define STREAMDEMUX_H
#include <atomic>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <mutex>
#include <thread>
extern "C" {
#include <pthread.h>
#include "libavfilter/avfilter.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/timestamp.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/frame.h"
#include "libavutil/threadmessage.h"
}
#include "StreamMessage.h"
#include "TimeDelay.h"

#define ATLAS300

#ifdef SDL
#include <sdl2.0/SDL.h>
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)
#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)
#endif
#ifdef ATLAS300
#define INPUT_SIZE 1
#define OUTPUT_SIZE 1
#include "common_data_type.h"
#include "hiaiengine/engine.h"
#include "hiaiengine/multitype_queue.h"
#include "error_code.h"
#include "hiaiengine/ai_memory.h"
#include "utils_common.h"
#include <dirent.h>
#include <stdint.h>
#include <unistd.h>
#include "video_analysis_params.h"
#endif

class StreamDemux
#ifdef ATLAS300
    : public hiai::Engine
#endif
{
public:
    StreamDemux();
	~StreamDemux();
    /**
     * @brief current engine initialize function
     * @param [in] config: hiai engine config
     * @param [in] model_desc: hiai AI model description
     * @return 0: initialize success; HIAI_ERROR:initialize failed
     */
#ifdef ATLAS300
    HIAI_StatusT Init(const hiai::AIConfig &config, const std::vector<hiai::AIModelDescription> &model_desc);
	/**
	 * @brief HIAI_DEFINE_PROCESS : reload Engine Process
	 * @param [in] INPUT_SIZE: the input size of engine
	 * @param [in] OUTPUT_SIZE: the output size of engine
	 */
	HIAI_DEFINE_PROCESS(INPUT_SIZE, OUTPUT_SIZE)
#else
	int Init(std::string& stream_url, int channel_id);
	int Process();
#endif
    static int GetDemuxStream(int vencchn, unsigned char*& pdataOut, unsigned int& framesize, void* pArg);
    static int StartDemuxStream(int vencchn, void* pArg);
private:
    int isStop = 0;
    unsigned int channel_id = -1;
    std::string stream_url;
	std::mutex message_status_mutex;

    int video_index;
    AVFrame* frame;
    AVCodecID video_type;
    AVCodecContext* codec;
    AVBSFContext* bsf_ctx = nullptr;
    AVFormatContext* input_fmt = nullptr;

    int video_dst_linesize[4];
    uint8_t* video_dst_data[4] = { NULL };

	StreamMessage* stream_msg;
#ifdef SDL
	static int thread_exit;
	static int thread_pause;
    SDL_Event event;
    SDL_Rect sdlRect;
    SDL_Thread* video_tid;
    SDL_Texture* sdlTexture;
    SDL_Renderer* sdlRenderer;
	static int sfp_refresh_thread(void* opaque);
    void SDLShowYUV(uint8_t* video_dst_data[4], int video_dst_linesize[4]);
    int SdlInit(AVCodecContext* codec, SDL_Texture*& sdlTexture, SDL_Rect& sdlRect, SDL_Renderer*& sdlRenderer);
#endif
    int StreamInit();
    static void* DemuxStream2Packet(void* arg);
	void DemuxStream2YUV();
    int AddDecodeBsfInfo(AVBSFContext* bsf_ctx, AVPacket& pkt);
    int DecodePacket(AVCodecContext* codec, AVPacket& pkt, AVFrame* frame);
    void GetImageFromFrame(uint8_t *dst_data[4], int dst_linesizes[4], AVFrame* frame);
#ifdef ATLAS300
    int kAllowedMaxImageMemory = 67108864;

    static void* DemuxStream2AtlasPacket(void* arg);
	void SendImageData(shared_ptr<StreamRawData>& video_image_data);
    void SendImageData(shared_ptr<VideoImageParaT>& video_image_data);
    HIAI_StatusT PrepareStreamRawData(AVPacket& av_packet, std::shared_ptr<StreamRawData>& video_image_data);
    HIAI_StatusT PrepareVideoImageParaT(AVPacket& av_packet, std::shared_ptr<VideoImageParaT> &video_image_para);
#endif
    bool isRtspStream(const char* url);
    void setRtspDict(AVDictionary*& options);
    int ReopenAVStream(AVFormatContext*& fmt);
    int AVH264BsfInit(AVBSFContext*& bsf_ctx, const AVCodecContext* codec, int video_index);
    int AVStreamInit(AVFormatContext*& fmt, AVCodecID& codec_id, int& video_index, const char* filename);
    int AVDecoderInit(AVCodecContext*& codec, AVFormatContext* fmt, AVCodecID video_type, int video_index);
    int AVDecoderH264Init(AVCodecContext*& codec, AVFormatContext* fmt, int video_index);
};

#endif // STREAMDEMUX_H

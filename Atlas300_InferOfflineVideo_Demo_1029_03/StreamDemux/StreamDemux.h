#ifndef STREAMDEMUX_H
#define STREAMDEMUX_H
#include "common_data_type.h"
#include "hiaiengine/engine.h"
#include "hiaiengine/multitype_queue.h"
#include "error_code.h"
#include "hiaiengine/ai_memory.h"
#include "utils_common.h"
#include <dirent.h>
#include <stdint.h>
#include <unistd.h>
#include <atomic>

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#define INPUT_SIZE 1
#define OUTPUT_SIZE 1

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

class StreamDemux : public hiai::Engine {
{
public:
    StreamDemux();
    /**
     * @brief current engine initialize function
     * @param [in] config: hiai engine config
     * @param [in] model_desc: hiai AI model description
     * @return HIAI_OK: initialize success; HIAI_ERROR:initialize failed
     */
    HIAI_StatusT Init(const hiai::AIConfig &config, const std::vector<hiai::AIModelDescription> &model_desc);
    /**
     * @brief HIAI_DEFINE_PROCESS : reload Engine Process
     * @param [in] INPUT_SIZE: the input size of engine
     * @param [in] OUTPUT_SIZE: the output size of engine
     */
    HIAI_DEFINE_PROCESS(INPUT_SIZE, OUTPUT_SIZE)

private:
    int isStop = 0;
    int channel_id = -1;
    std::string stream_url;

    int video_index;
    AVFrame* frame;
    AVCodecID video_type;
    AVCodecContext* codec;
    AVBSFContext* bsf_ctx = nullptr;
    AVFormatContext* input_fmt = nullptr;

    int video_dst_linesize[4];
    uint8_t* video_dst_data[4] = { NULL };

    int StreamInit();
    void DemuxStream2Packet();
    int AddDecodeBsfInfo(AVBSFContext* bsf_ctx, AVPacket& pkt);
    int DecodePacket(AVCodecContext* codec, AVPacket& pkt, AVFrame* frame);
    void GetImageFromFrame(uint8_t *dst_data[4], int dst_linesizes[4], AVFrame* frame);
    int PrepareImage(AVPacket& av_packet, std::shared_ptr<StreamRawData>& video_image_data);

    bool isRtspStream(const char* url);
    void setRtspDict(AVDictionary*& options);
    int ReopenAVStream(AVFormatContext*& fmt);
    int AVH264BsfInit(AVBSFContext*& bsf_ctx, const AVCodecContext* codec, int video_index);
    int AVStreamInit(AVFormatContext*& fmt, AVCodecID& codec_id, int& video_index, const char* filename);
    int AVDecoderInit(AVCodecContext*& codec, AVFormatContext* fmt, AVCodecID video_type, int video_index);
    int AVDecoderH264Init(AVCodecContext*& codec, AVFormatContext* fmt, int video_index);

};

#endif // STREAMDEMUX_H

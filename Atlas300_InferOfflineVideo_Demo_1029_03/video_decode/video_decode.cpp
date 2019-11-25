/**
 * ============================================================================
 *
 * Copyright (C) 2018, Hisilicon Technologies Co., Ltd. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1 Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   2 Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   3 Neither the names of the copyright holders nor the names of the
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * ============================================================================
 */

#include "video_decode.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/prctl.h>

#include <fstream>
#include <memory>
#include <mutex>
#include <regex>
#include <sstream>
#include <thread>

#include "hiaiengine/log.h"
#include "hiaiengine/data_type_reg.h"

using namespace std;

namespace {
const int kWait10Milliseconds = 10000; // wait 10ms

const int kNoFlag = 0; // no flag

const int kHandleSuccessful = 0; // the process handled successfully

const string kStrChannelId1 = "channel1"; // channle id 1 string

const string kStrChannelId2 = "channel2"; // channle id 2 string

// standard: 4096 * 4096 * 4 = 67108864 (64M)
const int kAllowedMaxImageMemory = 67108864;

const string kNeedRemoveStr = " \r\n\t"; // the string need remove

const string kVideoImageParaType = "VideoImageParaT"; // video image para type

const int kInvalidVideoIndex = -1; // invalid video index

const string kRtspTransport = "rtsp_transport"; // rtsp transport

const string kUdp = "udp"; // video format udp

const string kTcp = "tcp"; // video format tcp

const string kBufferSize = "buffer_size"; // buffer size string

const string kMaxBufferSize = "104857600"; // maximum buffer size:100MB

const string kMaxDelayStr = "max_delay"; // maximum delay string

const string kMaxDelayValue = "100000000"; // maximum delay time:100s

const string kTimeoutStr = "stimeout"; // timeout string

const string kTimeoutValue = "5000000"; // timeout:5s

const string kPktSize = "pkt_size"; // ffmpeg pakect size string

const string kPktSizeValue = "10485760"; // ffmpeg packet size value:10MB

const string kReorderQueueSize = "reorder_queue_size"; // reorder queue size

const string kReorderQueueSizeValue = "0"; // reorder queue size value

const string kThreadNameHead = "handle_"; // thread name head string

const int kErrorBufferSize = 1024; // buffer size for error info

const int kThreadNameLength = 32; // thread name string length

const string kRegexSpace = "^[ ]*$"; // regex for check string is empty

// regex for verify .mp4 file name
const string kRegexMp4File = "^/((?!\\.\\.).)*\\.(mp4)$";

// regex for verify RTSP rtsp://ip:port/channelname
const string kRegexRtsp =
    "^rtsp://(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[0-9])\\."
        "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
        "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
        "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)"
        ":([1-9]|[1-9]\\d|[1-9]\\d{2}|[1-9]\\d{3}|[1-5]\\d{4}|"
        "6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])/"
        "(.{1,100})$";
}

HIAI_REGISTER_DATA_TYPE("VideoImageParaT", VideoImageParaT);

VideoDecode::VideoDecode() {
  channel1_ = ""; // initialize channel1 to empty string
  channel2_ = ""; // initialize channel2 to empty string
}

VideoDecode::~VideoDecode() {
}

void VideoDecode::SendFinishedData() {
  VideoImageInfoT video_image_info;
  video_image_info.is_finished = true;

  hiai::ImageData<unsigned char> image_data;
  shared_ptr<VideoImageParaT> video_image_para = make_shared<VideoImageParaT>();
  video_image_para->img = image_data;
  video_image_para->video_image_info = video_image_info;

  HIAI_StatusT hiai_ret = HIAI_OK;

  // send finished data to next engine, use output port:0
  do {
    hiai_ret = SendData(0, kVideoImageParaType, static_pointer_cast<void>(video_image_para));
    if (hiai_ret == HIAI_QUEUE_FULL) { // check hiai queue is full
      HIAI_ENGINE_LOG("Queue full when send finished data, sleep 10ms");
      usleep(kWait10Milliseconds); // sleep 10 ms
    }
  } while (hiai_ret == HIAI_QUEUE_FULL); // loop when hiai queue is full

  if (hiai_ret != HIAI_OK) {
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT, "Send finished data failed! error code: %d", hiai_ret);
  }
}

int VideoDecode::GetVideoIndex(AVFormatContext* av_format_context) {
  if (av_format_context == nullptr) { // verify input pointer
    return kInvalidVideoIndex;
  }

  // get video index in streams
  for (int i = 0; i < av_format_context->nb_streams; i++) {
    if (av_format_context->streams[i]->codecpar->codec_type
        == AVMEDIA_TYPE_VIDEO) { // check is media type is video
      return i;
    }
  }

  return kInvalidVideoIndex;
}

bool VideoDecode::CheckVideoType(int video_index, AVFormatContext* av_format_context, VideoType &video_type) {
  AVStream* in_stream = av_format_context->streams[video_index];
  HIAI_ENGINE_LOG("Display video stream resolution, width:%d, height:%d", in_stream->codecpar->width, in_stream->codecpar->height);

  if (in_stream->codecpar->codec_id == AV_CODEC_ID_H264) { // video type: h264
    video_type = kH264;
    HIAI_ENGINE_LOG("Video type:H264");
    return true;
  } else if (in_stream->codecpar->codec_id == AV_CODEC_ID_HEVC) { // h265
    video_type = kH265;
    HIAI_ENGINE_LOG("Video type:H265");
    return true;
  } else { // the video type is invalid
    video_type = kInvalidTpye;
    HIAI_ENGINE_LOG( "The video type is invalid, should be h264 or h265 AVCodecID:%d(detail type please to view enum AVCodecID in ffmpeg)",
        in_stream->codecpar->codec_id);
    return false;
  }
}

void VideoDecode::InitVideoStreamFilter(VideoType video_type, const AVBitStreamFilter* &video_filter) {
  if (video_type == kH264) { // check video type is h264
    video_filter = av_bsf_get_by_name("h264_mp4toannexb");
  } else { // the video type is h265
    video_filter = av_bsf_get_by_name("hevc_mp4toannexb");
  }
}

const string &VideoDecode::GetChannelValue(const string &channel_id) {
  if (channel_id == kStrChannelId1) { // check channel is channel1
    return channel1_;
  } else { // the channel is channel2
    return channel2_;
  }
}

void VideoDecode::SetDictForRtsp(const string& channel_value,
                                 AVDictionary* &avdic) {
  if (IsValidRtsp(channel_value)) { // check channel value is valid rtsp address
    HIAI_ENGINE_LOG("Set parameters for %s", channel_value.c_str());
    avformat_network_init();

    av_dict_set(&avdic, kRtspTransport.c_str(), kTcp.c_str(), kNoFlag);
    av_dict_set(&avdic, kBufferSize.c_str(), kMaxBufferSize.c_str(), kNoFlag);
    av_dict_set(&avdic, kMaxDelayStr.c_str(), kMaxDelayValue.c_str(), kNoFlag);
    av_dict_set(&avdic, kTimeoutStr.c_str(), kTimeoutValue.c_str(), kNoFlag);
    av_dict_set(&avdic, kReorderQueueSize.c_str(), kReorderQueueSizeValue.c_str(), kNoFlag);
    av_dict_set(&avdic, kPktSize.c_str(), kPktSizeValue.c_str(), kNoFlag);
  }
}

bool VideoDecode::OpenVideoFromInputChannel(const string &channel_value, AVFormatContext* &av_format_context) {
  AVDictionary* avdic = nullptr;
  SetDictForRtsp(channel_value, avdic);

  int ret_open_input_video = avformat_open_input(&av_format_context, channel_value.c_str(), nullptr, &avdic);

  if (ret_open_input_video < kHandleSuccessful) { // check open video result
    char buf_error[kErrorBufferSize];
    av_strerror(ret_open_input_video, buf_error, kErrorBufferSize);

    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT, "Could not open video:%s, result of avformat_open_input:%d, error info:%s",
                    channel_value.c_str(), ret_open_input_video, buf_error);

    if (avdic != nullptr) { // free AVDictionary
      av_dict_free(&avdic);
    }

    return false;
  }

  if (avdic != nullptr) { // free AVDictionary
    av_dict_free(&avdic);
  }

  return true;
}

bool VideoDecode::InitVideoParams(int videoindex, VideoType &video_type, AVFormatContext* av_format_context, AVBSFContext* &bsf_ctx) {
  // check video type, only support h264 and h265
  if (!CheckVideoType(videoindex, av_format_context, video_type)) {
    avformat_close_input(&av_format_context);

    return false;
  }

  const AVBitStreamFilter* video_filter;
  InitVideoStreamFilter(video_type, video_filter);
  if (video_filter == nullptr) { // check video fileter is nullptr
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
                    "Unkonw bitstream filter, video_filter is nullptr!");
    return false;
  }

  // checke alloc bsf context result
  if (av_bsf_alloc(video_filter, &bsf_ctx) < kHandleSuccessful) {
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
                    "Fail to call av_bsf_alloc!");
    return false;
  }

  // check copy parameters result
  if (avcodec_parameters_copy(bsf_ctx->par_in,av_format_context->streams[videoindex]->codecpar) < kHandleSuccessful) {
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT, "Fail to call avcodec_parameters_copy!");
    return false;
  }

  bsf_ctx->time_base_in = av_format_context->streams[videoindex]->time_base;

  // check initialize bsf contextreult
  if (av_bsf_init(bsf_ctx) < kHandleSuccessful) {
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
                    "Fail to call av_bsf_init!");
    return false;
  }

  return true;
}

void VideoDecode::UnpackVideo2Image(const string &channel_id) {
  char thread_name_log[kThreadNameLength];
  AVFormatContext* av_format_context;
  string thread_name = kThreadNameHead + channel_id;
  prctl(PR_SET_NAME, (unsigned long) thread_name.c_str());
  prctl(PR_GET_NAME, (unsigned long) thread_name_log);
  HIAI_ENGINE_LOG("Unpack video to image from:%s, thread name:%s", channel_id.c_str(), thread_name_log);

  string channel_value = GetChannelValue(channel_id);
ARGIN:
  av_format_context = avformat_alloc_context();

  // check open video result
  if (!OpenVideoFromInputChannel(channel_value, av_format_context)) {
    return;
  }

  int videoindex = GetVideoIndex(av_format_context);
  if (videoindex == kInvalidVideoIndex) { // check video index is valid
    HIAI_ENGINE_LOG( HIAI_ENGINE_RUN_ARGS_NOT_RIGHT, "Video index is -1, current media has no video info, channel id:%s", channel_id.c_str());
    return;
  }

  AVBSFContext* bsf_ctx;
  VideoType video_type = kInvalidTpye;

  // check initialize video parameters result
  if (!InitVideoParams(videoindex, video_type, av_format_context, bsf_ctx)) {
    return;
  }

  AVPacket av_packet;
  // loop to get every frame from video stream
  while (1) {
      int ret = av_read_frame(av_format_context, &av_packet);
      if(ret < 0){
          printf("EOF, video stream is disconnected.\n");
          av_bsf_free(&bsf_ctx);
          avformat_close_input(&av_format_context);
          goto ARGIN;
      }
    if (av_packet.stream_index == videoindex) { // check current stream is video
      // send video packet to ffmpeg
      if (av_bsf_send_packet(bsf_ctx, &av_packet) != kHandleSuccessful) {
        HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT, "Fail to call av_bsf_send_packet, channel id:%s", channel_id.c_str());
      }

      // receive single frame from ffmpeg
      while (av_bsf_receive_packet(bsf_ctx, &av_packet) == kHandleSuccessful) {

        if (av_packet.size <= 0 || av_packet.size > kAllowedMaxImageMemory) {
          continue;
        }

        uint8_t* buffer;
        HIAI_StatusT ret = hiai::HIAIMemory::HIAI_DMalloc(av_packet.size, (void*&)buffer, hiai::MALLOC_DEFAULT_TIME_OUT, hiai::HIAI_MEMORY_ATTR_MANUAL_FREE);
        if (HIAI_OK != ret) {
            printf("channel %d HIAI_DMalloc buffer faild\n", channelId);
            av_packet_unref(&av_packet);
            break;
        }

        memcpy_s(buffer, av_packet.size, av_packet.data, av_packet.size);
        dataBuffer.data = std::shared_ptr<uint8_t>(buffer, hiai::HIAIMemory::HIAI_DFree);
        dataBuffer.len_of_byte = av_packet.size;
        blockId++;
        std::shared_ptr<StreamRawData> output = std::make_shared<StreamRawData>();
        output->buf = dataBuffer;
        output->info.channelId = channelId;
        output->info.format = format;
        output->info.isEOS = 0;

        ret = SendData(0, "StreamRawData", std::static_pointer_cast<void>(output));
        if (ret != HIAI_OK) {
            printf("channel %d StreamPuller send data failed %d\n", channelId, ret);
            if (1) {
                printf("channel %d sleep for 1 seconds\n", channelId);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

        av_packet_unref(&av_packet);
      }
    }
  }
    printf("FFmpeg video thread exit.\n");
  av_bsf_free(&bsf_ctx); // free AVBSFContext pointer
  avformat_close_input(&av_format_context); // close input video

  // send last yuv image data after call vdec
  //SendImageData(video_image_para);

  HIAI_ENGINE_LOG("Ffmpeg read frame finished, channel id:%s", channel_id.c_str());
}

bool VideoDecode::VerifyVideoWithUnpack(const string &channel_value) {
  HIAI_ENGINE_LOG("Start to verify unpack video file:%s",
                  channel_value.c_str());

  AVFormatContext* av_format_context = avformat_alloc_context();
  AVDictionary* avdic = nullptr;
  SetDictForRtsp(channel_value, avdic);

  int ret_open_input_video = avformat_open_input(&av_format_context,
                                                 channel_value.c_str(), nullptr,
                                                 &avdic);
  if (ret_open_input_video < kHandleSuccessful) { // check open video result
    char buf_error[kErrorBufferSize];
    av_strerror(ret_open_input_video, buf_error, kErrorBufferSize);

    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT, "Could not open video:%s, "
                    "result of avformat_open_input:%d, error info:%s",
                    channel_value.c_str(), ret_open_input_video, buf_error);

    if (avdic != nullptr) { // free AVDictionary
      av_dict_free(&avdic);
    }

    return false;
  }

  if (avdic != nullptr) { // free AVDictionary
    av_dict_free(&avdic);
  }

  int video_index = GetVideoIndex(av_format_context);
  if (video_index == kInvalidVideoIndex) { // check video index is valid
    HIAI_ENGINE_LOG(
        HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
        "Video index is -1, current media stream has no video info:%s",
        channel_value.c_str());

    avformat_close_input(&av_format_context);
    return false;
  }

  VideoType video_type = kInvalidTpye;
  bool is_valid = CheckVideoType(video_index, av_format_context, video_type);
  avformat_close_input(&av_format_context);

  return is_valid;
}

bool VideoDecode::VerifyVideoType() {
  // check channel1 and channel2 is not empty
  if (!IsEmpty(channel1_, kStrChannelId1)
      && !IsEmpty(channel2_, kStrChannelId2)) {
    return (VerifyVideoWithUnpack(channel1_) && VerifyVideoWithUnpack(channel2_));
  } else if (!IsEmpty(channel1_, kStrChannelId1)) { // channel1 is not empty
    return VerifyVideoWithUnpack(channel1_);
  } else { // channel2 is not empty
    return VerifyVideoWithUnpack(channel2_);
  }
}

void VideoDecode::MultithreadHandleVideo() {
  // create two thread unpacke channel1 and channel2 video in same time
  if (!IsEmpty(channel1_, kStrChannelId1)
      && !IsEmpty(channel2_, kStrChannelId2)) {
    thread thread_channel_1(&VideoDecode::UnpackVideo2Image, this, kStrChannelId1);
    thread thread_channel_2(&VideoDecode::UnpackVideo2Image, this, kStrChannelId2);

    thread_channel_1.join();
    thread_channel_2.join();
  } else if (!IsEmpty(channel1_, kStrChannelId1)) { // unpacke channel1 video
    UnpackVideo2Image(kStrChannelId1);
  } else { // unpacke channel2 video
    UnpackVideo2Image(kStrChannelId2);
  }
}

HIAI_StatusT VideoDecode::Init(const hiai::AIConfig &config, const vector<hiai::AIModelDescription> &model_desc) {
  HIAI_ENGINE_LOG("Start process!");

  // get channel values from configs item
  for (int index = 0; index < config.items_size(); ++index) {
    const ::hiai::AIConfigItem &item = config.items(index);

    // get channel1 value
    if (item.name() == kStrChannelId1) {
      channel1_ = item.value();
      continue;
    }

    // get channel2 value
    if (item.name() == kStrChannelId2) {
      channel2_ = item.value();
      continue;
    }

    if(item.name() == "channel1_id"){
        channelId = std::atoi(item.value().c_str());
    }
  }

  // verify channel values are valid
  if (!VerifyChannelValues()) {
    return HIAI_ERROR;
  }

  HIAI_ENGINE_LOG("End process!");
  return HIAI_OK;
}

bool VideoDecode::IsEmpty(const string &input_str, const string &channel_id) {
  regex regex_space(kRegexSpace.c_str());

  // check input string is empty or spaces
  if (regex_match(input_str, regex_space)) {
    HIAI_ENGINE_LOG("The channel string is empty or all spaces, channel id:%s",
                    channel_id.c_str());
    return true;
  }

  return false;
}

bool VideoDecode::VerifyVideoSourceName(const string &input_str) {
  // verify input string is valid mp4 file name or rtsp address
  if (!IsValidRtsp(input_str) && !IsValidMp4File(input_str)) {
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
                    "Invalid mp4 file name or RTSP name:%s", input_str.c_str());
    return false;
  }

  return true;
}

bool VideoDecode::IsValidRtsp(const string &input_str) {
  regex regex_rtsp_address(kRegexRtsp.c_str());

  // verify input string is valid rtsp address
  if (regex_match(input_str, regex_rtsp_address)) {
    return true;
  }

  return false;
}

bool VideoDecode::IsValidMp4File(const string &input_str) {
  regex regex_mp4_file_name(kRegexMp4File.c_str());

  // verify input string is valid mp4 file name
  if (regex_match(input_str, regex_mp4_file_name)) {
    return true;
  }

  return false;
}

bool VideoDecode::VerifyChannelValues() {
  // check channel1 and channel2 are empty
  if (IsEmpty(channel1_, kStrChannelId1)
      && IsEmpty(channel2_, kStrChannelId2)) {
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT,
                    "Both channel1 and channel2 are empty!");
    return false;
  }

  // verify channel1 value when channel1 is not empty
  if (!IsEmpty(channel1_, kStrChannelId1)) {
    // deletes the space at the head of the string
    channel1_.erase(0, channel1_.find_first_not_of(kNeedRemoveStr.c_str()));

    // deletes spaces at the end of the string
    channel1_.erase(channel1_.find_last_not_of(kNeedRemoveStr.c_str()) + 1);

    HIAI_ENGINE_LOG("Display channel1:%s", channel1_.c_str());

//    if (!VerifyVideoSourceName(channel1_)) { // verify channel1
//      return false;
//    }
  }

  // verify channel2 value when channel1 is not empty
  if (!IsEmpty(channel2_, kStrChannelId2)) {
    // deletes the space at the head of the string
    channel2_.erase(0, channel2_.find_first_not_of(kNeedRemoveStr.c_str()));

    // deletes spaces at the end of the string
    channel2_.erase(channel2_.find_last_not_of(kNeedRemoveStr.c_str()) + 1);

    HIAI_ENGINE_LOG("Display channel2:%s", channel2_.c_str());

    if (!VerifyVideoSourceName(channel2_)) { // verify channel2
      return false;
    }
  }

  return true;
}

void VideoDecode::SendImageData(shared_ptr<VideoImageParaT> &video_image_data) {
  HIAI_StatusT hiai_ret = HIAI_OK;

  if (video_image_data == nullptr) { // the queue is empty and return
    return;
  }

  // send image data
  do {
    hiai_ret = SendData(0, kVideoImageParaType, static_pointer_cast<void>(video_image_data));
    if (hiai_ret == HIAI_QUEUE_FULL) { // check queue is full
      HIAI_ENGINE_LOG("The queue is full when send image data, sleep 10ms");
      usleep(kWait10Milliseconds); // sleep 10 ms
    }
  } while (hiai_ret == HIAI_QUEUE_FULL); // loop while queue is full

  if (hiai_ret != HIAI_OK) { // check send data is failed
    HIAI_ENGINE_LOG(HIAI_ENGINE_RUN_ARGS_NOT_RIGHT, "Send data failed! error code: %d", hiai_ret);
  }
}

HIAI_IMPL_ENGINE_PROCESS("video_decode", VideoDecode, INPUT_SIZE) {
  av_log_set_level(AV_LOG_INFO); // set ffmpeg log level

  // verify video type
  if (!VerifyVideoType()) {
//    SendFinishedData(); // send the flag data when finished
    return HIAI_ERROR;
  }

  MultithreadHandleVideo(); // handle video from file or RTSP with multi-thread

//  SendFinishedData(); // send the flag data when finished

  return HIAI_OK;
}

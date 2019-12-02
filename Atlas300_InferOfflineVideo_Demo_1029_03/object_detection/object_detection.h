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
#ifndef OBJECT_DETECTION_OBJECT_DETECTION_H_
#define OBJECT_DETECTION_OBJECT_DETECTION_H_
#include <string>
#include <vector>
#include "hiaiengine/ai_model_manager.h"
#include "hiaiengine/ai_tensor.h"
#include "hiaiengine/ai_types.h"
#include "hiaiengine/api.h"
#include "hiaiengine/data_type.h"
#include "hiaiengine/data_type_reg.h"
#include "hiaiengine/engine.h"
#include "dvpp/idvppapi.h"
#include "dvpp/Vpc.h"
#include "thread_safe_queue.h"
#include "video_analysis_params.h"
#include "hiai_data_sp_son.h"
#include "bicyclecommon.h"
#include "stream_data.h"
#include "error_code.h"

#define INPUT_SIZE 2
#define OUTPUT_SIZE 1

namespace ascend {
namespace videoanalysis {
// yuv420sp image frame info
struct YuvImageFrameInfo {
  std::string channel_name;
  std::string channel_id;
};

unsigned int frame_id = 0;

/**
 * @brief check current is key frame, key frame: 1,6,11,16...
 * @param [in] frame_id: frame id
 * @return true: is key frame; false: is not key frame
 */
bool IsKeyFrame(uint32_t frame_id);

/**
 * @brief get frame id by channel id
 * @param [in] channel_id: channel id
 * @return frame id
 */
uint32_t GetFrameId(const std::string &channel_id);

/**
 * @brief call vpc to get yuv42sp image
 * @param [in] frame: image frame data
 * @param [in] hiai_data: used for transmit channel and frame info
 */
void CallVpcGetYuvImage(FRAME* frame, void* hiai_data);

/**
 * @brief add image data to queue by channel id
 * @param [in] video_image_para: the image data from video
 * @param [out] current_queue: the queue used for current channel
 */
void AddImage2Queue(const shared_ptr<VideoImageParaT> &video_image_para);
}
}

class ObjectDetectionInferenceEngine : public hiai::Engine {
 public:
  /**
   * @brief ObjectDetectionInferenceEngine constructor
   */
  ObjectDetectionInferenceEngine();

  /**
   * @brief ObjectDetectionInferenceEngine constructor
   */
  ~ObjectDetectionInferenceEngine();

  /**
   * @brief Engine init method.
   * @return HIAI_StatusT
   */
  HIAI_StatusT Init(const hiai::AIConfig &config,
                    const std::vector<hiai::AIModelDescription> &model_desc);

  /**
   * @ingroup hiaiengine
   * @brief HIAI_DEFINE_PROCESS : override Engine Process logic.
   * @[in]: define a input port, a output port
   * @return HIAI_StatusT
   */
HIAI_DEFINE_PROCESS(INPUT_SIZE, OUTPUT_SIZE)

 private:
  /**
   * @brief : image preprocess function.
   * @param [in] src_img: input image data.
   * @param [out] resized_img: resized image data.
   * @return HIAI_StatusT
   */
  HIAI_StatusT ImagePreProcess(const hiai::ImageData<u_int8_t> &src_img,
                               hiai::ImageData<u_int8_t> &resized_img);

  /**
   * @brief : convert video frame data to hfbc data and put results to queue
   * @param [in] video_image: the input video frame data
   * @return true: success; false: failed
   */
  bool ConvertVideoFrameToHfbc(
      const std::shared_ptr<VideoImageParaT> &video_image);

  /**
   * @brief get channel id(integer value)
   * @param [in] channel_id: the input channel id
   * @return channel id(integer value)
   */
  int GetIntChannelId(const std::string channel_id);

  /**
   * @brief convert hfbc data to yuv image from queue, and detect object
   */
  void ObjectDetectInference();

  void SendDeviceStreamParam();
  void SendDeviceStreamParam(ImageData<u_int8_t>& img);
  /**
   * @brief : handle input data have is_finished flag
   * @param [in] video_image: the input video frame data
   * @return HIAI_StatusT
   */
  HIAI_StatusT HandleFinishedData(
      const std::shared_ptr<VideoImageParaT> &video_image);

  // shared ptr to load ai model.
  std::shared_ptr<hiai::AIModelManager> ai_model_manager_;

  // dvpp vdec api for channel 1
  IDVPPAPI* dvpp_api_channel1_;
};

#endif /* OBJECT_DETECTION_OBJECT_DETECTION_H_ */

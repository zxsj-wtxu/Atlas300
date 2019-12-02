/**
 * ============================================================================
 *
 * Copyright (C) 2019, Huawei Technologies Co., Ltd. All Rights Reserved.
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
#ifndef SAMPLE_SSDDETECTION_H
#define SAMPLE_SSDDETECTION_H

#include "dvpp/idvppapi.h"
#include "hiaiengine/ai_model_manager.h"
#include "hiaiengine/engine.h"
#include "stream_data.h"
#include <utility>
#include "hiaiengine/multitype_queue.h"

#define DT_INPUT_SIZE 2
#define DT_OUTPUT_SIZE 1

class SSDDetection : public hiai::Engine {
public:
    SSDDetection():input_que_(DT_INPUT_SIZE) {}

    HIAI_StatusT Init(const hiai::AIConfig& config,
        const std::vector<hiai::AIModelDescription>& model_desc);

    HIAI_DEFINE_PROCESS(DT_INPUT_SIZE, DT_OUTPUT_SIZE)

private:
    uint32_t kBatchSize = 1;
    uint32_t kChannel = 0;
    uint32_t kWidth = 0;
    uint32_t kHeight = 0;
    uint32_t kAlignedWidth = 0;
    uint32_t kAlignedHeight = 0;
    uint32_t kInputSize = 0;
    std::shared_ptr<hiai::AIModelManager> modelManager;
    std::vector<std::shared_ptr<DeviceStreamData> > inputArgQueue;
    std::vector<std::shared_ptr<uint8_t> > inputDataBuffer;
    std::vector<std::shared_ptr<uint8_t> > outputDataBuffer;
    std::vector<std::shared_ptr<hiai::IAITensor> > inputTensorVec;
    std::vector<std::shared_ptr<hiai::IAITensor> > outputTensorVec;

    hiai::MultiTypeQueue input_que_;
//    HIAI_StatusT postProcessDetection();
    HIAI_StatusT postProcessDetection(int channel, shared_ptr<DeviceStreamData>* streamPtr, const float thresh);
};
#define MULTI_INPUT_ARG_PUSH(index) \
    std::shared_ptr<DeviceStreamData> _multi_input_##index; \
    input_que_.PushData(index, arg##index) \

#define MULTI_INPUT_ARG_PUSH_1 \
    MULTI_INPUT_ARG_PUSH(0); \

#define MULTI_INPUT_ARG_PUSH_2 \
    MULTI_INPUT_ARG_PUSH_1 \
    MULTI_INPUT_ARG_PUSH(1); \

#define MULTI_INPUT_ARG_PUSH_3 \
    MULTI_INPUT_ARG_PUSH_2 \
    MULTI_INPUT_ARG_PUSH(2); \

#define MULTI_INPUT_ARG_PUSH_4 \
    MULTI_INPUT_ARG_PUSH_3 \
    MULTI_INPUT_ARG_PUSH(3); \

#define MULTI_INPUT_ARG_PUSH_5 \
    MULTI_INPUT_ARG_PUSH_4 \
    MULTI_INPUT_ARG_PUSH(4); \

#define MULTI_INPUT_ARG_PUSH_6 \
    MULTI_INPUT_ARG_PUSH_5 \
    MULTI_INPUT_ARG_PUSH(5); \

#define MULTI_INPUT_ARG_PUSH_7 \
    MULTI_INPUT_ARG_PUSH_6 \
    MULTI_INPUT_ARG_PUSH(6); \

#define MULTI_INPUT_ARG_PUSH_8 \
    MULTI_INPUT_ARG_PUSH_7 \
    MULTI_INPUT_ARG_PUSH(7); \

#define MULTI_INPUT_ARG_PUSH_9 \
    MULTI_INPUT_ARG_PUSH_8 \
    MULTI_INPUT_ARG_PUSH(8); \

#define MULTI_INPUT_ARG_PUSH_10 \
    MULTI_INPUT_ARG_PUSH_9 \
    MULTI_INPUT_ARG_PUSH(9); \

#define MULTI_INPUT_ARG_PUSH_11 \
    MULTI_INPUT_ARG_PUSH_10 \
    MULTI_INPUT_ARG_PUSH(10); \

#define MULTI_INPUT_ARG_PUSH_12 \
    MULTI_INPUT_ARG_PUSH_11 \
    MULTI_INPUT_ARG_PUSH(11); \

#define MULTI_INPUT_ARG_PUSH_13 \
    MULTI_INPUT_ARG_PUSH_12 \
    MULTI_INPUT_ARG_PUSH(12); \

#define MULTI_INPUT_ARG_PUSH_14 \
    MULTI_INPUT_ARG_PUSH_13 \
    MULTI_INPUT_ARG_PUSH(13); \

#define MULTI_INPUT_ARG_PUSH_15 \
    MULTI_INPUT_ARG_PUSH_14 \
    MULTI_INPUT_ARG_PUSH(14); \

#define MULTI_INPUT_ARG_PUSH_16 \
    MULTI_INPUT_ARG_PUSH_15 \
    MULTI_INPUT_ARG_PUSH(15); \

#define MULTI_INPUT_POP_QUEUE_1 \
    inputArgQueue.push_back(std::move(_multi_input_0));

#define MULTI_INPUT_POP_QUEUE_2 \
    MULTI_INPUT_POP_QUEUE_1 \
    inputArgQueue.push_back(std::move(_multi_input_1));

#define MULTI_INPUT_POP_QUEUE_3 \
    MULTI_INPUT_POP_QUEUE_2 \
    inputArgQueue.push_back(std::move(_multi_input_2));

#define MULTI_INPUT_POP_QUEUE_4 \
    MULTI_INPUT_POP_QUEUE_3 \
    inputArgQueue.push_back(std::move(_multi_input_3));

#define MULTI_INPUT_POP_QUEUE_5 \
    MULTI_INPUT_POP_QUEUE_4 \
    inputArgQueue.push_back(std::move(_multi_input_4));

#define MULTI_INPUT_POP_QUEUE_6 \
    MULTI_INPUT_POP_QUEUE_5 \
    inputArgQueue.push_back(std::move(_multi_input_5));

#define MULTI_INPUT_POP_QUEUE_7 \
    MULTI_INPUT_POP_QUEUE_6 \
    inputArgQueue.push_back(std::move(_multi_input_6));

#define MULTI_INPUT_POP_QUEUE_8 \
    MULTI_INPUT_POP_QUEUE_7 \
    inputArgQueue.push_back(std::move(_multi_input_7));

#define MULTI_INPUT_POP_QUEUE_9 \
    MULTI_INPUT_POP_QUEUE_8 \
    inputArgQueue.push_back(std::move(_multi_input_8));

#define MULTI_INPUT_POP_QUEUE_10 \
    MULTI_INPUT_POP_QUEUE_9 \
    inputArgQueue.push_back(std::move(_multi_input_9));

#define MULTI_INPUT_POP_QUEUE_11 \
    MULTI_INPUT_POP_QUEUE_10 \
    inputArgQueue.push_back(std::move(_multi_input_10));

#define MULTI_INPUT_POP_QUEUE_12 \
    MULTI_INPUT_POP_QUEUE_11 \
    inputArgQueue.push_back(std::move(_multi_input_11));

#define MULTI_INPUT_POP_QUEUE_13 \
    MULTI_INPUT_POP_QUEUE_12 \
    inputArgQueue.push_back(std::move(_multi_input_12));

#define MULTI_INPUT_POP_QUEUE_14 \
    MULTI_INPUT_POP_QUEUE_13 \
    inputArgQueue.push_back(std::move(_multi_input_13));

#define MULTI_INPUT_POP_QUEUE_15 \
    MULTI_INPUT_POP_QUEUE_14 \
    inputArgQueue.push_back(std::move(_multi_input_14));

#define MULTI_INPUT_POP_QUEUE_16 \
    MULTI_INPUT_POP_QUEUE_15 \
    inputArgQueue.push_back(std::move(_multi_input_15));

#define MULTI_INPUT_ARG_POP_1 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)


#define MULTI_INPUT_ARG_POP_2 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_3 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_4 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, _multi_input_3)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_6 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, _multi_input_4)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_6 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2,\
    _multi_input_3, , _multi_input_4, _multi_input_5)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_7 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, , _multi_input_4, _multi_input_5, _multi_input_6)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_8 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, , _multi_input_4, _multi_input_5, _multi_input_6, \
    _multi_input_7)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_9 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, , _multi_input_4, _multi_input_5, _multi_input_6, \
    _multi_input_7, _multi_input_8)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_10 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, , _multi_input_4, _multi_input_5, _multi_input_6, \
    _multi_input_7, _multi_input_8, _multi_input_9)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_11 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, , _multi_input_4, _multi_input_5, _multi_input_6, \
    _multi_input_7, _multi_input_8, _multi_input_9, _multi_input_10)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_12 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, , _multi_input_4, _multi_input_5, _multi_input_6, \
    _multi_input_7, _multi_input_8, _multi_input_9, _multi_input_10, _multi_input_11)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_13 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, , _multi_input_4, _multi_input_5, _multi_input_6, \
    _multi_input_7, _multi_input_8, _multi_input_9, _multi_input_10, _multi_input_11, \
    _multi_input_12)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_14 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, , _multi_input_4, _multi_input_5, _multi_input_6, \
    _multi_input_7, _multi_input_8, _multi_input_9, _multi_input_10, _multi_input_11, \
    _multi_input_12, _multi_input_13)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_15 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, , _multi_input_4, _multi_input_5, _multi_input_6 ,\
    _multi_input_7, _multi_input_8, _multi_input_9, _multi_input_10, _multi_input_11, \
    _multi_input_12, _multi_input_13, _multi_input_14)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)
#define MULTI_INPUT_ARG_POP_16 \
    do{ \
        if (!input_que_.PopAllData(_multi_input_0, _multi_input_1, _multi_input_2, \
    _multi_input_3, _multi_input_4, _multi_input_5, _multi_input_6 ,\
    _multi_input_7, _multi_input_8, _multi_input_9, _multi_input_10, _multi_input_11, \
    _multi_input_12, _multi_input_13, _multi_input_14, _multi_input_15)) \
        { \
            return HIAI_ERROR; \
        } \
    }while(0)


#define DEFINE_MULTI_INPUT_ARGS_PUSH(index) MULTI_INPUT_ARG_PUSH_##index

#define DEFINE_MULTI_INPUT_ARGS_POP(index) MULTI_INPUT_ARG_POP_##index

#define DEFINE_MULTI_INPUT_POP_QUEUE(index) MULTI_INPUT_POP_QUEUE_##index
#endif //ATLASFACEDEMO_FACEDETECTION_H

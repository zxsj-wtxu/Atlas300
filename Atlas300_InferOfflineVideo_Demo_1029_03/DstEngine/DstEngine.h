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

#ifndef OUTPUT_ENGINE_H
#define OUTPUT_ENGINE_H

#include <fstream>
#include "hiaiengine/engine.h"
#include "Common.h"
#include "stream_data.h"
#include "AppCommon.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "bicyclecommon.h"
#include "hiaiengine/multitype_queue.h"
#define DST_INPUT_SIZE 1
#define DST_OUTPUT_SIZE 1

class DstEngine : public hiai::Engine {
public:
    DstEngine();
    HIAI_StatusT Init(const hiai::AIConfig& config, const std::vector<hiai::AIModelDescription>& model_desc);

    HIAI_DEFINE_PROCESS(DST_INPUT_SIZE, DST_OUTPUT_SIZE)
private:
    // process the classify result
    HIAI_StatusT ProcessResult(int output_port, const std::shared_ptr<DeviceStreamData>& inputArg);
    HIAI_StatusT ProcessResult(const std::string& resultFileTxt, const std::shared_ptr<DeviceStreamData>& inputArg);

    hiai::MultiTypeQueue input_que_;
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

#endif

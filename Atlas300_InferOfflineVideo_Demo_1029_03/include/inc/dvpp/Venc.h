/**
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef DVPP_VENC_H
#define DVPP_VENC_H

#include <stdint.h>
#include "ExportMacro.h"

struct VencOutMsg {
    void* outputData;
    uint32_t outputDataSize;
    uint32_t timeStamp; // for stream header, whether a idependent time stamp is needed
};

typedef void (*VencOutMsgCallBack)(struct VencOutMsg* vencOutMsg, void* userData);

struct VencConfig {
    uint32_t width;
    uint32_t height;
    uint32_t codingType; // 0: H265 MP, 1: H264 BP, 2: H264 MP, 3: H264 HP
    uint32_t yuvStoreType; // 0: nv12(YUV420SP), 1: nv21(YVU420SP)
    uint32_t keyFrameInterval;
    VencOutMsgCallBack vencOutMsgCallBack;
    void* userData; // The caller can pass in the information
                    // Dvpp will not use or change it, will be returned by callback function
};

struct VencInMsg {
    void* inputData;
    uint32_t inputDataSize;
    uint32_t keyFrameInterval;
    uint32_t forceIFrame; // 0: not force, 1: force
    uint32_t eos; // 0: not eos, 1: eos
};

DVPP_EXPORT int32_t CreateVenc(struct VencConfig* vencConfig);
DVPP_EXPORT int32_t RunVenc(int32_t vencHandle, struct VencInMsg* vencInMsg);
DVPP_EXPORT int32_t DestroyVenc(int32_t vencHandle);

#endif // DVPP_VENC_H
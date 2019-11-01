/**
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef DVPP_PNG_H
#define DVPP_PNG_H

#include <cstdint>
#include <memory>
#include "Base.h"

// input
struct PngInputInfoAPI {
    void* inputData; 
    uint64_t inputSize;
    void* address; // Memory address
    uint64_t size; // Memory size
    uint32_t transformFlag;
};

// output
struct PngOutputInfoAPI {
    std::shared_ptr<DvppBuffer> pngBuffer;
    uint32_t format; // output format,2:RGB,6:RGBA
    uint32_t width; // output width;
    uint32_t high; // output high
    uint32_t widthAlign; // Memory alignment width
    uint32_t highAlign; // Memory alignment high
};

#endif // DVPP_PNG_H
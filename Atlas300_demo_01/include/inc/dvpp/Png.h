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
    void* inputData; // 输入数据
    uint64_t inputSize; // 输入大小
    void* address; // 内存地址
    uint64_t size; // 内存大小
    uint32_t transformFlag; // 转换标志
};

// output
struct PngOutputInfoAPI {
    std::shared_ptr<DvppBuffer> pngBuffer; // 输出数据
    uint32_t format; // 输出格式，2：RGB，6：RGBA
    uint32_t width; // 输出宽度
    uint32_t high; // 输出高度
    uint32_t widthAlign; // 内存对齐宽度
    uint32_t highAlign; // 内存对齐高度
};

#endif // DVPP_PNG_H
/**
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef DVPP_BASE_H
#define DVPP_BASE_H

#include <cstddef> // size_t

struct DvppBuffer {
    void* address; // 内存地址
    size_t size; // 内存大小
    void* dataAddress; // 数据地址
    size_t dataSize; // 数据大小
};

#endif // DVPP_BASE_H
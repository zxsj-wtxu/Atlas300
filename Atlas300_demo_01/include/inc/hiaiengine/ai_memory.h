/** 
* @file ai_memory.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/*lint -e* */
#ifndef HIAI_MEMORY_H_
#define HIAI_MEMORY_H_
#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <memory>
#include "hiaiengine/status.h"

namespace hiai {
    constexpr uint32_t MALLOC_DEFAULT_TIME_OUT = 100000;
    constexpr uint32_t HIAI_ALIBUF_MAP_VA32BIT = 0x200;

    typedef enum {
        HIAI_MEMORY_ATTR_NONE = 0,
        // 框架自行释放DMalloc的内存 (0x1 << 0保留，防止和之前的大页标志重复) 
        HIAI_MEMORY_ATTR_ATUO_FREE = (0x1 << 1),
        // 需要手动调用DFree释放内存
        HIAI_MEMORY_ATTR_MANUAL_FREE = (0x1 << 2),
        IAI_MEMORY_ATTR_MAX
    } HIAI_MEMORY_ATTR;

    class HIAIMemory {
      public:
        /**
        * @ingroup HIAI
        * @brief HIAI_DMalloc  : 从内存池获取相应大小的内存块,
        *                        如果内存池没有初始化，根据申请的内存块大小初始化内存池;
        *                        如果后续内存块的大小要大于初始的内存块大小， 重新申请
        * @param [in] data_size: 分配内存的大小, 单位为bytes
        * @param [in] dataBuffer: 分配内存的大小
        * @param [in] time_out : 时延
        * @param [in] flag : 内存属性标志
        * @return HIAI_OK成功，或者其他错误码
        */
        HIAI_LIB_VISIBILITY static HIAI_StatusT HIAI_DMalloc(
            const uint32_t dataSize, void*& dataBuffer,
            const uint32_t& timeOut = MALLOC_DEFAULT_TIME_OUT,
            uint32_t flag = HIAI_MEMORY_ATTR_ATUO_FREE);

        /**
        * @ingroup HIAI
        * @brief HIAI_DFree: 释放从内存池中获取的内存
        * @param [in] dataBuffer: 内存指针
        * @return HIAI_OK成功，或者其他错误码
        */
        HIAI_LIB_VISIBILITY static HIAI_StatusT HIAI_DFree(void* dataBuffer);

        /**
        * @ingroup HIAI
        * @brief IsDMalloc: 判断内存是否是由DMalloc申请
        * @param [in] dataBuffer: 内存指针
        * @return bool, true: 内存为DMalloc申请, false: 内存非DMalloc申请
        */
        HIAI_LIB_VISIBILITY static bool IsDMalloc(const void* dataBuffer,
            const uint32_t& dataSize);

        /**
        * @ingroup HIAI
        * @brief HIAI_DVPP_DMalloc : 获取相应大小的4G地址范围内的内存块,
        *                        优先使用大页内存，大页申请不到申请小页内存;
        * @param [in] data_size: 分配内存的大小, 单位为bytes
        * @param [in] dataBuffer: 分配内存的大小
        * @return HIAI_OK成功，或者其他错误码
        */
        HIAI_LIB_DEVICE_VISIBILITY static HIAI_StatusT HIAI_DVPP_DMalloc(
            const uint32_t dataSize,    void*& dataBuffer);

        /**
        * @ingroup HIAI
        * @brief HIAI_DVPP_DFree: 释放通过HIAI_DVPP_DMalloc接口获取的内存
        * @param [in] dataBuffer: 内存指针
        * @return HIAI_OK成功，或者其他错误码
        */
        HIAI_LIB_DEVICE_VISIBILITY static HIAI_StatusT HIAI_DVPP_DFree(void* dataBuffer);

        HIAI_LIB_INTERNAL HIAIMemory() = default;
        HIAI_LIB_INTERNAL ~HIAIMemory() = default;
      private:
        HIAI_LIB_INTERNAL HIAIMemory(const HIAIMemory&) = delete;
        HIAI_LIB_INTERNAL HIAIMemory(HIAIMemory&&) = delete;
        HIAI_LIB_INTERNAL HIAIMemory& operator=(
            const HIAIMemory&) = delete;
        HIAI_LIB_INTERNAL HIAIMemory& operator=(
            HIAIMemory&&) = delete;
    };
}
#endif  // HIAI_MEMORY_H_

/* Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the Apache License Version 2.0.You may not use this file except in compliance with the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * Apache License for more details at
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef FMK_TYPES_H__
#define FMK_TYPES_H__

namespace domi {

/**
* @brief 模型数据结构定义，由调用方传入
*/
struct ModelData
{
    void *model_data = NULL;    /**< 模型二进制数据首地址 */
    uint32_t model_len;         /**< 模型二进制数据长度 */
    int32_t priority;           /**< 模型优先级 */
    std::string key;            /**< 模型解密密钥的路径，如果为非加密模型，则传入空字符串 */
};

struct TaskGenInfo {
    uint32_t mode;                      // 模式（是否融合等）
    uint32_t size;                      // task序列化数据size
    uint8_t *serializedTasks;           // task序列化数据
    uint64_t dataBase;                  // task生成时的数据基址
    uint64_t weightBase;                // task生成时的权值基址
};

}// namespace domi

#endif // FMK_TYPES_H__

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
#ifndef DOMI_OMG_PARSER_WEIGHTS_PARSER_H_
#define DOMI_OMG_PARSER_WEIGHTS_PARSER_H_

#include "omg/graph/graph.h"

namespace domi
{
    /**
     * @ingroup domi_omg
     * @brief 权重信息解析器
     * 
     */
    class WeightsParser
    {
    public:

        /**
         * @ingroup domi_omg
         * @brief 构造函数
         */
        WeightsParser()
        {
        }

        /**
         * @ingroup domi_omg
         * @brief 析构函数
         */
        virtual ~WeightsParser()
        {
        }

        /**
         * @ingroup domi_omg
         * @brief 解析权重数据
         * @param [in] file 训练后权重文件的路径
         * @param [in|out]  graph 保存解析后权重信息的图
         * @return SUCCESS  解析成功
         * @return 其他          解析失败
         */
        virtual Status Parse(const char* file, Graph* graph) = 0;
    };

}  // namespace domi

#endif  // DOMI_OMG_PARSER_WEIGHTS_PARSER_H_

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
#ifndef _DOMI_OMG_PARSER_OP_PARSER_H_
#define _DOMI_OMG_PARSER_OP_PARSER_H_


#include <google/protobuf/text_format.h>
#include "common/types.h"
#include "omg/omg_types.h"
#include "proto/om.pb.h"

using google::protobuf::Message;

namespace domi
{
    /**
     * @ingroup domi_omg
     * @brief 用于解析算子信息，Protobuf或JSON格式信息的解析各对应一组ParseParams/ParseWeights函数。
     * 
     */
    class OpParser
    {
    public:

        /**
         * @ingroup domi_omg
         * @brief 析构函数
         */
        virtual ~OpParser()
        {
        }

        /**
         * @ingroup domi_omg
         * @brief 解析算子参数
         * @param [in] op_src 待解析的参数数据
         * @param [out] graph 解析后的参数数据
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         */
        virtual Status ParseParams(const Message* op_src, OpDef* op_dest) = 0;

        /**
         * @ingroup domi_omg
         * @brief 解析算子权重信息
         * @param [in] op_src 待解析的权重数据
         * @param [out] op_dest 解析后的权重数据
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         */
        virtual Status ParseWeights(const Message* op_src, OpDef* op_dest) = 0;

        /**
         * @ingroup domi_omg
         * @brief 根据算子中的参数，获取format信息
         * @param [in] op_src 待解析的参数数据
         * @param [out] format 输出解析出的format
         * @return SUCCESS 获取成功
         * @return FAILED 获取失败
         */
        virtual Status GetFormat(const Message* op_src, domiTensorFormat_t& format)
        {
            (void)op_src;
            // 设置为CC_TENSOR_RESERVED，表示该op不提供format的值
            format = DOMI_TENSOR_RESERVED;
            return domi::SUCCESS;
        }
    };
}
// end namespace domi

#endif //_DOMI_OMG_PARSER_OP_PARSER_H_

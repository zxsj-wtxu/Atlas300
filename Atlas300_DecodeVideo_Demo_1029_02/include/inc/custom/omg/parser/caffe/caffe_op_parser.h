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
#ifndef _DOMI_OMG_PARSER_CAFFE_CAFFE_OP_PARSER_H_
#define _DOMI_OMG_PARSER_CAFFE_CAFFE_OP_PARSER_H_

#include "omg/parser/op_parser.h"
#include "proto/caffe/caffe.pb.h"

using namespace caffe;

namespace domi
{
    /**
     * @ingroup domi_omg
     * @brief 用于解析Caffe算子信息
     */
    class CaffeOpParser: public OpParser
    {
    public:
        Status ParseParams(const Message* op_src, OpDef* op_dest);

        /**
         * @ingroup domi_omg
         * @brief 解析算子权重信息
         * @param [in] op_src 待解析的权重数据
         * @param [out] graph 解析后的权重数据
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         * @author 
         */
        Status ParseWeights(const Message* op_src, OpDef* op_dest);

    protected:
        /**
         * @ingroup domi_omg
         * @brief 将BlobProto转换为WeightDef
         * @param [in] proto 待转换的Caffe权重信息
         * @param [out] weight 转换后的权重信息
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         */
        static Status ConvertWeight(const BlobProto& proto, WeightDef* weight);

        /**
         * @ingroup domi_omg
         * @brief 将BlobProto转换为ShapeDef
         * @param [in] proto 保存转换前的Shape信息
         * @param [out] shape 保存转换的Shape信息
         */
        static void ConvertShape(const BlobProto& proto, ShapeDef* shape);
    };

}
// end namespace domi

#endif //_DOMI_OMG_PARSER_CAFFE_CAFFE_OP_PARSER_H_

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
#ifndef _DOMI_OMG_PARSER_TENSORFLOW_FUSION_OP_PARSER_H_
#define _DOMI_OMG_PARSER_TENSORFLOW_FUSION_OP_PARSER_H_

#include "omg/parser/op_parser.h"
#include "proto/tensorflow/graph.pb.h"
#include "proto/tensorflow/node_def.pb.h"
#include "omg/parser/tensorflow/tensorflow_util.h"
#include "omg/parser/tensorflow/tensorflow_op_parser.h"
#include "common/op/attr_value_util.h"

using namespace google::protobuf;
using namespace domi::tensorflow;
using namespace std;

namespace domi
{
    /**
     * @ingroup domi_omg
     * @brief 用于解析TensorFlow算子信息
     */
    class TensorFlowFusionOpParser: public TensorFlowOpParser
    {
    public:

        /**
         * @ingroup domi_omg
         * @brief 解析算子参数
         * @param [in] v_input_const 待解析的算子参数
         * @param [out] op_dest 解析后的模型数据
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         * 
         */
        virtual Status ParseParams(const vector<const NodeDef*>& v_input_const, OpDef* op_dest);

        /**
         * @ingroup domi_omg
         * @brief 解析算子参数
         * @param [in] op_src 待解析的参数数据
         * @param [out] graph 解析后的参数数据
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         */
        Status ParseParams(const Message* op_src, OpDef* op_dest) final;
        /**
        * @ingroup domi_omg
        * @brief 解析权值信息
        * @param [in] v_input_const 待解析的权值数据
        * @param [out] op_dest 保存解析后权值数据
        * @return SUCCESS 解析成功
        * @return FAILED 解析失败
        * 
        */
        Status ParseWeights(const vector<const NodeDef*>& v_input_const, OpDef* op_dest);

    protected:

        /**
         * @ingroup domi_omg
         * @brief 从const op中解析参数
         * @param [in] op_src 待解析的模型数据
         * @param [out] op_dest 解析后的模型数据
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         * 
         */
        // template <class T>
        Status ParseParamFromConst(const NodeDef* input_const , int32_t & param);

        Status ParseParamFromConst(const NodeDef* nodeDef , int32_t & param ,int index);

        Status ParseParamFromConst(const NodeDef* input_const , float & param);

        Status ParseParamFromConst(const NodeDef* nodeDef , float & param ,int index);

        Status GetTensorFromNode(const NodeDef* nodeDef , TensorProto& tensor);
    };

}
// end namespace domi

#endif //_DOMI_OMG_PARSER_TENSORFLOW_FUSION_OP_PARSER_H_

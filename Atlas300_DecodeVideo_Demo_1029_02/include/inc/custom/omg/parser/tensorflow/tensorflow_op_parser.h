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
#ifndef _DOMI_OMG_PARSER_TENSORFLOW_OP_PARSER_H_
#define _DOMI_OMG_PARSER_TENSORFLOW_OP_PARSER_H_

#include "omg/parser/op_parser.h"
#include "proto/tensorflow/graph.pb.h"
#include "proto/tensorflow/node_def.pb.h"
#include "omg/parser/tensorflow/tensorflow_util.h"
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
    class TensorFlowOpParser: public OpParser
    {
    public:
        /**
        * @ingroup domi_omg
        * @brief 解析权值信息
        * @param [in] v_input_const 待解析的权值数据
        * @param [out] op_dest 保存解析后权值数据
        * @return SUCCESS 解析成功
        * @return FAILED 解析失败
        * 
        */
        virtual Status ParseWeights(const vector<const NodeDef*>& v_input_const, OpDef* op_dest);

        virtual Status ParseWeights(const vector<const NodeDef*>& input_nodes,
                OpDef* op_dest, vector<OpDef*>& input_ops);

        /**
         * @ingroup domi_omg
         * @brief 解析模型文件信息
         * @param [in] op_src 待解析的模型数据
         * @param [out] op_dest 解析后的模型数据
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         * 
         */
        Status ParseParams(const Message* op_src, OpDef* op_dest);

        /**
         * @ingroup domi_omg
         * @brief 解析算子权重信息
         * @param [in] op_src 待解析的权重数据
         * @param [out] op_dest 解析后的权重数据
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         * 
         */
        Status ParseWeights(const Message* op_src, OpDef* op_dest) final;

        /**
         * @ingroup domi_omg
         * @brief 解析数据类型
         * @param [in] node_src 待解析的Node
         * @param [in] attr_src 待解析的Attribute
         * @param [out] data_type 解析后的数据类型
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         * 
         */
        Status ParseDataType(const NodeDef *node_src, const string &attr_src,
                    DataType &data_type);

        /**
         * @ingroup domi_omg
         * @brief 解析数据类型
         * @param [in] node_src 待解析的Node
         * @param [in] attr_src 待解析的Attribute
         * @param [out] op_dest 解析后的Op
         * @param [in] attr_dest 解析后的attr_dest
         * @return SUCCESS 解析成功
         * @return FAILED 解析失败
         * 
         */
        Status ParseDataType(const NodeDef *node_src, const string &attr_src,
            OpDef *op_dest, const string &attr_dest);

    protected:
        /**
        * @ingroup domi_omg
        * @brief 将NodeDef转换为WeightDef
        * @param [in] nodeDef 待转换的TensorFlow权重信息
        * @param [out] weight 转换后的权重信息
        * @return SUCCESS 解析成功
        * @return FAILED 解析失败
        * 
        */
        Status ConvertWeight(const NodeDef* nodeDef, WeightDef* weight);

        /**
        * @ingroup domi_omg
        * @brief 将NodeDef中的tensor值取出放入weightdef
        * @param [in] tensor
        * @param [out] weight 转换后的权重信息
        * @return SUCCESS 解析成功
        * @return FAILED 解析失败
        */
        Status SetWeightData(const TensorProto& tensor, WeightDef* weight);

        /**
         * @ingroup domi_omg
         * @brief Return domiDataType_t according to DataType.
         * @param [in] tf_type data type of TensorFlow
         * @return data type of domi
        */
        static domiDataType_t TensorflowDataTypeToDomi(DataType tf_type);
    };

}
// end namespace domi

#endif //_DOMI_OMG_PARSER_TENSORFLOW_OP_PARSER_H_

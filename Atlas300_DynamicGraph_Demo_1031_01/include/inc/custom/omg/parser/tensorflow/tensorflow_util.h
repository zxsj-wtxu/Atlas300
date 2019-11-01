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
#ifndef _DOMI_OMG_PARSER_TENSORFLOW_UTIL_H_
#define _DOMI_OMG_PARSER_TENSORFLOW_UTIL_H_

#include "common/types.h"
#include "omg/omg_types.h"
#include "proto/tensorflow/graph.pb.h"
#include "common/op_def/operator.h"
#include "omg/graph/graph.h"

using namespace domi::tensorflow;
using namespace std;

namespace domi
{
    /*各种数据类型所占字节大小 */
    const static int32_t SIZE_FLOAT = sizeof(float);
    const static int32_t SIZE_HALF_FLOAT = sizeof(float) / 2;
    const static int32_t SIZE_INT8 = sizeof(int8_t);
    const static int32_t SIZE_INT32 = sizeof(int32_t);
    const static int32_t SIZE_INT64 = sizeof(int64_t);
    const static int32_t SIZE_UINT8 = sizeof(uint8_t);
    const static int32_t SIZE_BOOL = sizeof(bool);

    const static set<string> tensorflow_frameworkop_skipped_op_set =
    {
        "_Arg"
    };

    typedef ::google::protobuf::Map<string, AttrValue> AttrValueMap;
    /*Const节点的输出op,const数据的处理方式*/
    static map<string, domiTensorFormat_t> CONST_OUTPUT_OP_AND_FORMAT_MAP =
    {
        {"MatMul" , DOMI_TENSOR_FRACTAL_Z},
        {"Gather" , DOMI_TENSOR_NC1HWC0},
    };

    /* tensorflow中的data_type和OM中的对应关系 */
    static map<int32_t, domiDataType_t> CONST_OP_DATA_TYPE_MAP =
    {
        {tensorflow::DT_FLOAT, DOMI_DATA_FLOAT},
        {tensorflow::DT_BFLOAT16, DOMI_DATA_HALF},
        {tensorflow::DT_INT8, DOMI_DATA_INT8},
        {tensorflow::DT_INT32, DOMI_DATA_INT32},
        {tensorflow::DT_INT64, DOMI_DATA_INT64},
        {tensorflow::DT_UINT8, DOMI_DATA_UINT8},
        {tensorflow::DT_BOOL, DOMI_DATA_BOOL},
    };

    /* tensorflow中的data_type和OM中的对应关系 */
    static map<int32_t, int32_t> TENSORFLOW_DATA_TYPE_MAP =
    {
        {DOMI_DATA_FLOAT, tensorflow::DT_FLOAT},
        {DOMI_DATA_HALF, tensorflow::DT_BFLOAT16},
        {DOMI_DATA_INT8, tensorflow::DT_INT8},
        {DOMI_DATA_INT32, tensorflow::DT_INT32},
        {DOMI_DATA_INT64, tensorflow::DT_INT64},
        {DOMI_DATA_UINT8, tensorflow::DT_UINT8},
        {DOMI_DATA_BOOL, tensorflow::DT_BOOL},
    };

    /* OM中data_type和所占字节大小的对应关系 */
    static map<domiDataType_t, int32_t> CONST_OP_DATA_TYPE_SIZE_MAP =
    {
        {DOMI_DATA_FLOAT, SIZE_FLOAT},
        {DOMI_DATA_HALF, SIZE_HALF_FLOAT},
        {DOMI_DATA_INT8, SIZE_INT8},
        {DOMI_DATA_INT32, SIZE_INT32},
        {DOMI_DATA_INT64, SIZE_INT64},
        {DOMI_DATA_UINT8, SIZE_UINT8},
        {DOMI_DATA_BOOL, SIZE_BOOL},
    };

    /***************************TensorFlow属性类型，常量定义*******************************************/
    /*
    bytes s = 2;                 // "string"
    int64 i = 3;                 // "int"
    float f = 4;                 // "float"
    bool b = 5;                  // "bool"
    DataType type = 6;           // "type"
    TensorShapeProto shape = 7;  // "shape"
    TensorProto tensor = 8;      // "tensor"
    ListValue list = 1;          // any "list(...)"
    NameAttrList func = 10;
    string placeholder = 9;
    */
    static const string TENSORFLOW_ATTR_TYPE_STRING = "string";
    static const string TENSORFLOW_ATTR_TYPE_INT = "int";
    static const string TENSORFLOW_ATTR_TYPE_FLOAT = "float";
    static const string TENSORFLOW_ATTR_TYPE_BOOL = "bool";
    static const string TENSORFLOW_ATTR_TYPE_TYPE = "type";
    static const string TENSORFLOW_ATTR_TYPE_SHAPE = "shape";
    static const string TENSORFLOW_ATTR_TYPE_TENSOR = "tensor";
    static const string TENSORFLOW_ATTR_TYPE_FUNC = "func";

    static const string TENSORFLOW_ATTR_LIST_TYPE_STRING = "list(string)";
    static const string TENSORFLOW_ATTR_LIST_TYPE_INT = "list(int)";
    static const string TENSORFLOW_ATTR_LIST_TYPE_FLOAT = "list(float)";
    static const string TENSORFLOW_ATTR_LIST_TYPE_BOOL = "list(bool)";
    static const string TENSORFLOW_ATTR_LIST_TYPE_TYPE = "list(type)";
    static const string TENSORFLOW_ATTR_LIST_TYPE_SHAPE = "list(shape)";
    static const string TENSORFLOW_ATTR_LIST_TYPE_TENSOR = "list(tensor)";
    static const string TENSORFLOW_ATTR_LIST_TYPE_FUNC = "list(func)";


    /***************************常量定义*******************************************/
    static const string TENSORFLOW_ATTR_OUTPUT_OP = "output_op";

    static const string TENSORFLOW_ATTR_T = "T";
    static const string TENSORFLOW_ATTR_N = "N";
    static const string TENSORFLOW_ATTR_DATA_FORMAT = "data_format";
    static const string TENSORFLOW_ATTR_PADDING = "padding";
    static const string TENSORFLOW_ATTR_KSIZE = "ksize";
    static const string TENSORFLOW_ATTR_STRIDES = "strides";
    static const string TENSORFLOW_ATTR_DILATIONS = "dilations";
    static const string TENSORFLOW_ATTR_DTYPE = "dtype";
    static const string TENSORFLOW_ATTR_VALUE = "value";
    static const string TENSORFLOW_ATTR_TRANSINPUT = "transpose_a";
    static const string TENSORFLOW_ATTR_TRANSWEIGHT = "transpose_b";
    static const string TENSORFLOW_ATTR_SHAPE = "shape";
    static const string TENSORFLOW_ATTR_TIDX = "Tidx";
    static const string TENSORFLOW_ATTR_TPADDINGS = "Tpaddings";
    static const string TENSORFLOW_ATTR_TMULTIPLES = "Tmultiples";
    static const string TENSORFLOW_ATTR_TINDICES = "Tindices";
    static const string TENSORFLOW_ATTR_TPARAMS = "Tparams";
    static const string TENSORFLOW_ATTR_DSTT = "DstT";
    static const string TENSORFLOW_ATTR_SRCT = "SrcT";
    static const string TENSORFLOW_ATTR_PERM = "perm";
    static const string TENSORFLOW_ATTR_INDEX = "Index";
    static const string TENSORFLOW_ATTR_TSHAPE = "Tshape";
    static const string TENSORFLOW_ATTR_AXIS = "Axis";
    static const string TENSORFLOW_ATTR_BIAS = "bias";
    static const string TENSORFLOW_ATTR_DEPTH_RADIUS = "depth_radius";
    static const string TENSORFLOW_ATTR_ALPHA = "alpha";
    static const string TENSORFLOW_ATTR_BETA = "beta";
    static const string TENSORFLOW_ATTR_MODE = "mode";


    //op:Const
    static const string TENSORFLOWF_NODE_OP_CONST = "Const";
    static const string TENSORFLOWF_NODE_OP_IDENTITY = "Identity";
    static const string TENSORFLOWF_NODE_OP_SWITCH = "Switch";
    static const string TENSORFLOWF_NODE_OP_PLACEHOLDER = "Placeholder";
    static const string TENSORFLOWF_NODE_OP_ADDN = "AddN";
    static const string TENSORFLOWF_NODE_OP_MATMUL = "MatMul";
    static const string TENSORFLOWF_NODE_OP_RELU = "Relu";
    static const string TENSORFLOWF_NODE_OP_SHAPE = "Shape";
    static const string TENSORFLOWF_NODE_OP_TRANSPOSE = "Transpose";
    static const string TENSORFLOWF_NODE_OP_MERGE = "Merge";

    //data_format
    static const string TENSORFLOWF_TENSOR_NCHW = "NCHW";
    static const string TENSORFLOWF_TENSOR_NHWC = "NHWC";

    //padding
    static const string TENSORFLOWF_OP_PADDING_VALID = "VALID";
    static const string TENSORFLOWF_OP_PADDING_SAME = "SAME";

    //normal input size
    static const uint32_t TENSORFLOW_NORMAL_INPUT_SIZE_MATMUL = 2;
    static const uint32_t TENSORFLOW_NORMAL_INPUT_SIZE_RESHAPE = 1;
    static const uint32_t TENSORFLOW_NORMAL_INPUT_SIZE_POOL = 1;

    //normal weight size
    static const uint32_t TENSORFLOW_NORMAL_WEIGHT_SIZE_MATMUL = 1;
    static const uint32_t TENSORFLOW_NORMAL_WEIGHT_SIZE_RESHAPE = 1;

    //input or output
    static const uint32_t TENSORFLOW_NORMAL_INPUT_TENSOR_FLAG = 1;
    static const uint32_t TENSORFLOW_NORMAL_OUTPUT_TENSOR_FLAG = 2;

    class TensorFlowUtil
    {
    public:
        /**
        * @ingroup domi_omg
        * @brief NodeDef中查找对应的AttrValue
        * @param [in] nodeDef 需要查找的Nodedef对象
        * @param [in] attr_name 属性name
        * @param [out] attr_value 属性Value对象
        * @return true 属性存在
        * @return false 属性不存在
        *
        */
        static bool FindAttrValue(const tensorflow::NodeDef* nodeDef, const string attr_name, tensorflow::AttrValue& attr_value);

        /**
        * @ingroup domi_omg
        * @brief 检查AttrValue属性的实际类型和期望类型，int、float、list(int)、list(bool)等
        * @param [in] attr_value 需要检查的AttrValue
        * @param [in] type 期望的属性类型
        * @return SUCCESS 成功
        * @return FAILED 失败
        *
        */
        static Status CheckAttrHasType(const AttrValue& attr_value, string type);

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
        static Status ParseDataType(const NodeDef *node_src, const string &attr_src, DataType &data_type);

        /**
         * @ingroup domi_omg
         * @brief 解析数据类型
         * @param [in] attr_value 待转换的NodeDef中Attr
         * @param [out] op 解析后的信息存储在父类的属性中
         * @return SUCCESS 转换成功
         * @return FAILED 转换失败
         *
         */
        static Status TransTensorDescriptor(const AttrValue attr_value, Operator *op, const uint32_t io, string type="");
        /*
        * @brief 添加NodeDef属性
         * @param [in] attr_name 属性name
         * @param [in] attr_value 属性Value对象
         * @param [out] node_def
         * @return void
         *
         */
        static void AddNodeAttr(const string& attr_name, const tensorflow::AttrValue& value, tensorflow::NodeDef* node_def);

        static Status ClearUnusedParam(Graph* graph);

    };

    class GraphToFunctionDef
    {
    public:

        static Status RecordArg(Graph* graph ,vector<Edge *> input_edges);

        static Status RecordResult(Graph* graph ,vector<Edge *> output_edges);

        static Status DavGraphToFunctionDef(Graph* graph, const string& name,
                            FunctionDef* fdef);
        static Status BuildFunctionDef(Graph* graph,
                                        const string name_in,
                                        FunctionDefLibrary* library,
                                        NodeDef* call_node_def,
                                        vector<Edge *> input_edges,
                                        vector<Edge *> output_edges);
    };
    // Class that maintains a one-to-one original node name -> new name mapping.
    // We have to normalize the names used as input and output arguments to
    // match regexp "[a-z][a-z0-9_]*". Once we rename them, we risk creating
    // a name collision with the other node names, so if necessary we add
    // a suffix to make names unique.  So if we have an input named "A" and a
    // node in the function body named "a", they will be renamed to "a" and "a_0".
    class NodeNameMapping {
    public:
        NodeNameMapping() = default;

        // Normalize the input/output name and then make it unique.
        string Normalize(const string& name);

        // Make the node name unique.
        string Uniquify(const string& name);

        // Look up how a node name was previously normalized/uniquified.
        // Returns empty if name was never seen.
        string Renormalize(const string& name) const;

    private:
        string NormalizeHelper(string name) const;
        string UniquifyHelper(string name);

        std::set<string> used_names_;
        std::unordered_map<string, string> name_mapping_;
    };
}
// end namespace domi

#endif //_DOMI_OMG_PARSER_TENSORFLOW_TENSORFLOW_RESHAPE_PARSER_H_


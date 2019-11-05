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
#ifndef _DOMI_OMG_MODEL_OP_BUILDER_OP_BUILDER_H_
#define _DOMI_OMG_MODEL_OP_BUILDER_OP_BUILDER_H_

#include <string>
#include <vector>
#include "common/op/attr_value_util.h"
#include "common/types.h"
#include "omg/omg_types.h"
#include "proto/om.pb.h"
#include "omg/graph/graph.h"

using namespace std;

/*lint -e148*/
namespace cce
{
    struct tagCcTensor;
    typedef struct tagCcTensor *ccTensorDescriptor_t;
}

namespace domi {
    extern bool FC_WEIGHT_COMPRESS_FLAG;
    class OpDef;

    class OpBuilder {
    public:

        /**
         * @ingroup domi_omg
         * @brief 默认算子输入Tensor的数量
         */
        static const int DEFAULT_EXACT_INPUT_COUNT = 1;

        /**
         * @ingroup domi_omg
         * @brief 默认算子输出Tensor的数量
         */
        static const int DEFAULT_EXACT_OUTPUT_COUNT = 1;

        /* 由于内核限制，CCE侧对部分权值的地址有校验，需要保证地址能被512整除 */
        static const int32_t OP_WEIGHT_MEM_BASE_OFFSET = 512;

        /**
        * @ingroup domi_omg
        * @brief 基类构造函数
        * @param [in] op_def 对应算子的op_def
        */
        OpBuilder(OpDef* op_def)
            : op_def_(op_def),
              node_(nullptr)
        {
        }

        /**
        * @ingroup domi_omg
        * @brief 基类析构函数，Destroy构建时创建的描述
        *
        */
        virtual ~OpBuilder() {}

        /**
        * @ingroup domi_omg
        * @brief 设置Node
        * @param [in] 算子对应的图结点
        */
        void SetNode(Node* node);

        /**
        * @ingroup domi_omg
        * @brief 执行op build操作
        * @param [in|out] mem_offset 内存偏移
        * @return Status 运行结果
        *
        */
        Status Build(size_t& mem_offset);

        /**
         * @ingroup domi_omg
         * @brief 获取算子的输出描述
         * @param [out] v_output_desc 输出描述Vector
         * @return Status 运行结果
         *
         */
        virtual Status GetOutputDesc(vector<TensorDescriptor>& v_output_desc);

        /**
        * @ingroup domi_omg
        * @brief 调整算子的模型定义. 调用GetOutputDesc后，再调用该函数
        * @return Status
        */
        virtual Status AdjustParams();

        /**
         * @ingroup domi_omg
         * @brief 获取算子的所有workspace内存大小
         * @param [out] v_workspace_memory workspace内存值Vector
         * @return Status 运行结果
         *
         */
        virtual Status GetWorkSpaceMemory(vector<int64_t>& v_workspace_memory);

        /**
         * @ingroup domi_omg
         * @brief 转换算子内所有权重数据的数据类型并计算内存偏移
         * @param [in|out] mem_offset 内存偏移
         * @return SUCCESS 转换成功
         * @return 其他        转换失败
         *
         */
        virtual Status TransWeights(size_t& mem_offset);

        /**
         * @ingroup domi_omg
         * @brief 转换算子内所有权重数据的数据类型并计算内存偏移
         * @param [in] tensor_desc    待转换权重数据的描述
         * @param [in|out] weight     待转换的权重信息
         * @param [in|out] mem_offset 内存偏移
         * @return SUCCESS 转换成功
         * @return 其他        转换失败
         *
         */
        static Status TransWeight(const cce::ccTensorDescriptor_t tensor_desc,
                WeightDef* weight, size_t& mem_offset);

        /**
        * @ingroup domi_omg
        * @brief 设置format和datatype
        * @return Status
        */
        virtual void SetFormatAndDatatype(vector<TensorDescriptor>& v_output_desc);

        virtual Status BuildTvmBinFile(TVMBinInfo& tvm_bin_info);

        /**
        * @ingroup domi_omg
        * @brief 设置算子的FP16高精度标志位
        * @return Status
        */
        virtual Status HighPrecisionFp16();

        static bool IsBuildinOp(OpDef*& op_def);

        /**
         * @ingroup domi_omg
         * @brief 创建FractalZ格式的filter描述
         * @return Status 运行结果
         *
         */
        static Status SetQuantizeFactorOffset(QuantizeFactor* quantize_factor,
                                              size_t& mem_offset);

        /**
         * @ingroup domi_omg
         * @brief 创建FractalZ格式的filter描述
         * @return Status 运行结果
         *
         */
        static Status SetQuantizeFactorParamsOffset(QuantizeFactorParams* quantize_factor_params,
                                                    size_t& mem_offset);

        static Status GetImgInfo(int32_t &img_h,int32_t &img_w,OpDef*& op_def);

        /**
        * @ingroup domi_omg
        * @brief 设置realdimcnt，子类如果需要
        * @return SUCCESS
        * @return 其他
        *
        */
        virtual Status SetRealDimCnt(vector<uint32_t>);

        /**
        * @ingroup domi_omg
        * @brief 从用户指定的输入中解析原始图片的高和宽
        * @param [in/out] img_h img_w
        * @return SUCCESS 转换成功
        * @return PARAM_INVALID 转换失败
        *
        */
        static Status GetImgInfoFromInput(int32_t &img_h,int32_t &img_w);

        /**
        * @ingroup domi_omg
        * @brief 初始化Op Builder
        * @return Status 运行结果
        *
        */
        virtual Status Init();

    protected:

        //构建tvm op
        virtual Status BuildTvmOp();

        //构建ai cpu op
        Status BuildAiCpuOp();

        // 构造时传入的op_def，这个OpDef不完整，部分信息需要后续补齐
        OpDef *op_def_;

        // OpDef所属的图结点
        Node *node_;

    private:

        // 将GetOutputDesc获取到的算子输出描述 添加到OpDef中
        inline Status AddOutputDescs(vector<TensorDescriptor>& v_output_desc);

        // 是否有需要量化的算子
        bool HasQuantizeDescriptor();

        bool IsOutputTop(string op_name,int32_t index);

        // tensorflow设置真实realdimcnt，其他框架默认为4
        Status SetRealdimcntForType(vector<TensorDescriptor>& v_output_desc);

        //设置Caffe框架下TE算子的realdimcnt
        Status SetCaffeTERealdimcnt(vector<TensorDescriptor>& v_output_desc);

        // realdimcnt列表
        vector<uint32_t> real_dim_cnt_;

        // 默认取第一个input的realdimcnt，如果没有输入，默认4
        Status SetDefaultRealDimCnt();

        // 设置const op的输出大小
        Status AdjustConstOpOutputSize();

        // 对bias进行16个数对齐
        static void PadWeight(WeightDef* weight);

    };
};
/*lint +e148*/
#endif //_DOMI_OMG_MODEL_OP_BUILDER_OP_BUILDER_H_

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
#ifndef DOMI_OMG_REGISTER_H_
#define DOMI_OMG_REGISTER_H_

#include <google/protobuf/message.h>
#include "omg_types.h"
#include <unordered_map>
#include <memory>
#include <set>
#include <string>
#include <initializer_list>

/*lint -e148*/
namespace ge {
    class Operator;

    class TensorDesc;
    using TensorDescPtr = std::shared_ptr<TensorDesc>;
    using ConstTensorDescPtr = std::shared_ptr<const TensorDesc>;


    class Tensor;
    using TensorPtr = std::shared_ptr<Tensor>;
    using ConstTensorPtr = std::shared_ptr<const Tensor>;
}

namespace domi {

    Status AutoMappingFn(const google::protobuf::Message* op_src, ge::Operator& op);
    using google::protobuf::Message;


    using ParseParamFunc = std::function<domi::Status(const google::protobuf::Message*, ge::Operator&)>;
    using InferShapeFunc = std::function<domi::Status(const ge::Operator&, std::vector<ge::TensorDesc>&)>;
    using GetWorkspaceSizeFunc = std::function<domi::Status (const ge::Operator&, std::vector<int64_t>&)>;
    using UpdateOpDescFunc = std::function<domi::Status(ge::Operator&)>;
    using BuildTeBinFunc = std::function<domi::Status(const ge::Operator&, TEBinInfo&)>;

    class FMK_FUNC_HOST_VISIBILITY OpRegistrationData
    {
    public:
        OpRegistrationData(const std::string& om_optype);

        ~OpRegistrationData();

        OpRegistrationData& FrameworkType(domi::FrameworkType fmk_type);

        OpRegistrationData& OriginOpType (
            const std::initializer_list<std::string> & ori_optype_list);

        OpRegistrationData& OriginOpType (const std::string& ori_optype);

        OpRegistrationData& ParseParamsFn(ParseParamFunc parseParamFn);

        OpRegistrationData& InferShapeAndTypeFn(InferShapeFunc inferShapeFn);

        OpRegistrationData& UpdateOpDescFn(UpdateOpDescFunc updateOpDescFn);

        OpRegistrationData& GetWorkspaceSizeFn(GetWorkspaceSizeFunc getWorkspaceSizeFn);

        OpRegistrationData& TEBinBuildFn(BuildTeBinFunc buildTeBinFn);

        OpRegistrationData& ImplyType(domi::ImplyType imply_type);

        OpRegistrationData& Formats(
            const std::initializer_list<domi::tagDomiTensorFormat>& input_formats,
            const std::initializer_list<domi::tagDomiTensorFormat>& output_formats);

        OpRegistrationData& Formats(
            const domi::tagDomiTensorFormat& input_format,
            const domi::tagDomiTensorFormat& output_format);

        OpRegistrationData& WeightFormats(
            const std::initializer_list<domi::tagDomiTensorFormat>& weight_formats);

        bool Finalize();

    private:
        domi::FrameworkType fmk_type_;              // 框架类型
        std::set<std::string> ori_optype_set_;      // 原始模型中的op type,可能有多个
        std::string om_optype_;                // OM模型中的op type
        domi::ImplyType imply_type_;                // 算子执行的方式
        std::vector<domi::tagDomiTensorFormat> input_formats_; // 算子输入支持的数据格式
        std::vector<domi::tagDomiTensorFormat> output_formats_; // 算子输出支持的数据格式
        std::vector<domi::tagDomiTensorFormat> weight_formats_; // 算子权重支持的数据格式

        ParseParamFunc parseParamFn_;      // parseParam函数
        InferShapeFunc inferShapeFn_;    // inferShape函数
        GetWorkspaceSizeFunc getWorkspaceSizeFn_; // GetWorkspaceSizeFunc函数
        UpdateOpDescFunc   updateOpDescFn_;
        BuildTeBinFunc     buildTeBinFn_;
        friend class OpRegistry;
    };

    class FMK_FUNC_HOST_VISIBILITY OpReceiver
    {
    public:
        OpReceiver(OpRegistrationData& reg_data);
        ~OpReceiver(){}
    };

    class OpRegistry
    {
    public:
        static OpRegistry* Instance();

        bool Register(const OpRegistrationData& reg_data);

        domi::ImplyType GetImplyType(const std::string& op_type);

        void GetOpTypeByImplyType(std::vector<std::string>& vec_op_type,const domi::ImplyType& imply_type);

        void GetFormats(const std::string& op_type,
            std::vector<domi::tagDomiTensorFormat>& input_format_vector,std::vector<domi::tagDomiTensorFormat>& output_format_vector);

        void GetWeightFormats(const std::string& op_type,
            std::vector<domi::tagDomiTensorFormat>& format_vector);

        domi::ParseParamFunc GetParseParamFunc(const std::string& op_type);

        domi::InferShapeFunc GetInferShapeFunc(const std::string& op_type);

        domi::GetWorkspaceSizeFunc GetGetWorkspaceSizeFunc(const std::string& op_type);

        domi::UpdateOpDescFunc GetUpdateOpDescFunc(const std::string& op_type);

        domi::BuildTeBinFunc GetBuildTeBinFunc(const std::string& op_type);

    private:
        std::unordered_map<std::string, ImplyType> op_run_mode_map_;
        std::unordered_map<std::string, std::vector<domi::tagDomiTensorFormat>>
            op_input_formats_map_;
        std::unordered_map<std::string, std::vector<domi::tagDomiTensorFormat>>
            op_output_formats_map_;
        std::unordered_map<std::string, std::vector<domi::tagDomiTensorFormat>>
            op_weight_formats_map_;
        std::unordered_map<std::string, ParseParamFunc> opParseParamsFnMap_;
        std::unordered_map<std::string, InferShapeFunc> opInferShapeFnMap_;
        std::unordered_map<std::string, GetWorkspaceSizeFunc> opGetWorkspaceSizeFnMap_;
        std::unordered_map<std::string, UpdateOpDescFunc> opUpdateOpDescFnMap_;
        std::unordered_map<std::string, BuildTeBinFunc> opBuildTeBinFnMap_;

    };

#define REGISTER_CUSTOM_OP(name) REGISTER_CUSTOM_OP_UNIQ_HELPER(__COUNTER__, name)
#define REGISTER_CUSTOM_OP_UNIQ_HELPER(ctr, name) REGISTER_CUSTOM_OP_UNIQ(ctr, name)
#define REGISTER_CUSTOM_OP_UNIQ(ctr, name)                                          \
  static OpReceiver register_op##ctr    \
      __attribute__((unused)) =                                                  \
          OpRegistrationData(name)

}  // namespace domi
/*lint +e148*/
#endif  // DOMI_OMG_REGISTER_H_

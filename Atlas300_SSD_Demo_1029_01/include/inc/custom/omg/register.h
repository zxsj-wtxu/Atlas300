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

#include "common/types.h"
#include "omg_types.h"
#include <unordered_map>
#include <memory>
#include <set>
#include <string>
#include <initializer_list>

namespace domi {

    class OpRegistrationData
    {
    public:
        OpRegistrationData(const std::string& om_optype);

        OpRegistrationData& FrameworkType(domi::FrameworkType fmk_type);

        OpRegistrationData& OriginOpType (
            const std::initializer_list<std::string> & ori_optype_list);

        OpRegistrationData& OriginOpType (const std::string& ori_optype);

        OpRegistrationData& ParserCreatorFn(PARSER_CREATOR_FN parser_creator_fn);

        OpRegistrationData& BuilderCreatorFn(BUILDER_CREATOR_FN builder_creator_fn);

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
        PARSER_CREATOR_FN iparser_creator_fn_;      // parser创建函数
        BUILDER_CREATOR_FN ibuilder_creator_fn_;    // builder创建函数
        domi::ImplyType imply_type_;                // 算子执行的方式
        std::vector<domi::tagDomiTensorFormat> input_formats_; // 算子输入支持的数据格式
        std::vector<domi::tagDomiTensorFormat> output_formats_; // 算子输出支持的数据格式
        std::vector<domi::tagDomiTensorFormat> weight_formats_; // 算子权重支持的数据格式
        friend class OpRegistry;
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

    private:
        std::unordered_map<std::string, ImplyType> op_run_mode_map_;
        std::unordered_map<std::string, std::vector<domi::tagDomiTensorFormat>>
            op_input_formats_map_;
        std::unordered_map<std::string, std::vector<domi::tagDomiTensorFormat>>
            op_output_formats_map_;
        std::unordered_map<std::string, std::vector<domi::tagDomiTensorFormat>>
            op_weight_formats_map_;
    };

    class OpReceiver
    {
    public:
        OpReceiver(OpRegistrationData& reg_data);
        ~OpReceiver(){}
    };

#define DOMI_REGISTER_OP(name) DOMI_REGISTER_OP_UNIQ_HELPER(__COUNTER__, name)
#define DOMI_REGISTER_OP_UNIQ_HELPER(ctr, name) DOMI_REGISTER_OP_UNIQ(ctr, name)
#define DOMI_REGISTER_OP_UNIQ(ctr, name)                                          \
  static OpReceiver register_op##ctr    \
      __attribute__((unused)) =                                                  \
          OpRegistrationData(name)

#define PARSER_FN(clazz)  [] ()-> std::shared_ptr<OpParser> \
    {\
        return std::shared_ptr<OpParser>(new clazz());\
    }

#define BUILDER_FN(clazz) [] (OpDef* op_def)-> std::shared_ptr<OpBuilder> \
    {\
        return std::shared_ptr<OpBuilder>(new clazz(op_def));\
    }

}  // namespace domi

#endif  // DOMI_OMG_REGISTER_H_

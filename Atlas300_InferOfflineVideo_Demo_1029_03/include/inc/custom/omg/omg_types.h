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
#ifndef OMG_TYPES_H__
#define OMG_TYPES_H__

#include "common/fmk_error_codes.h"
#include "common/types.h"
#include <functional>
#include <memory>

#include <vector>
#include <unordered_map>

namespace domi {

    DEF_ERRORNO_OMG(PARSE_MODEL_FAILED, 0, "Failed to parse the model!");
    DEF_ERRORNO_OMG(PARSE_WEIGHTS_FAILED, 1, "Failed to parse the weights!");
    DEF_ERRORNO_OMG(NOT_INITIALIZED, 2, "It hasn't been initialized!");
    DEF_ERRORNO_OMG(TIMEOUT, 3, "Running time out!");

    /**
    * @ingroup domi_omg
    * @brief 支持的AI框架类型
    */
    enum FrameworkType
    {
        CAFFE = 0,
        TENSORFLOW = 3,
        ANDROID_NN,
        FRAMEWORK_RESERVED,
    };

    /**
     * @ingroup domi_omg
     * @brief 执行模式
     */
    enum RunMode
    {
        // 生成离线模型
        GEN_OM_MODEL = 0,

        // 模型转为JSON格式
        MODEL_TO_JSON = 1,

        // 仅做预检
        ONLY_PRE_CHECK = 3
    };

    enum HighPrecisionMode
    {
        //普通模式，不使能FP16高精度
        HIGH_PRECISION_DEFAULT = 0,

        //高精度模式，使能FP16高精度(涉及算子Convolution、FullConnect、AvgPooling)
        HIGH_PRECISION_FP16 = 1
    };

    struct TVMBinInfo
    {
        std::string bin_file_path; // 2018.11.21 已废弃，以后自动从json文件binFileName字段获取。为了兼容以前用户编写的用例，字段不删除。
        std::string json_file_path;
        std::string ddk_version;
    };

    class OpParser;
    class OpBuilder;
    class OpDef;
    using PARSER_CREATOR_FN = std::function<std::shared_ptr<OpParser>(void)>;
    using BUILDER_CREATOR_FN = std::function<std::shared_ptr<OpBuilder>(OpDef*)>;

    /**
 * @brief 保存OMG的全局数据
 */
    struct OmgContext
    {
        domiTensorFormat_t format;

        // 命令行指定的输入format
        std::unordered_map<std::string, domiTensorFormat_t> input_nodes_format_map;
        std::vector<domiTensorFormat_t> output_formats;

        std::unordered_map<std::string, std::vector<int64_t>> input_dims;
        // 解决重名算子，算子和对应网络的映射，格式e.g. Detectionoutput:SsdDetectiontOutput
        std::map<std::string, std::string> op_conf_map;
        // 保存网络输出节点，key = 算子名称，value = index，index标示该算子的输出索引
        std::map<std::string, std::vector<int32_t>> out_nodes_map;
        // 保存aicpu 自定义算子 so路径
        std::vector<std::string> aicpu_op_run_paths;
        // 保存ddk 版本号
        std::string ddk_version;
        // 保存整网优先使用的format
        domiTensorFormat_t net_format;
        FrameworkType type;
        RunMode run_mode;
        bool train_flag = false;
        //是否使用FP16高精度
        int32_t fp16_high_precision = HIGH_PRECISION_DEFAULT;
    };

    /**
     * @ingroup domi_omg
     * @brief 获取OMG上下文
     * @return OmgContext对象的引用
     */
    OmgContext& GetContext();
}

#endif // OMG_TYPES_H__

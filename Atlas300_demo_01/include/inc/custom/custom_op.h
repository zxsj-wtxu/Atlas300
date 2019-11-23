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
#ifndef DOMI_CUSTOM_OP_H
#define DOMI_CUSTOM_OP_H
#include <string>
#include <vector>

namespace custom
{

    enum BinType
    {
        TE_AICORE = 0,  /* 以AICore方式实现的TVM自定义算子 */
        TE_AICPU = 1,  /* 以AICpu方式实现的TVM自定义算子 */
        AICPU = 2,      /* 用户定义的AICpu算子 */
    };
    struct ErrorInfo
    {
        uint32_t    error_code;
        std::string error_msg;
    };

    /**
    * param[in] name   算子kernel名称
    * param[in] type    算子类型: 0 - AICORE; 1 - AICPU
    * param[in] bin_file  算子二进制文件路径
    * param[in] in_files  输入数据文件路径
    * param[in] out_files 输出数据文件路径
    * param[in] out_buf_sizes   输出数据大小
    * param[in] workspace_sizes   workspace大小
    * param[in] op_cfg_file  用户指定的算子的输入输出描述信息
    * param[in] param  用户指定的算子参数结构体地址信息
    * param[in] param_len  用户指定的算子参数结构体的长度，取sizeof(结构体)
    * return              错误信息. 其中, 错误码: 0成功，其余错误
    * detail:自定义算子单算子调测入口函数
    */
    ErrorInfo custom_op_run(const std::string& name, int32_t type, const std::string& bin_file,
                         const std::vector< std::string >& in_files,
                         const std::vector< std::string >& out_files,
                         const std::vector< uint32_t >&    out_buf_sizes,
                         const std::vector< uint32_t >& workspace_sizes = std::vector<uint32_t>(),
                         const std::string& op_cfg_file = "", const void *param = nullptr, int param_len = 0);
    /**
    * param[in] expect_file   期望数据文件
    * param[in] actual_file   实际数据文件
    * param[in] data_type  算子类型，目前支持FP32（0）、FP16（1）
    * param[in] precision_deviation  精度偏差, 单数据相对误差允许范围
    * param[in] statistical_discrepancy  统计偏差, 整个数据集中精度偏差超过门限的数据量占比
    * param[out] compare_result 比较结果， true：对比成功；false：对比失败
    * return    错误信息. 其中, 错误码: 0成功，其余错误
    */
    ErrorInfo custom_op_compare(const std::string& expect_file, const std::string& actual_file,
                             int32_t data_type, float precision_deviation,
                             float statistical_discrepancy, bool& compare_result);

}  // namespace custom

#endif  // DOMI_CUSTOM_OP_H

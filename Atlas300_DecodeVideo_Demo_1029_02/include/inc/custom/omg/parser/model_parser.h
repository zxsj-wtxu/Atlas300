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
#ifndef DOMI_OMG_PARSER_MODEL_PARSER_H_
#define DOMI_OMG_PARSER_MODEL_PARSER_H_

#include <string>
#include <vector>
#include "common/types.h"
#include "omg/omg_types.h"
#include "omg/graph/graph.h"

#include <google/protobuf/message.h>


using namespace std;

namespace domi {
    class ModelParser {
    public:

        explicit ModelParser() {}

        virtual ~ModelParser() {}

        /**
        * @ingroup domi_omg
        * @brief 解析网络模型数据
        * @param [in] file  网络模型文件路径
        * @param [in|out]  graph 保存解析后网络信息的图
        * @return SUCCESS  解析成功
        * @return 其他          解析失败
        */
        virtual Status Parse(const char* file, Graph *graph) = 0;
        /**
        * @ingroup domi_omg
        * @brief 解析网络模型数据
        * @param [in] proto  网络模型
        * @param [in|out]  graph 保存解析后网络信息的图
        * @return SUCCESS  解析成功
        * @return 其他          解析失败
        */
        virtual Status ParseProto(google::protobuf::Message* proto, Graph *graph) = 0;
        /**
        * @ingroup domi_omg
        * @brief 将模型文件转换为JSON格式
        * @param [in] model_file  待转换的模型文件路径
        * @param [out] json_file 转换后的JSON文件路径
        * @return SUCCESS 解析成功
        * @return 其他 解析失败
        */
        virtual Status ToJson(const char* model_file, const char* json_file)
        {
            return SUCCESS;
        }
    };

}  // namespace domi

#endif  // DOMI_OMG_PARSER_MODEL_PARSER_H_

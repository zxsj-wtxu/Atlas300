/** 
* @file ai_model_parser.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef _HIAI_MODEL_PARSER_H
#define _HIAI_MODEL_PARSER_H
#include "hiaiengine/ai_types.h"
#include "hiaiengine/ai_tensor.h"

namespace hiai
{

        /**
        * @ingroup hiai
        * @brief 解析一个模型文件,获取模型信息
        * @param [in] model_path 模型文件路径
        * @param [in] key 模型解密秘钥
        * @param [out] model_desc 模型信息
        * @return Status 运行结果
        */

        hiai::AIStatus getModelInfo(const std::string & model_path, 
            const std::string & key, AIModelDescription & model_desc);
}


#endif

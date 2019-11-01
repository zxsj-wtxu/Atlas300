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
#ifndef _DOMI_OMG_MODEL_OP_BUILDER_TVM_OP_BUILDER_H_
#define _DOMI_OMG_MODEL_OP_BUILDER_TVM_OP_BUILDER_H_
#include "common/types.h"
#include "omg/omg_types.h"
#include "omg/model/op_builder/op_builder.h"

using JsonHandle = void*;

namespace domi {
    class TVMOpBuilder : public OpBuilder {
    public:

        /**
        * @ingroup domi_omg
        * @brief 构建函数，需要创建卷积描述，输入描述，参数描述
        * @param [in] op_def 对应算子的op_def
        */
        TVMOpBuilder(OpDef *op_def);
        /**
        * @ingroup domi_omg
        * @brief 构建函数，Destroy构建时创建的描述
        */
        ~TVMOpBuilder();

        /**
        * @ingroup domi_omg
        * @brief 获取算子的所有workspace内存大小
        * @param [out] v_workspace_memory workspace内存值Vector
        * @return Status 运行结果
        * @author 
        */
        Status GetWorkSpaceMemory(vector<int64_t>& v_workspace_memory) override;
        
    protected:
        /**
        * @ingroup domi_omg
        * @brief 构造tvm 算子
        * @param [out] 
        * @return Status 运行结果
        * @author 
        */
        Status BuildTvmOp() override;
    private:
        //保存TVM算子workspace大小
        vector<int32_t> tvm_workspace_sizes_;

        /**
        * @ingroup domi_omg
        * @brief tvm 数据封装到opdef
        * @param [out] 
        * @return Status 运行结果
        * @author 
        */
        Status PackageTvmBinFile(const JsonHandle handle);

        Status PackageTvmJsonInfo(const TVMBinInfo& info);

        Status ReadJsonFile(const std::string& file, JsonHandle handle);

        Status ParseTvmMagic(const JsonHandle handle);

        Status ParseTvmBlockDim(const JsonHandle handle);

        Status ParseTvmWorkSpace(const JsonHandle handle);

        Status ParseTvmMetaData(const JsonHandle handle);

        Status ParseTvmKernelName(const JsonHandle handle);

        std::string GetStrValueFromJson(const JsonHandle handle, const std::string& key);

    private:
        std::string tvm_file_path_; // tvm算子.o .json .so等文件的路径，应该都在同一个目录
    };
};

#endif //_DOMI_OMG_MODEL_OP_BUILDER_TVM_OP_BUILDER_H_

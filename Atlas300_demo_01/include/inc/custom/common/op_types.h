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
#ifndef DOMI_OP_TYPE_H__
#define DOMI_OP_TYPE_H__

#include <string>
#include <set>

namespace domi
{

class OpTypeContainer
{
public:
    static OpTypeContainer * Instance()
    {
        static OpTypeContainer instance;
        return &instance;
    }

    void Register(const std::string& op_type)
    {
        op_type_list_.insert(op_type);
    }

    bool IsExisting(const std::string& op_type)
    {
        auto iter_find = op_type_list_.find(op_type);
        return iter_find != op_type_list_.end();
    }

protected:
    OpTypeContainer() {}

private:
    std::set<std::string> op_type_list_;
};

class OpTypeRegistrar
{
public:
    OpTypeRegistrar(const std::string& op_type)
    {
        OpTypeContainer::Instance()->Register(op_type);
    }
    ~OpTypeRegistrar(){}
};

#define REGISTER_OPTYPE(var_name, str_name) \
                static const char* var_name __attribute__((unused)) = str_name;\
                const OpTypeRegistrar g_##var_name##_reg(str_name);

#define REGISTER_OPSAMETYPE(var_name, str_name) \
                static const char* var_name __attribute__((unused)) = str_name;

#define IS_OPTYPE_EXISTING(str_name) (OpTypeContainer::Instance()->IsExisting(str_name))

} // namespace domi

#endif // DOMI_OP_TYPE_H__

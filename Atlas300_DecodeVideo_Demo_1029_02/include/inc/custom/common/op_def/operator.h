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
#ifndef DOMI_COMMON_OP_OPERATOR_H
#define DOMI_COMMON_OP_OPERATOR_H

#include "common/op_def/op_schema.h"
#include "common/tuple.h"
#include "proto/om.pb.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace domi
{

struct OpAttribute
{
    OpAttribute(const std::string& _name, AttrDef _value) : name(_name), value(_value){};
    const std::string name;
    AttrDef           value;
};

class Operator
{
public:
    Operator(const std::string& type);
    Operator(){};

    virtual ~Operator();

    Operator& Input(const Operator& in_op, uint32_t index = 0);

    Operator& Attr(const OpAttribute& op_attr);

    Operator& AttrVector(std::string key, std::vector< int32_t >& value);

    Operator& Name(const std::string& name);

    Operator& Type(const std::string& type);

    Operator& InputTensorDesc(const TensorDescriptor& input_tensordesc);

    Operator& OutputTensorDesc(const TensorDescriptor& output_tensordesc);

    Operator& Attr_bt(const std::string& name, const std::string& value);

    // Register "optional" attribute with default value.
#define ATTR_SETTER_WITH_VALUE(TypeName)                                           \
    Operator& Attr(const std::string& name, const TypeName& value);                \
    Operator& Attr(const std::string& name, const std::vector< TypeName >& value); \
    Operator& Attr(const std::string& name, const domi::Tuple< TypeName >& value)

    ATTR_SETTER_WITH_VALUE(uint32_t);
    ATTR_SETTER_WITH_VALUE(int64_t);
    ATTR_SETTER_WITH_VALUE(bool);
    ATTR_SETTER_WITH_VALUE(float);
    ATTR_SETTER_WITH_VALUE(std::string);

    const std::string& GetName() const
    {
        return name_;
    }

    const std::string& GetType() const
    {
        return type_;
    }

    const std::vector< std::string >& GetInputs() const
    {
        return inputs_;
    }

    const std::vector< TensorDescriptor >& GetInputTensorDesc() const
    {
        return input_descs_;
    }

    const std::vector< TensorDescriptor >& GetOutputTensorDesc() const
    {
        return output_descs_;
    }

    const std::unordered_map< std::string, OpAttribute > GetOpAttrs() const
    {
        return op_attrs_;
    }

    bool HasAttr(const std::string& name) const
    {
        return op_attrs_.count(name);
    }

    const OpSchema* GetSchema() const
    {
        return op_schema_;
    }

    int64_t GetIntAttr(const std::string& name) const;

    uint32_t GetUintAttr(const std::string& name) const;

    float GetFloatAttr(const std::string& name) const;

    bool GetBoolAttr(const std::string& name) const;

    std::string GetStringAttr(const std::string& name) const;

    IntTuple GetIntTupleAttr(const std::string& name) const;

    UintTuple GetUintTupleAttr(const std::string& name) const;

    FloatTuple GetFloatTupleAttr(const std::string& name) const;

    BoolTuple GetBoolTupleAttr(const std::string& name) const;

    StringTuple GetStringTupleAttr(const std::string& name) const;

private:
    const OpSchema*                                op_schema_;
    std::string                                    name_;
    std::string                                    type_;
    std::vector< std::string >                     inputs_;
    std::unordered_map< std::string, OpAttribute > op_attrs_;
    std::vector< TensorDescriptor >                input_descs_;
    std::vector< TensorDescriptor >                output_descs_;
};

}  // namespace domi

#endif  // DOMI_COMMON_OP_OPERATOR_H

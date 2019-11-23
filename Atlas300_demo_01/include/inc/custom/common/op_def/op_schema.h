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
#ifndef DOMI_COMMON_OP_SCHEMA_H
#define DOMI_COMMON_OP_SCHEMA_H

#include "proto/om.pb.h"
#include "common/tuple.h"
#include <string>
#include <unordered_map>
#include <vector>


namespace domi
{

enum class AttributeType
{
    UNDEFINED,
    INT,
    UINT,
    BOOL,
    FLOAT,
    STRING,
    BYTES,

    // TODO 增加Tensor
    INTLIST,
    UINTLIST,
    BOOLLIST,
    FLOATLIST,
    STRINGLIST
};

class OpSchema;

class OpSchemaRegistry;

class OpSchema
{
public:
    // Formal parameter options.
    enum FormalParameterOption
    {
        // The input formal parameter is single and not optional.
        // Number of this input is 1.
        Single = 0,
        // The input formal parameter is single and optional.
        // Number of this input is 0 or 1.
        Optional = 1,
        // The input formal parameter is variadic.
        // Number of this input is [1, n].
        Variadic = 2,
    };

    // Formal parameter represenation, including input/output name, typeStr,
    // description, and type constraints.
    class FormalParameter
    {
    public:
        // Constructor.
        FormalParameter() = default;

        explicit FormalParameter(const std::string&    name,
                                 FormalParameterOption param_option = Single);

        // Get formal parameter name.
        const std::string& Name() const;

        // Get the parameter option, it could be Single, Optional or Variadic.
        FormalParameterOption Option() const;

    private:
        friend class OpSchema;

        // Formal parameter name.
        std::string name_;

        // Formal parameter option.
        FormalParameterOption param_option_;
    };

    OpSchema(const std::string& name);

    OpSchema& Input(const std::string& name, FormalParameterOption param_option = Single);

    OpSchema& Output(const std::string& name, FormalParameterOption param_option = Single);

    struct Attribute
    {
        Attribute(const std::string& name_, AttributeType type_, bool required_)
            : name(name_), type(type_), required(required_)
        {
        }

        Attribute(const std::string& name_, AttributeType type_, AttrDef default_value_)
            : name(name_), type(type_), required(false), default_value(default_value_)
        {
        }

        const std::string name;
        AttributeType     type;
        bool              required;
        AttrDef           default_value;
    };

    OpSchema& Attr(const Attribute& attr);

    // Register "optional" attribute with default value.
#define ATTR_SETTER_WITH_DEFAULT_VALUE(TypeName)                                               \
    OpSchema& Attr(const std::string& name, AttributeType type, const TypeName& defaultValue); \
    OpSchema& Attr(const std::string& name, AttributeType type,                                \
                   const std::vector<TypeName>& defaultValue);                                 \
    OpSchema& Attr(const std::string& name, AttributeType type,                                \
                   const domi::Tuple<TypeName>& defaultValue);

    ATTR_SETTER_WITH_DEFAULT_VALUE(uint32_t)
    ATTR_SETTER_WITH_DEFAULT_VALUE(int64_t)
    ATTR_SETTER_WITH_DEFAULT_VALUE(bool)
    ATTR_SETTER_WITH_DEFAULT_VALUE(float)
    ATTR_SETTER_WITH_DEFAULT_VALUE(std::string)

    // Register "required" attribute without default value.
    OpSchema& AttrRequired(const std::string& name, AttributeType type);

    bool HasDefaultAttr(const std::string& name) const;

    const AttrDef& GetDefaultAttr(const std::string& name) const;

    // 校验OpDef是否符合要求
    bool Verify(const OpDef* op_def) const;

private:
    friend class OpSchemaRegistry;
    // 校验proto是否合法
    bool Finalize();

    std::string name_;

    std::vector<FormalParameter> inputs_;

    std::vector<FormalParameter> outputs_;

    std::unordered_map<std::string, Attribute> attributes_;
};

class OpSchemaFactory
{
public:
    // this is a singleton object
    static OpSchemaFactory* Instance();

    const OpSchema* Get(const std::string& op) const;

private:
    OpSchemaFactory();
    ~OpSchemaFactory();

    friend class OpSchemaRegistry;
    // the op schema map
    std::unordered_map<std::string, OpSchema> op_schema_map_;
};

class OpSchemaRegistry
{
public:
    OpSchemaRegistry(OpSchema& op_schema);
    ~OpSchemaRegistry(){}
};

#define DOMI_OP_SCHEMA(name) DOMI_OP_SCHEMA_UNIQ_HELPER(__COUNTER__, name)
#define DOMI_OP_SCHEMA_UNIQ_HELPER(ctr, name) DOMI_OP_SCHEMA_UNIQ(ctr, name)
#define DOMI_OP_SCHEMA_UNIQ(ctr, name) \
    static OpSchemaRegistry op_schema_registry##ctr __attribute__((unused)) = ::domi::OpSchema(#name)

}  // namespace domi

#endif  // DOMI_COMMON_OP_SCHEMA_H
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
#ifndef _COMMON_OP_ATTR_VALUE_UTIL_H
#define _COMMON_OP_ATTR_VALUE_UTIL_H

#include <unordered_map>
#include "common/types.h"

#include "common/op/attr_define.h"
#include "proto/om.pb.h"
#include <google/protobuf/map.h>

namespace domi
{
        using AttrDefMap = ::google::protobuf::Map<::std::string, ::domi::AttrDef>;
        using AttrDefPair = ::google::protobuf::MapPair<std::string, domi::AttrDef>;

        void AddOpAttr(const std::string& key, AttrDef& attr, OpDef* opdef);
        //DEFINE_ADD_ATTR_VALUE
        void AddOpAttr(const std::string& key, const std::string& value, AttrDefMap* attrs);
        void AddOpAttr(const std::string& key, const char* value, AttrDefMap* attrs);
        void AddOpAttr(const char* key, const char* value, AttrDefMap* attrs);
        void AddOpAttr(const std::string& key, const uint32_t value, AttrDefMap* attrs);
        void AddOpAttr(const std::string& key, const int32_t value, AttrDefMap* attrs);
        void AddOpAttr(const std::string& key, const int64_t value, AttrDefMap* attrs);
        void AddOpAttr(const std::string& key, const float value, AttrDefMap* attrs);
        void AddOpAttr(const std::string& key, const double value, AttrDefMap* attrs);
        void AddOpAttr(const std::string& key, const bool value, AttrDefMap* attrs);

        void AddOpAttr(const std::string& key, const AttrDef_ListValue& value, AttrDefMap* attrs);

        //DEFINE_ADD_ATTR_VALUE
        void AddOpAttr(const std::string& key, const std::string& value, OpDef* opdef);
        void AddOpAttr(const std::string& key, const char* value, OpDef* opdef);
        void AddOpAttr(const char* key, const char* value, OpDef* opdef);
        void AddOpAttr(const std::string& key, const uint32_t value, OpDef* opdef);
        void AddOpAttr(const std::string& key, const int32_t value, OpDef* opdef);
        void AddOpAttr(const std::string& key, const int64_t value, OpDef* opdef);
        void AddOpAttr(const std::string& key, const float value, OpDef* opdef);
        void AddOpAttr(const std::string& key, const double value, OpDef* opdef);
        void AddOpAttr(const std::string& key, const bool value, OpDef* opdef);

        void AddOpAttr(const std::string& key, const AttrDef_ListValue& value, OpDef* opdef);

        void AddOpBytesAttr(const std::string& key, const void * value, size_t size, OpDef* opdef);

        //DEFINE_ADD_ATTR_VALUE_LIST
        void AddOpAttrList(const std::string& key, const double value, AttrDefMap* attrs);
        void AddOpAttrList(const std::string& key, const float value, AttrDefMap* attrs);
        void AddOpAttrList(const std::string& key, const uint32_t value, AttrDefMap* attrs);
        void AddOpAttrList(const std::string& key, const int32_t value, AttrDefMap* attrs);
        void AddOpAttrList(const std::string& key, const std::string value, AttrDefMap* attrs);
        void AddOpAttrList(const std::string& key, const double value, OpDef* opdef);
        void AddOpAttrList(const std::string& key, const float value, OpDef* opdef);
        void AddOpAttrList(const std::string& key, const uint32_t value, OpDef* opdef);
        void AddOpAttrList(const std::string& key, const int32_t value, OpDef* opdef);
        void AddOpAttrList(const std::string& key, const bool value, OpDef* opdef);
        void AddOpAttrList(const std::string& key, const int64_t value, OpDef* opdef);

        void AddOpAttrList(const std::string& key, const std::string& value, OpDef* opdef);


        bool GetOpAttr(const std::string& key, std::string* value, const OpDef* opdef);
        bool GetOpAttr(const std::string& key, int32_t* value, const OpDef* opdef);
        bool GetOpAttr(const std::string& key, int64_t* value, const OpDef* opdef);
        bool GetOpAttr(const std::string& key, uint32_t* value, const OpDef* opdef);
        bool GetOpAttr(const std::string& key, float* value, const OpDef* opdef);
        bool GetOpAttr(const std::string& key, double* value, const OpDef* opdef);
        bool GetOpAttr(const std::string& key, bool* value, const OpDef* opdef);
        bool GetOpAttr(const std::string& key, AttrDef_ListValue * value, const OpDef * opdef);

        uint32_t GetOpAttrListSize(const std::string& key, std::string value, const OpDef* opdef);
        uint32_t GetOpAttrListSize(const std::string& key, int32_t value, const OpDef* opdef);
        uint32_t GetOpAttrListSize(const std::string& key, int64_t value, const OpDef* opdef);
        uint32_t GetOpAttrListSize(const std::string& key, uint32_t value, const OpDef* opdef);
        uint32_t GetOpAttrListSize(const std::string& key, float value, const OpDef* opdef);
        uint32_t GetOpAttrListSize(const std::string& key, double value, const OpDef* opdef);
        uint32_t GetOpAttrListSize(const std::string& key, bool value, const OpDef* opdef);

        bool GetBytesAttr(const std::string& key, std::string* value, const OpDef* opdef);
        bool GetBytesAttr(const std::string& key, std::string* value, const ModelDef* modeldef);

        void AddModelAttr(const std::string& key, const std::string& value, ModelDef* modeldef);
        void AddModelAttr(const std::string& key, const char* value, ModelDef* modeldef);
        void AddModelAttr(const char* key, const char* value, ModelDef* modeldef);
        void AddModelAttr(const std::string& key, const uint32_t value, ModelDef* modeldef);
        void AddModelAttr(const std::string& key, const int32_t value, ModelDef* modeldef);
        void AddModelAttr(const std::string& key, const int64_t value, ModelDef* modeldef);
        void AddModelAttr(const std::string& key, const float value, ModelDef* modeldef);
        void AddModelAttr(const std::string& key, const double value, ModelDef* modeldef);
        void AddModelAttr(const std::string& key, const bool value, ModelDef* modeldef);
        void AddModelAttr(const std::string& key, const void * value, size_t size, ModelDef* modeldef);
        void AddModelAttr(const std::string& key, const AttrDef_ListValue& value, ModelDef* modeldef);

        void AddModelAttrList(const std::string& key, const double value, ModelDef* modeldef);
        void AddModelAttrList(const std::string& key, const float value, ModelDef* modeldef);
        void AddModelAttrList(const std::string& key, const uint32_t value, ModelDef* modeldef);
        void AddModelAttrList(const std::string& key, const int32_t value, ModelDef* modeldef);
        void AddModelAttrList(const std::string& key, const std::string& value, ModelDef* modeldef);

        bool GetModelAttr(const std::string& key, std::string* value, const ModelDef* modeldef);
        bool GetModelAttr(const std::string& key, int32_t* value, const ModelDef* modeldef);
        bool GetModelAttr(const std::string& key, int64_t* value, const ModelDef* modeldef);
        bool GetModelAttr(const std::string& key, uint32_t* value, const ModelDef* modeldef);
        bool GetModelAttr(const std::string& key, float* value, const ModelDef* modeldef);
        bool GetModelAttr(const std::string& key, double* value, const ModelDef* modeldef);
        bool GetModelAttr(const std::string& key, bool* value, const ModelDef* modeldef);
        bool GetModelAttr(const std::string& key, AttrDef_ListValue * value, const ModelDef* modeldef);

        bool HasOpAttr(const OpDef* opdef, const std::string attr_name);

        void SetAttrDef(const std::string& value, AttrDef* out);
        void SetAttrDef(const char* value, AttrDef* out);
        void SetAttrDef(const uint32_t value, AttrDef* out);
        void SetAttrDef(const int32_t value, AttrDef* out);
        void SetAttrDef(const float value, AttrDef* out);
        void SetAttrDef(const double value, AttrDef* out);
        void SetAttrDef(const bool value, AttrDef* out);
        void SetAttrList(const std::string& value, AttrDef* out);
        void SetAttrList(const bool value, AttrDef* out);
        void SetAttrList(const float value, AttrDef* out);
        void SetAttrList(const double value, AttrDef* out);
        void SetAttrList(const uint32_t value, AttrDef* out);

        bool GetAttrDefValue(const std::string& key, std::string* value, const AttrDefMap &attr);
        bool GetAttrDefValue(const std::string& key, int32_t* value, const AttrDefMap &attr);
        bool GetAttrDefValue(const std::string& key, int64_t* value, const AttrDefMap &attr);
        bool GetAttrDefValue(const std::string& key, uint32_t* value, const AttrDefMap &attr);
        bool GetAttrDefValue(const std::string& key, float* value, const AttrDefMap &attr);
        bool GetAttrDefValue(const std::string& key, double* value, const AttrDefMap &attr);
        bool GetAttrDefValue(const std::string& key, bool* value, const AttrDefMap &attr);
        bool GetAttrDefValue(const std::string& key, AttrDef_ListValue * value, const AttrDefMap &attr);
        bool GetAttrDefValue(const std::string& key, NamedAttrs *& value, AttrDefMap *attr);
        bool GetAttrDefValue(const std::string& key, const NamedAttrs *& value, const AttrDefMap &attr);

        bool GetAttrDefListValue(const std::string& key, int idx, int32_t* value, const AttrDefMap &attr);
        bool GetAttrDefListValue(const std::string& key, int idx, uint32_t* value, const AttrDefMap &attr);
        bool GetAttrDefListValue(const std::string& key, int idx, float* value, const AttrDefMap &attr);
        bool GetAttrDefListValue(const std::string& key, int idx, double* value, const AttrDefMap &attr);
}

#endif

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
#ifndef DOMI_COMMON_OP_DEF_IR_PB_CONVERTER_H
#define DOMI_COMMON_OP_DEF_IR_PB_CONVERTER_H

#include "common/fmk_error_codes.h"
#include "common/op_def/op_schema.h"
#include "common/op_def/operator.h"
#include "proto/om.pb.h"

namespace domi
{
Status ConvertToOpDef(const Operator& op, OpDef* op_def);

Status ConvertFromOpDef(const OpDef* op_def, Operator& op);

}  // namespace domi

#endif  // DOMI_COMMON_OP_DEF_IR_PB_CONVERTER_H
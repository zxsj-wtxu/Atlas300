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
#ifndef _DOMI_OMG_PARSER_CAFFE_CAFFE_TVM_OP_PARSER_H_
#define _DOMI_OMG_PARSER_CAFFE_CAFFE_TVM_OP_PARSER_H_

#include "caffe_op_parser.h"

namespace domi
{
    class CaffeTVMOpParser: public CaffeOpParser
    {
    public:
        virtual Status ParseWeights(const Message* op_src, OpDef* op_dest) override;

    };

}
// end namespace domi

#endif //_DOMI_OMG_PARSER_CAFFE_CAFFE_TVM_OP_PARSER_H_

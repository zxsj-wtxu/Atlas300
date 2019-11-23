/*
Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/

#ifndef __CC_CUSTOMIZE_API__
#define __CC_CUSTOMIZE_API__
#include <stdint.h>

#define CC_DEVICE_DIM_MAX 8
typedef enum tagOpTensorFormat
{
    OP_TENSOR_FORMAT_NC1HWC0 = 0,
    OP_TENSOR_FORMAT_ND,
    OP_TENSOR_FORMAT_RESERVED,

} opTensorFormat_t;


typedef enum tagOpDataType
{
    OP_DATA_FLOAT = 0,             /**< float type */
    OP_DATA_HALF,                  /**< fp16 type */
    OP_DATA_INT8,                  /**< int8 type */
    OP_DATA_INT32,                 /**< int32 type */
    OP_DATA_UINT8,                 /**< uint8 type */
    OP_DATA_HALF_UINT16_PROPOSAL,  /**<mixed type for proposal*/
    OP_DATA_RESERVED
} opDataType_t;

typedef struct tagOpTensor
{
    // real dim info
    opTensorFormat_t format;
    //char align1[4];
    opDataType_t data_type;
    //char align2[4];
    int32_t dim_cnt;
    int32_t mm;   //lint !e148
    int32_t dim[CC_DEVICE_DIM_MAX];  //lint !e148
} opTensor_t;

typedef opTensor_t tagCcAICPUTensor;
typedef void * rtStream_t;
typedef void (*aicpu_run_func)(opTensor_t **, void **, int32_t,
                               opTensor_t **, void **, int32_t, void *, rtStream_t);


#endif /*__CC_CUSTOMIZE_API__*/


/*
Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/

#ifndef __CC_BLAS_STRUCT_API__
#define __CC_BLAS_STRUCT_API__
#include <stdint.h>

typedef enum {
    CCBLAS_FILL_MODE_LOWER = 0,
    CCBLAS_FILL_MODE_UPPER = 1
} ccblasFillMode_t;

typedef enum {
    CCBLAS_OP_N = 0,
    CCBLAS_OP_T = 1,
} ccblasOperation_t;

typedef enum {
    CCBLAS_DIAG_NON_UNIT = 0,
    CCBLAS_DIAG_UNIT = 1
} ccblasDiagType_t;

#endif /*__CC_BLAS_STRUCT_API__*/

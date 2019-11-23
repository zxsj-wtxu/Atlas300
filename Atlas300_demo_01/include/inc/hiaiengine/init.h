/**
* @file init.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_INIT_H_
#define INC_HIAIENGINE_INIT_H_

#include <stddef.h>
#include <hiaiengine/status.h>

/*
* @brief: It is required to perform HIAI_Init() before using HIAI System
* @params: [none]
* @return: HIAI_OK if successful, otherwise return specific error code
*/
#ifdef __cplusplus
extern "C" {
#endif
    /**
    * @ingroup
    * @brief 初始化HdcClient，建立连接
    * @param [in] deviceID:目标device的id
    * @return 错误码
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_Init(uint32_t deviceID);

#ifdef __cplusplus
}
#endif

#endif  // INC_HIAIENGINE_INIT_H_


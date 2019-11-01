/**
* @file init.h
*
* Copyright (C) Huawei Technologies Co., Ltd. 2018-2019. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_INIT_H_
#define INC_HIAIENGINE_INIT_H_

#include <stddef.h>
#include <hiaiengine/status.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @ingroup
* @brief： 用于初始化HIAI 系统
* @param [in] deviceID: 系统默认的全局device ID. 
*        若Graph没有单独配置device ID，则使用此处配置的device ID。否则使用Graph自己单独配置的device ID.
* @return: 成功返回 HIAI_OK，否则返回具体错误码
*/
HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_Init(uint32_t deviceID);

#ifdef __cplusplus
}
#endif

#endif  // INC_HIAIENGINE_INIT_H_


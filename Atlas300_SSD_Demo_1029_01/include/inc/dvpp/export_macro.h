/**
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef _EXPORT_MACRO_H_
#define _EXPORT_MACRO_H_

#if defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define DVPP_EXPORT __attribute__((visibility("default")))
#else
#define DVPP_EXPORT
#endif

#endif // _EXPORT_MACRO_H_
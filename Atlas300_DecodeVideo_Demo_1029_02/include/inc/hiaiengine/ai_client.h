/**
* @file ai_client.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef _AI_CLIENT_H_
#define _AI_CLIENT_H_

/*
*    为了同时兼容Android和mini平台，ai_client.h文件中包含两套头文件代码：
*    1. Android平台头文件代码：包含AIClient类、AIModelManagerClient类，分别用于创建HiAI内置业务&模型管家的HIDL Client客户端；
*    2. Mini平台头文件代码：包含AIServiceClient类，用于创建AIModelManager，并调用相应模型加载、推理接口；
*/

#if defined(__ANDROID__) || defined(__HIAIENGINE_TEST__)
#include "ai_client/ai_client_4_lite.h"
#endif // defined(__ANDROID__) || defined(__HIAIENGINE_ST__)

#if !defined(__ANDROID__) || defined(__HIAIENGINE_TEST__)
#include "ai_client/ai_client_4_mini.h"
#endif // !defined(__ANDROID__) || defined(__HIAIENGINE_ST__)

#endif // _AI_CLIENT_H_

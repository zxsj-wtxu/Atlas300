/**
* @file log.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_CONNECTOR_INC_LOG_H_
#define INC_HIAIENGINE_CONNECTOR_INC_LOG_H_

#ifdef __cplusplus
#include <mutex>
#include <map>
#include <string>
#include <stdio.h>
#include <securec.h>
#include "hiaiengine/graph.h"

constexpr int32_t HIAI_TM_BUF_LENGHT = 64;
constexpr int32_t HIAI_TIME_BASE_YEAR = 1900;
constexpr int32_t HIAI_TIME_BASE_MONTH = 1;

namespace hiai {
    class Engine;
    class Graph;
    class GraphImpl;

    class HIAILog {
      public:
        /**
        * @ingroup hiaiengine
        * @brief 获取HIAILog指针
        */
        HIAI_LIB_INTERNAL static HIAILog* GetInstance();

        /**
        * @ingroup hiaiengine
        * @brief : 是否初始化
        * @return : 是否初始化 true 是
        */
        HIAI_LIB_INTERNAL bool IsInit() { return isInitFlag; }

        /**
        * @ingroup hiaiengine
        * @brief 获取log输出级别
        * @return :log输出级别
        */
        HIAI_LIB_INTERNAL uint32_t HIAIGetCurLogLevel();

        /**
        * @ingroup hiaiengine
        * @brief 判断该条log是否需要输出
        * @param [in]errorCode:消息的错误码
        * @return 是否能被输出
        */
        HIAI_LIB_INTERNAL bool HIAILogOutputEnable(const uint32_t errorCode);

        /**
        * @ingroup hiaiengine
        * @brief log对应的级别名称
        * @param [in]logLevel:宏定义的log级别
        */
        HIAI_LIB_INTERNAL std::string HIAILevelName(const uint32_t logLevel);

        /**
        * @ingroup hiaiengine
        * @brief 输出log
        * @param [in]moudleID:enum定义的组件id,
        * @param [in]logLevel:宏定义的log级别,
        * @param [in]strLog:输出的日志内容
        */
        HIAI_LIB_INTERNAL void HIAISaveLog(const int32_t moudleID,
            const uint32_t logLevel, const char* strLog);
      protected:
        /**
        * @ingroup hiaiengine
        * @brief HIAILog构造函数
        */
        HIAI_LIB_INTERNAL HIAILog();
        HIAI_LIB_INTERNAL ~HIAILog() {}
      private:
        /**
        * @ingroup hiaiengine
        * @brief HIAILog初始化函数
        */
        HIAI_LIB_INTERNAL void Init();
        HIAI_LIB_INTERNAL HIAILog(const HIAILog&) = delete;
        HIAI_LIB_INTERNAL HIAILog(HIAILog&&) = delete;
        HIAI_LIB_INTERNAL HIAILog& operator=(const HIAILog&) = delete;
        HIAI_LIB_INTERNAL HIAILog& operator=(HIAILog&&) = delete;
      private:
        HIAI_LIB_INTERNAL static std::mutex mutexHandle;
        uint32_t outputLogLevel;
        std::map<uint32_t, std::string> levelName;
        std::map<std::string, uint32_t> levelNum;
        static bool isInitFlag;
    };
}  // namespace hiai

#define MAX_LOG_STRING  0x2000  // 最长16K字节
#endif // __cplusplus

#define HIAI_ENGINE_LOG(...) \
    HIAI_ENGINE_LOG_IMPL(__FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)


#ifdef __cplusplus
/**
* @ingroup hiaiengine
* @brief 输出log的接口
* @param [in] GraphImpl* GraphImpl对象指针
* @param [in] errorCode 错误码
* @param [in] format log描述
*/
HIAI_LIB_INTERNAL void HIAI_ENGINE_LOG_IMPL(const char* funcPointer,
    const char* filePath, int32_t lineNumber,
    hiai::GraphImpl* graph, const uint32_t errorCode, const char* format, ...);

/**
* @ingroup hiaiengine
* @brief 用户输出log的接口
* @param [in] Graph* Graph对象指针
* @param [in] errorCode 错误码
* @param [in] format log描述
*/
HIAI_LIB_VISIBILITY void HIAI_ENGINE_LOG_IMPL(const char* funcPointer,
    const char* filePath, int32_t lineNumber,
    hiai::Graph* graph, const uint32_t errorCode, const char* format, ...);

/**
* @ingroup hiaiengine
* @brief 用户输出log的接口
* @param [in] Engine* engine对象指针
* @param [in] errorCode 错误码
* @param [in] format log描述
*/
HIAI_LIB_VISIBILITY void HIAI_ENGINE_LOG_IMPL(const char* funcPointer,
    const char* filePath, int32_t lineNumber,
    hiai::Engine* engine, const uint32_t errorCode, const char* format, ...);

/**
* @ingroup hiaiengine
* @brief 用户输出log的接口
* @param [in] errorCode 错误码
* @param [in] format log描述
*/
HIAI_LIB_VISIBILITY void HIAI_ENGINE_LOG_IMPL(const char* funcPointer,
    const char* filePath, int32_t lineNumber,
    const uint32_t errorCode, const char* format, ...);

/**
* @ingroup hiaiengine
* @brief 用户输出log的接口
* @param [in] format log描述
*/
HIAI_LIB_VISIBILITY void HIAI_ENGINE_LOG_IMPL(const char* funcPointer,
    const char* filePath, int32_t lineNumber,
    const char* format, ...);

/**
* @ingroup hiaiengine
* @brief 输出log的接口
* @param [in] moudle_id模块名枚举ID
* @param [in] GraphImpl* graph对象指针
* @param [in] errorCode 错误码
* @param [in] format log描述
*/
HIAI_LIB_INTERNAL void HIAI_ENGINE_LOG_IMPL(const char* funcPointer,
    const char* filePath, int32_t lineNumber,
    const int32_t moudleID, hiai::GraphImpl* graphImpl,
    const uint32_t errorCode, const char* format, ...);

/**
* @ingroup hiaiengine
* @brief 用户输出log的接口
* @param [in] moudle_id模块名枚举ID
* @param [in] Graph* graph对象指针
* @param [in] errorCode 错误码
* @param [in] format log描述
*/
HIAI_LIB_VISIBILITY void HIAI_ENGINE_LOG_IMPL(const char* funcPointer,
    const char* filePath, int32_t lineNumber,
    const int32_t moudleID, hiai::Graph* graph,
    const uint32_t errorCode, const char* format, ...);

/**
* @ingroup hiaiengine
* @brief 用户输出log的接口
* @param [in] moudle_id模块名枚举ID
* @param [in] Engine* engine对象指针
* @param [in] errorCode 错误码
* @param [in] format log描述
*/
HIAI_LIB_VISIBILITY void HIAI_ENGINE_LOG_IMPL(const char* funcPointer,
    const char* filePath, int32_t lineNumber,
    const int32_t moudleID, hiai::Engine* engine, const uint32_t errorCode,
    const char* format, ...);

/**
* @ingroup hiaiengine
* @brief 用户输出log的接口
* @param [in] format log描述
*/
HIAI_LIB_VISIBILITY void HIAI_ENGINE_LOG_IMPL(const char* funcPointer,
    const char* filePath, int32_t lineNumber,
    const int32_t moudleID, const char* format, ...);


#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

/**
* @ingroup hiaiengine
* @brief 用户输出log的接口
* @param [in] moudle_id模块名枚举ID
* @param [in] errorCode 错误码
* @param [in] format log描述
*/
HIAI_LIB_VISIBILITY void HIAI_ENGINE_LOG_IMPL(const char* funcPointer,
    const char* filePath, int32_t lineNumber,
    const int32_t moudleID, const uint32_t errorCode, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif  // INC_HIAIENGINE_CONNECTOR_INC_LOG_H_

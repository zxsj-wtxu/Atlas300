/**
* @file engine.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_ENGINE_H_
#define INC_HIAIENGINE_ENGINE_H_

#include <vector>
#include <algorithm>
#include <string>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "hiaiengine/ai_types.h"
#include "hiaiengine/ai_tensor.h"
#include "hiaiengine/status.h"
#include "hiaiengine/data_recv_interface.h"

namespace hiai {
    // TIME_OUT_VALUE: 60*1000 ms = 60s
    constexpr uint32_t TIME_OUT_VALUE = 60 * 1000;
    constexpr int32_t ENGINE_MAX_QUEUE_SIZE = 100;
    constexpr int32_t QUEUE_START_INDEX = 0;
    constexpr int32_t IN_QUEUE = 0;
    constexpr int32_t OUT_QUEUE = 1;
    constexpr uint32_t REMOVE_MODEL_FILE_FLAG = 0;
    // engine接收数据超时处理机制信息结构体
    struct EngineRecvDataTimeoutInfo {
        uint32_t waitMaxTime;  // 超时时间,单位ms
        bool isRepeatTimeout;  // 是否反复超时
        bool isSingleTimeout;  // 上一次是否收到数据，用于单次超时
    };
    template <typename T>
    class QueueManager;
    class Worker;
    class EngineConfig;
    class GraphImpl;
    struct HIAIEngineProfile;
    struct EngineQueueItem;
    class Engine {
        friend class Graph;
        friend class GraphImpl;
      public:
        /**
        * @ingroup hiaiengine
        * @brief Init 初始化Engine, 供用户继承使用
        * @param [in]config:配置
        * @param [in]modelDesc:模型描述
        */
        HIAI_LIB_INTERNAL virtual HIAI_StatusT Init(const AIConfig &config,
            const std::vector<AIModelDescription>& modelDesc) {
            return HIAI_OK;
        }

        /**
        * @ingroup hiaiengine
        * @brief UpdateConfig 更新配置
        * @param [in]config:配置
        */
        HIAI_LIB_INTERNAL virtual HIAI_StatusT UpdateConfig(
            const AIConfig &config) {
            return HIAI_OK;
        }

        /**
        * @ingroup hiaiengine
        * @brief GetEngineId 获取Engine的ID
        */
        HIAI_LIB_INTERNAL uint32_t GetEngineId() const;

        /**
        * @ingroup hiaiengine
        * @brief GetEngineName， 获取Engine Name
        */
        HIAI_LIB_INTERNAL const std::string& GetEngineName() const;

        /**
        * @ingroup hiaiengine
        * @brief GetGraphId， 获取Graph ID
        */
        HIAI_LIB_VISIBILITY uint32_t GetGraphId() const;

        /** @ingroup hiaiengine_common
        *  @brief   获取ReqestId
        *  @param
        *  @return ReqestId
        */
        HIAI_LIB_INTERNAL uint64_t GetReqId() const;

        /** @ingroup hiaiengine_common
        *  @brief   获取JobId
        *  @param
        *  @return JobId
        */
        HIAI_LIB_INTERNAL uint64_t GetJobId() const;

        /** @ingroup hiaiengine_common
        *  @brief   获取Engine的Queue最大Size
        *  @param
        *  @return 该Engine的Queue最大Size
        */
        HIAI_LIB_VISIBILITY const uint32_t GetQueueMaxSize();

        /** @ingroup hiaiengine_common
        *  @brief   获取Engine的某端口号Queue的Size
        *  @param [in]portID : 端口号
        *  @return 该Engine的portID端口号Queue的Size
        */
        HIAI_LIB_VISIBILITY const uint32_t GetQueueCurrentSize(
            const uint32_t portID);

      protected:
        /**
        * @ingroup hiaiengine
        * @brief Init 初始化Engine， 创建队列等；
        * @param [in]graphIdInput：Engine所属的Graph的编号
        * @param [in]configInput：Engine的配置描述
        * @param [in]jobIdInput：Engine的Job编号
        */
        HIAI_LIB_INTERNAL HIAI_StatusT Init(const uint32_t graphIdInput,
            const EngineConfig& configInput, const uint64_t jobIdInput);

        /**
        * @ingroup hiaiengine
        * @brief Init 初始化Engine， 创建队列等；
        * @param [in]inPortNumInput : 输入端口数目
        * @param [in]outPortNumInput：输出端口数目
        */
        HIAI_LIB_VISIBILITY HIAI_StatusT InitQueue(
            const uint32_t& inPortNumInput,
            const uint32_t& outPortNumInput);

        /**
        * @ingroup hiaiengine
        * @brief Run 线程中执行Run函数， 用户继承使用
        * @param [in]args : 多参数
        */
        HIAI_LIB_INTERNAL virtual HIAI_StatusT Run(
            std::vector<std::shared_ptr<void> >& args) = 0;

        /**
        * @ingroup hiaiengine
        * @brief InitMsgQueue 初始化队列
        * @param
        */
        HIAI_LIB_INTERNAL virtual HIAI_StatusT InitMsgQueue() = 0;

        /**
        * @ingroup hiaiengine
        * @brief 发送数据到指定的Output 端口
        * @param [in] portId : Output端口
        * @param [in] messageName : 消息名称
        * @param [in] dataPtr ： 消息指针
        * @param [in] timeOut ： 超时参数
        */
        HIAI_LIB_VISIBILITY HIAI_StatusT SendData(
            const uint32_t portId,
            const std::string& messageName,
            const std::shared_ptr<void>& dataPtr,
            const uint32_t timeOut = TIME_OUT_VALUE);

        /**
        * @ingroup hiaiengine
        * @brief 接受Graph分发到指定Input端口的数据
        * @param [in] portId:Input端口
        * @param [in] dataPtr:数据指针
        * @param [in] reqId:请求ID
        */
        HIAI_LIB_INTERNAL HIAI_StatusT RecvData(
            const uint32_t portId,
            const std::shared_ptr<void>& dataPtr,
            const uint64_t reqId);

        /**
        * @ingroup hiaiengine
        * @brief 设置写数据回调
        * @param [in] portId:出端口ID
        * @param [in] userDefineDataRecv:DataRecv类
        */
        HIAI_LIB_VISIBILITY HIAI_StatusT SetDataRecvFunctor(
            const uint32_t portId,
            const std::shared_ptr<DataRecvInterface>& userDefineDataRecv);

        /**
        * @ingroup hiaiengine
        * @brief 开始运行线程
        */
        HIAI_LIB_INTERNAL HIAI_StatusT Start();

        /**
        * @ingroup hiaiengine
        * @brief 销毁线程
        */
        HIAI_LIB_INTERNAL HIAI_StatusT Destroy();

        /**
        * @ingroup hiaiengine_common
        * @brief  EngineRun:线程运行函数
        * @param [in] threadIdx ：线程编号
        * @return HIAI_StatusT
        */
        HIAI_LIB_INTERNAL HIAI_StatusT EngineRun(uint32_t threadIdx);

        /**
        * @ingroup hiaiengine
        * @brief 更新Profile数据
        * @param [in] startTime ：开始时间
        * @param [in] endTime ： 结束时间
        */
        HIAI_LIB_INTERNAL HIAI_StatusT UpdateProfileData(
            const uint64_t startTime, const uint64_t endTime);

        /**
        * @ingroup hiaiengine
        * @brief ReSetEngineConfig 重置Engine的配置
        * @param [in]config: 配置信息
        */
        HIAI_LIB_INTERNAL HIAI_StatusT ReSetEngineConfig(
            const AIConfig& config);

        /**
        * @ingroup hiaiengine
        * @brief 设置自动处理时间最大间隔
        * @param [in]msMaxTime超时时间
        */
        HIAI_LIB_INTERNAL void SetWaitInputdataMaxTime(uint32_t msMaxTime);

       /**
        * @ingroup hiaiengine
        * @brief Get Engine this point获取Engine的this指针
        */
        HIAI_LIB_INTERNAL const Engine* GetThis() const;

        /**
        * @ingroup hiaiengine
        * @brief 删除本engine的临时模型文件
        */
        HIAI_LIB_INTERNAL HIAI_StatusT DeleteTmpModelFile();

        /**
        * @ingroup hiaiengine
        * @brief 根据配置文件判断并删除本engine的临时模型文件
        * return HIAI_OK
        */
        HIAI_LIB_INTERNAL HIAI_StatusT CheckandDeleteTmpModelFile();
      public:
        /**
        * @ingroup hiaiengine
        * @brief Engine构造函数
        */
        HIAI_LIB_VISIBILITY Engine();

        /**
        * @ingroup hiaiengine
        * @brief Engine析构函数
        */
        HIAI_LIB_VISIBILITY virtual ~Engine();

      private:
        /**
        * @ingroup hiaiengine
        * @brief 初始化队列管理类
        */
        HIAI_LIB_INTERNAL void InitQueueManagerWithoutCheck();
        /**
        * @ingroup hiaiengine
        * @brief 获取data recv信息
        */
        HIAI_LIB_INTERNAL std::shared_ptr<DataRecvInterface> GetDataRecvInferInMap(const int32_t& portId);
        HIAI_LIB_INTERNAL Engine(const Engine&) = delete;
        HIAI_LIB_INTERNAL Engine(Engine&&) = delete;
        HIAI_LIB_INTERNAL Engine& operator=(const Engine&) = delete;
        HIAI_LIB_INTERNAL Engine& operator=(Engine&&) = delete;
      private:
        bool initFlag_;
        void* profileHandle;
        uint32_t graphID;
        uint32_t inPortNum;
        uint32_t outPortNum;
        std::mutex  mutexHandle;
        std::shared_ptr<EngineConfig> configPtr;
        EngineRecvDataTimeoutInfo recvDataTimeoutInfo_;
        // Out port id和DataRecvInterface的映射表
        std::recursive_mutex dataRecvMapMutex;
        std::map<uint32_t, std::shared_ptr<DataRecvInterface> > dataRecvMap;
        std::shared_ptr<QueueManager<EngineQueueItem>> queueManager;
        std::shared_ptr<Worker> threadWorker;
        thread_local static uint64_t reqID;
        uint64_t jobID;
    };

    struct HIAIEngineProfile {
        HIAIEngineProfile() :
            costTime(0), averageTime(0), runTimes(0) {
        }
        ~HIAIEngineProfile() {}
        uint64_t costTime;
        uint64_t averageTime;
        uint32_t runTimes;
    };

    typedef Engine* (*HIAI_ENGINE_FUNCTOR_CREATOR)();
    class HIAIEngineFactory {
        friend class Graph;
        friend class GraphImpl;
        friend class HIAIEngineAutoRegister;
      private:
        HIAI_LIB_INTERNAL HIAIEngineFactory() {}
        HIAI_LIB_INTERNAL ~HIAIEngineFactory() {}
        HIAI_LIB_INTERNAL HIAIEngineFactory(
            const HIAIEngineFactory&) = delete;
        HIAI_LIB_INTERNAL HIAIEngineFactory(
            HIAIEngineFactory&&) = delete;
        HIAI_LIB_INTERNAL HIAIEngineFactory& operator=(
            const HIAIEngineFactory&) = delete;
        HIAI_LIB_INTERNAL HIAIEngineFactory& operator=(
            HIAIEngineFactory&&) = delete;
      protected:
        /**
        * @ingroup hiaiengine
        * @brief GetInstance():获取h单例
        * @param [in]
        */
        HIAI_LIB_VISIBILITY static HIAIEngineFactory* GetInstance();

        /**
        * @ingroup hiaiengine
        * @brief RegisterEngineCreator():注册Engine构造函数
        * @param [in] engineName: Engine名称
        * @param [in] engineCreatorFunc: Engine Functor Creator
        */
        HIAI_LIB_VISIBILITY HIAI_StatusT RegisterEngineCreator(
            const std::string& engineName,
            HIAI_ENGINE_FUNCTOR_CREATOR engineCreatorFunc);

        /**
        * @ingroup hiaiengine
        * @brief UnRegisterEngineCreator()：注销Engine构造函数
        * @param [in] engineName: Engine名称
        */
        HIAI_LIB_VISIBILITY HIAI_StatusT UnRegisterEngineCreator(
            const std::string& engineName);

        /**
        * @ingroup hiaiengine
        * @brief GetEngine():创建Engine
        * @param [in] engineName: Engine名称
        * return : Engine的智能指针
        */
        HIAI_LIB_INTERNAL std::shared_ptr<hiai::Engine> CreateEngine(
            const std::string& engineName);

    private:
        std::map<std::string, HIAI_ENGINE_FUNCTOR_CREATOR> functorMap_;
        HIAI_LIB_INTERNAL static std::mutex engineFactoryMutex_;
    };

    // HIAIEngineAutoRegister
    class HIAIEngineAutoRegister {
      public:
        /**
        * @ingroup hiaiengine
        * @brief HIAIEngineAutoRegister():EngineAuto Register
        * @param [in] engineName: Engine名称
        * @param [in] func: Engine注册函数
        */
        HIAI_LIB_INTERNAL explicit HIAIEngineAutoRegister(
            const std::string& engineName,
            HIAI_ENGINE_FUNCTOR_CREATOR func) :
            engineName_(engineName),
            status_(HIAI_OK) {
            status_ = HIAIEngineFactory::GetInstance()->
                RegisterEngineCreator(engineName, func);
        }

        /**
        * @ingroup hiaiengine
        * @brief ~HIAIEngineAutoRegister():EngineAuto 析构
        */
        HIAI_LIB_INTERNAL ~HIAIEngineAutoRegister() {
            if (status_ == HIAI_OK) {
                HIAIEngineFactory::GetInstance()->
                    UnRegisterEngineCreator(engineName_);
            }
        }
      private:
        HIAI_LIB_INTERNAL HIAIEngineAutoRegister(
            const HIAIEngineAutoRegister&) = delete;
        HIAI_LIB_INTERNAL HIAIEngineAutoRegister(
            HIAIEngineAutoRegister&&) = delete;
        HIAI_LIB_INTERNAL HIAIEngineAutoRegister& operator=(
            const HIAIEngineAutoRegister&) = delete;
        HIAI_LIB_VISIBILITY HIAIEngineAutoRegister& operator=(
            HIAIEngineAutoRegister&&) = delete;
      private:
        std::string engineName_;
        HIAI_StatusT status_;
    };
}

#define HIAI_INPUT_ARGS_1 std::shared_ptr<void>&arg0

#define HIAI_INPUT_ARGS_2 std::shared_ptr<void>&arg0,   \
                          std::shared_ptr<void>&arg1

#define HIAI_INPUT_ARGS_3 std::shared_ptr<void>&arg0,   \
                          std::shared_ptr<void>&arg1,   \
                          std::shared_ptr<void>&arg2

#define HIAI_INPUT_ARGS_4 std::shared_ptr<void>&arg0,   \
                          std::shared_ptr<void>&arg1,   \
                          std::shared_ptr<void>&arg2,   \
                          std::shared_ptr<void>&arg3

#define HIAI_INPUT_ARGS_5 std::shared_ptr<void>&arg0,   \
                          std::shared_ptr<void>&arg1,   \
                          std::shared_ptr<void>&arg2,   \
                          std::shared_ptr<void>&arg3,   \
                          std::shared_ptr<void>&arg4

#define HIAI_INPUT_ARGS_6 std::shared_ptr<void>&arg0,   \
                          std::shared_ptr<void>&arg1,   \
                          std::shared_ptr<void>&arg2,   \
                          std::shared_ptr<void>&arg3,   \
                          std::shared_ptr<void>&arg4,   \
                          std::shared_ptr<void>&arg5

#define HIAI_INPUT_ARGS_7 std::shared_ptr<void>&arg0,   \
                          std::shared_ptr<void>&arg1,   \
                          std::shared_ptr<void>&arg2,   \
                          std::shared_ptr<void>&arg3,   \
                          std::shared_ptr<void>&arg4,   \
                          std::shared_ptr<void>&arg5,   \
                          std::shared_ptr<void>&arg6

#define HIAI_INPUT_ARGS_8 std::shared_ptr<void>&arg0,   \
                          std::shared_ptr<void>&arg1,   \
                          std::shared_ptr<void>&arg2,   \
                          std::shared_ptr<void>&arg3,   \
                          std::shared_ptr<void>&arg4,   \
                          std::shared_ptr<void>&arg5,   \
                          std::shared_ptr<void>&arg6,   \
                          std::shared_ptr<void>&arg7

#define HIAI_INPUT_ARGS_9 std::shared_ptr<void>&arg0,   \
                          std::shared_ptr<void>&arg1,   \
                          std::shared_ptr<void>&arg2,   \
                          std::shared_ptr<void>&arg3,   \
                          std::shared_ptr<void>&arg4,   \
                          std::shared_ptr<void>&arg5,   \
                          std::shared_ptr<void>&arg6,   \
                          std::shared_ptr<void>&arg7,   \
                          std::shared_ptr<void>&arg8

#define HIAI_INPUT_ARGS_10 std::shared_ptr<void>&arg0,  \
                           std::shared_ptr<void>&arg1,  \
                           std::shared_ptr<void>&arg2,  \
                           std::shared_ptr<void>&arg3,  \
                           std::shared_ptr<void>&arg4,  \
                           std::shared_ptr<void>&arg5,  \
                           std::shared_ptr<void>&arg6,  \
                           std::shared_ptr<void>&arg7,  \
                           std::shared_ptr<void>&arg8,  \
                           std::shared_ptr<void>&arg9

#define HIAI_INPUT_ARGS_11 std::shared_ptr<void>&arg0,  \
                           std::shared_ptr<void>&arg1,  \
                           std::shared_ptr<void>&arg2,  \
                           std::shared_ptr<void>&arg3,  \
                           std::shared_ptr<void>&arg4,  \
                           std::shared_ptr<void>&arg5,  \
                           std::shared_ptr<void>&arg6,  \
                           std::shared_ptr<void>&arg7,  \
                           std::shared_ptr<void>&arg8,  \
                           std::shared_ptr<void>&arg9,  \
                           std::shared_ptr<void>&arg10

#define HIAI_INPUT_ARGS_12 std::shared_ptr<void>&arg0,  \
                           std::shared_ptr<void>&arg1,  \
                           std::shared_ptr<void>&arg2,  \
                           std::shared_ptr<void>&arg3,  \
                           std::shared_ptr<void>&arg4,  \
                           std::shared_ptr<void>&arg5,  \
                           std::shared_ptr<void>&arg6,  \
                           std::shared_ptr<void>&arg7,  \
                           std::shared_ptr<void>&arg8,  \
                           std::shared_ptr<void>&arg9,  \
                           std::shared_ptr<void>&arg10, \
                           std::shared_ptr<void>&arg11

#define HIAI_INPUT_ARGS_13 std::shared_ptr<void>&arg0,  \
                           std::shared_ptr<void>&arg1,  \
                           std::shared_ptr<void>&arg2,  \
                           std::shared_ptr<void>&arg3,  \
                           std::shared_ptr<void>&arg4,  \
                           std::shared_ptr<void>&arg5,  \
                           std::shared_ptr<void>&arg6,  \
                           std::shared_ptr<void>&arg7,  \
                           std::shared_ptr<void>&arg8,  \
                           std::shared_ptr<void>&arg9,  \
                           std::shared_ptr<void>&arg10, \
                           std::shared_ptr<void>&arg11, \
                           std::shared_ptr<void>&arg12

#define HIAI_INPUT_ARGS_14 std::shared_ptr<void>&arg0,  \
                           std::shared_ptr<void>&arg1,  \
                           std::shared_ptr<void>&arg2,  \
                           std::shared_ptr<void>&arg3,  \
                           std::shared_ptr<void>&arg4,  \
                           std::shared_ptr<void>&arg5,  \
                           std::shared_ptr<void>&arg6,  \
                           std::shared_ptr<void>&arg7,  \
                           std::shared_ptr<void>&arg8,  \
                           std::shared_ptr<void>&arg9,  \
                           std::shared_ptr<void>&arg10, \
                           std::shared_ptr<void>&arg11, \
                           std::shared_ptr<void>&arg12, \
                           std::shared_ptr<void>&arg13

#define HIAI_INPUT_ARGS_15 std::shared_ptr<void>&arg0,  \
                           std::shared_ptr<void>&arg1,  \
                           std::shared_ptr<void>&arg2,  \
                           std::shared_ptr<void>&arg3,  \
                           std::shared_ptr<void>&arg4,  \
                           std::shared_ptr<void>&arg5,  \
                           std::shared_ptr<void>&arg6,  \
                           std::shared_ptr<void>&arg7,  \
                           std::shared_ptr<void>&arg8,  \
                           std::shared_ptr<void>&arg9,  \
                           std::shared_ptr<void>&arg10, \
                           std::shared_ptr<void>&arg11, \
                           std::shared_ptr<void>&arg12, \
                           std::shared_ptr<void>&arg13, \
                           std::shared_ptr<void>&arg14,

#define HIAI_INPUT_ARGS_16 std::shared_ptr<void>&arg0,  \
                           std::shared_ptr<void>&arg1,  \
                           std::shared_ptr<void>&arg2,  \
                           std::shared_ptr<void>&arg3,  \
                           std::shared_ptr<void>&arg4,  \
                           std::shared_ptr<void>&arg5,  \
                           std::shared_ptr<void>&arg6,  \
                           std::shared_ptr<void>&arg7,  \
                           std::shared_ptr<void>&arg8,  \
                           std::shared_ptr<void>&arg9,  \
                           std::shared_ptr<void>&arg10, \
                           std::shared_ptr<void>&arg11, \
                           std::shared_ptr<void>&arg12, \
                           std::shared_ptr<void>&arg13, \
                           std::shared_ptr<void>&arg14, \
                           std::shared_ptr<void>&arg15

#define HIAI_ARGS_LIST_1 args[0]

#define HIAI_ARGS_LIST_2 args[0], args[1]

#define HIAI_ARGS_LIST_3 args[0], args[1], args[2]

#define HIAI_ARGS_LIST_4 args[0], args[1], args[2],     \
                         args[3]

#define HIAI_ARGS_LIST_5 args[0], args[1], args[2],     \
                         args[3], args[4]

#define HIAI_ARGS_LIST_6 args[0], args[1], args[2],     \
                         args[3], args[4], args[5]

#define HIAI_ARGS_LIST_7 args[0], args[1], args[2],     \
                         args[3], args[4], args[5],     \
                         args[6]

#define HIAI_ARGS_LIST_8 args[0], args[1], args[2],     \
                         args[3], args[4], args[5],     \
                         args[6], args[7]

#define HIAI_ARGS_LIST_9 args[0], args[1], args[2],     \
                         args[3], args[4], args[5],     \
                         args[6], args[7], args[8]

#define HIAI_ARGS_LIST_10 args[0], args[1], args[2],    \
                          args[3], args[4], args[5],    \
                          args[6], args[7], args[8],    \
                          args[9]

#define HIAI_ARGS_LIST_11 args[0], args[1], args[2],    \
                          args[3], args[4], args[5],    \
                          args[6], args[7], args[8],    \
                          args[9], args[10]

#define HIAI_ARGS_LIST_12 args[0], args[1], args[2],    \
                          args[3], args[4], args[5],    \
                          args[6], args[7], args[8],    \
                          args[9], args[10], args[11]

#define HIAI_ARGS_LIST_13 args[0], args[1], args[2],    \
                          args[3], args[4], args[5],    \
                          args[6], args[7], args[8],    \
                          args[9], args[10], args[11],  \
                          args[12]

#define HIAI_ARGS_LIST_14 args[0], args[1], args[2],    \
                          args[3], args[4], args[5],    \
                          args[6], args[7], args[8],    \
                          args[9], args[10], args[11],  \
                          args[12], args[13]

#define HIAI_ARGS_LIST_15 args[0], args[1], args[2],    \
                          args[3], args[4], args[5],    \
                          args[6], args[7], args[8],    \
                          args[9], args[10], args[11],  \
                          args[12], args[13], args[14]

#define HIAI_ARGS_LIST_16 args[0], args[1], args[2],    \
                          args[3], args[4], args[5],    \
                          args[6], args[7], args[8],    \
                          args[9], args[10], args[11],  \
                          args[12], args[13], args[14], \
                          args[15]

#define HIAI_ARGS_LIST(n) HIAI_ARGS_LIST_##n
#define HIAI_INPUT_ARGS(n) HIAI_INPUT_ARGS_##n
#define HIAI_ENGINE_NAME(name) HIAIEngine##name

// Define HIAI Engine
/**
* @marco：HIAI_IMPL_ENGINE_PROCESS
* @brief 预编译接口[注册Engine类体]：放到用户自定义Engine的类体声明
* @param [in]inPortNumInput:定义该Engine的输入端口数量
* @param [in]outPortNumInput:定义该Engine的输出端口数量
*/
#define HIAI_DEFINE_PROCESS(inPortNumInput, outPortNumInput)        \
  protected:                                                        \
    HIAI_StatusT InitMsgQueue() {                                   \
      return  InitQueue(inPortNumInput, outPortNumInput);           \
    }                                                               \
    HIAI_StatusT Run(std::vector<std::shared_ptr<void> >& args) {   \
        if (args.size() < inPortNumInput) {                         \
            return HIAI_ENGINE_RUN_ARGS_NOT_RIGHT;                  \
        }                                                           \
        return Process(HIAI_ARGS_LIST(inPortNumInput));             \
    }                                                               \
    HIAI_StatusT Process(HIAI_INPUT_ARGS(inPortNumInput));

// HIAI PROCESS
// Auto Register
#define HIAI_REGISTER_ENGINE_UNIQUE(name, engineClass, ctr)         \
hiai::Engine* HIAI_Engine_##engineClass##_##ctr##_Creator() {       \
    return new(std::nothrow) engineClass;                           \
}                                                                   \
hiai::HIAIEngineAutoRegister                                        \
g_engine_##engineClass##_##ctr##_registor(                          \
    name, HIAI_Engine_##engineClass##_##ctr##_Creator);

/**
* @marco：HIAI_IMPL_ENGINE_PROCESS
* @brief 预编译接口[注册Engine类消息处理]：放到用户自定义Engine的类实现中
* @param [in]name:用户定义Engine的名称字符串
* @param [in]engineClass:用户定义Engine的类名
* @param [in]inPortNumInput:该Engine的输入端口数量
*/
#define HIAI_IMPL_ENGINE_PROCESS(name, engineClass, inPortNumInput) \
HIAI_REGISTER_ENGINE_UNIQUE(name, engineClass, __COUNTER__)         \
HIAI_StatusT engineClass::Process(HIAI_INPUT_ARGS(inPortNumInput))

#endif  // INC_HIAIENGINE_ENGINE_H_


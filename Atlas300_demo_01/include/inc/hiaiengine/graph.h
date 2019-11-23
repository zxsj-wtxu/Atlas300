/**
* @file graph.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_GRAPH_H_
#define INC_HIAIENGINE_GRAPH_H_

#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <memory>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "proto/ai_types.pb.h"
#include "hiaiengine/status.h"

namespace hiai {
    constexpr uint32_t SEND_DATA_DEFAULT_TIME_OUT = 100000;

    struct EnginePortID {
        uint32_t graph_id;
        uint32_t engine_id;
        uint32_t port_id;
    };

    typedef enum {
        HIAI_DEVICE_DISCONNECT_EVENT  // 断开消息
    }HIAIEvent;

    class GraphImpl;
    class GraphConfigList;
    class DataRecvInterface;
    class Engine;

    class Graph {
      public:
        /**
        * @ingroup hiaiengine
        * @brief 供用户设置签名加密公钥，需要在创建Graph之前调用
        * @param [in] publicKey:公钥
        * @return
        */
        HIAI_LIB_VISIBILITY static HIAI_StatusT SetPublicKeyForSignatureEncryption(
            const std::string& publicKey);
        /**
        * @ingroup hiaiengine
        * @brief 静态函数，根据id获取Graph对象
        * @param [in]graphID:目标graph的id
        * @return Graph的指针
        */
        HIAI_LIB_VISIBILITY static std::shared_ptr<Graph> GetInstance(
            uint32_t graphID);

        /**
        * @ingroup hiaiengine
        * @brief 静态函数，根据配置参数创建新的Graph
        * @param [in]graphConfig:配置参数
        * @return 错误码
        */
        HIAI_LIB_HOST_VISIBILITY static HIAI_StatusT CreateGraph(
            const GraphConfigList& graphConfig);
        /**
        * @ingroup hiaiengine
        * @brief 静态函数，解析配置文件
        * @param [in]configFile:配置文件名 [out]graphConfigList:配置参数
        * @return 错误码
        */
        HIAI_LIB_VISIBILITY static HIAI_StatusT ParseConfigFile(
            const std::string& configFile, GraphConfigList& graphConfigList);
        /**
        * @ingroup hiaiengine
        * @brief 静态函数，根据配置文件创建新的Graph
        * @param [in]configFile:配置文件名
        * @return 错误码
        */
        HIAI_LIB_HOST_VISIBILITY static HIAI_StatusT CreateGraph(
            const std::string& configFile);
        /**
        * @ingroup hiaiengine
        * @brief 静态函数，根据id销毁指定的Graph对象
        * @param [in]graphID:目标graph的id
        * @return 错误码
        */
        HIAI_LIB_HOST_VISIBILITY static HIAI_StatusT DestroyGraph(uint32_t graphID);
            
        /**
        * @ingroup hiaiengine
        * @brief 静态函数，设置写数据回调
        * @param [in] targetPortConfig:指定engine的端口信息
        * @param [in] dataRecv:DataRecv类，回调函数对象
        */
        HIAI_LIB_VISIBILITY static HIAI_StatusT SetDataRecvFunctor(
            const EnginePortID& targetPortConfig,
            const std::shared_ptr<DataRecvInterface>& dataRecv);

        /**
        * @ingroup hiaiengine
        * @brief 发送void类型的数据到指定的端口
        * @param [in]targetPortConfig:目标端口的配置
        * @param [in]messageName:消息的名称
        * @param [in]dataPtr:消息的指针
        * @param [in]timeOut:超时等待时间
        * @return 错误码
        */
        HIAI_LIB_VISIBILITY HIAI_StatusT  SendData(
            const EnginePortID& targetPortConfig,
            const std::string& messageName,
            const std::shared_ptr<void>& dataPtr,
            const uint32_t timeOut = SEND_DATA_DEFAULT_TIME_OUT);

        /**
        * @ingroup hiaiengine
        * @brief 根据id查找engine对象
        * @param [in]engineID:目标engine的id
        * @return engine的智能指针
        */
        HIAI_LIB_VISIBILITY std::shared_ptr<Engine> GetEngine(uint32_t engineID);

        /**
        * @ingroup hiaiengine
        * @brief 获取graph的id
        * @param 无
        * @return graph的id
        */
        HIAI_LIB_VISIBILITY uint32_t GetGraphId();

        /**
        * @ingroup hiaiengine
        * @brief 通知对端释放内存
        * @param [in] ptr:指针
        * @return
        */
        HIAI_LIB_VISIBILITY static void ReleaseDataBuffer(void* ptr);

        /**
        * @ingroup hiaiengine
        * @brief 构造函数
        * @param 无
        * @return 无
        */
        HIAI_LIB_INTERNAL Graph();
        /**
        * @ingroup hiaiengine
        * @brief 析构函数
        */
        HIAI_LIB_INTERNAL ~Graph();

        /**
        * @ingroup hiaiengine
        * @brief UpdateEngineConfig 更新EngineConfig
        * @param [in] graphId： graphID
        * @param [in] engineId: engine id
        * @param [in] aiConfig: 配置文件
        * @param [in] syncFlag: 是否同步
        * @return 成功HIAI_OK
        */
        HIAI_LIB_VISIBILITY static HIAI_StatusT UpdateEngineConfig(
            const uint32_t& graphId,
            const uint32_t& engineId,
            const AIConfig& aiConfig,
            const bool& syncFlag = false);
        /**
        * @ingroup hiaiengine
        * @brief 获取device id
        * @param [in]
        * @return device id
        */
        HIAI_LIB_VISIBILITY uint32_t GetDeviceID();

        /**
        * @ingroup hiaiengine
        * @brief 设置Impl实例智能指针
        * @param[in] Impl实例智能指针
        */
        HIAI_LIB_INTERNAL void SetGraphImpl(std::shared_ptr<GraphImpl>& graphImpl);

        /**
        * @ingroup hiaiengine
        * @brief   RegisterEventHandle 注册Event事件，用户通过该接口订阅感兴趣的事件
        * @param   [in]  event    : Event回调事件
        * @param   [in]  callBack : 用户注册回调方法
        * @return HIAI_StatusT : 正确值为HIAI_OK,其他则为异常
        */
        HIAI_LIB_VISIBILITY HIAI_StatusT RegisterEventHandle(
                const HIAIEvent& event,
                const std::function<HIAI_StatusT(void)>& callBack);

        HIAI_LIB_INTERNAL Graph(const Graph&) = delete;
        HIAI_LIB_INTERNAL Graph(Graph&&) = delete;
        HIAI_LIB_INTERNAL Graph& operator=(const Graph&) = delete;
        HIAI_LIB_INTERNAL Graph& operator=(Graph&&) = delete;
        /**
        * @ingroup hiaiengine
        * @brief 获取Impl实例智能指针
        * @return Impl实例智能指针
        */
        HIAI_LIB_INTERNAL std::shared_ptr<GraphImpl> GetGraphImpl();
      private:
        std::weak_ptr<GraphImpl> graphImpl_;
    };
}
#endif  // INC_HIAIENGINE_GRAPH_H_

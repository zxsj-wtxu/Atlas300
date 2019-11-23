/**
* @file c_graph.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_C_GRAPH_H_
#define INC_HIAIENGINE_C_GRAPH_H_

#include <stddef.h>
#include "status.h"

#ifdef __cplusplus
    #include <string>
    #include <memory>
#endif  // __cplusplus


typedef struct HIAI_PortID {
    uint32_t graph_id;
    uint32_t engine_id;
    uint32_t port_id;
} HIAI_PortID_t;

typedef HIAI_StatusT (*HIAI_RecvDataCallbackT)(void*);

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#ifdef __cplusplus
    /**
    * @ingroup hiaiengine
    * @brief C++语言发送数据到指定的端口
    * @param [in]targetPortConfig:目标端口的配置
    * @param [in]messageName:消息的名称
    * @param [in]dataPtr:消息的指针，由graph负责释放
    * @param [in]timeOut:超时等待时间
    * @return 错误码
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_SendData(
        HIAI_PortID_t targetPortConfig,
        const std::string& messageName, std::shared_ptr<void>& dataPtr,
        const uint32_t timeOut = 0);
#endif  // __cplusplus
    /**
    * @ingroup hiaiengine
    * @brief C语言发送数据到指定的端口
    * @param [in]targetPortConfig:目标端口的配置
    * @param [in]messageName:消息的名称
    * @param [in]len:消息的长度
    * @param [in]dataPtr:消息的指针，必须由调用者释放。
    * @return 错误码
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_C_SendData(
        HIAI_PortID_t targetPortConfig, const char* messageName,
        size_t len, void* dataPtr);
    /**
    * @ingroup hiaiengine
    * @brief 根据配置文件创建新的Graph
    * @param [in]configFile:配置文件名
    * @param [in]len:配置文件名的长度
    * @return 错误码
    */
    HIAI_LIB_HOST_VISIBILITY HIAI_StatusT HIAI_CreateGraph(
        const char* configFile, size_t len);
    /**
    * @ingroup hiaiengine
    * @brief 静态函数，根据id销毁指定的Graph对象
    * @param [in]graphID:目标graph的id
    * @return 错误码
    */
    HIAI_LIB_HOST_VISIBILITY HIAI_StatusT HIAI_DestroyGraph(uint32_t graphID);

    /**
    * @ingroup hiaiengine
    * @brief 设置写数据回调
    * @param [in]targetPortConfig:目标端口的配置
    * @param [in] HIAI_RecvDataCallbackT:recvCallback回调函数
    * @return 错误码
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_SetDataRecvFunctor(
        HIAI_PortID_t targetPortConfig, HIAI_RecvDataCallbackT recvCallback);

    /**
    * @ingroup hiaiengine
    * @brief 获取第一个graph的id
    * @param [out]firstGraphID:第一个graph的id
    * @return 成功HIAI_OK,失败HIAI_GRAPH_GET_GraphId_ERROR
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_GetFirstGraphId(
        uint32_t* firstGraphID);

    /**
    * @ingroup hiaiengine
    * @brief 获取下一个graph的id
    * @param [in]cur_graph_id当前graph id,
    * @param [out]nextGraphID:下一个graph的id
    * @return 成功HIAI_OK,失败HIAI_GRAPH_GET_GraphId_ERROR
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_GetNextGraphId(
                const uint32_t curGraphID, uint32_t* nextGraphID);

    /**
    * @ingroup hiaiengine
    * @brief 获取hiaiengine的版本信息
    * @param 无
    * @return 返回api版本信息
    */
    HIAI_LIB_VISIBILITY HIAI_API_VERSION HIAI_GetAPIVersion();

    /**
    * @ingroup hiaiengine
    * @brief 获取device个数
    * @param [out]devCount:返回device的个数
    * @return 成功HIAI_OK,失败HIAI_GRAPH_GET_DEVNUM_ERROR
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_GetDeviceNum(uint32_t* devCount);

    /**
    * @ingroup hiaiengine
    * @brief 获取第一个device的id
    * @param [out]firstDevID:返回第一个device的id
    * @return 成功HIAI_OK,失败HIAI_GRAPH_GET_DEVID_ERROR
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_GetFirstDeviceId(
        uint32_t* firstDevID);

    /**
    * @ingroup hiaiengine
    * @brief 获取第下一个device的id
    * @param [in]curDevID:当前device id,
    * @param [out]nextDevID:返回下一个device的id
    * @return 成功HIAI_OK,失败HIAI_GRAPH_GET_DEVID_ERROR
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_GetNextDeviceId(
        const uint32_t curDevID, uint32_t* nextDevID);

    /**
    * @ingroup hiaiengine
    * @brief 获取PCIe的info
    * @param [in]需要查询的devId,
    * @param [out]bus : pcie的info
    * @param [out]dev : pcie的info
    * @param [out]func : pcie的info
    * @return 成功HIAI_OK,失败HIAI_GRAPH_GET_PCIEINFO_ERROR
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_GetPCIeInfo(const uint32_t devId,
         int32_t* bus, int32_t* dev, int32_t* func);
    /**
    * @ingroup HIAI
    * @brief HIAI_DMalloc  :  从内存池获取相应大小的内存块
    * @param [in] data_size: 分配内存的大小
    * @param [in] time_out : 时延
    * @param [in] flag:为0（默认值），则分配普通内存。
    * 若flag为HIAI_MEMORY_HUGE_PAGE则从大页中分配
    * @return 分配的内存指针
    */
    HIAI_LIB_VISIBILITY void* HIAI_DMalloc(const uint32_t dataSize,
        const uint32_t timeOut, uint32_t flag);

    /**
    * @ingroup HIAI
    * @brief HIAI_DFree       : 释放从内存池中获取的内存
    * @param [in] data_buffer : 内存指针
    * @return HIAI_StatusT
    */
    HIAI_LIB_VISIBILITY HIAI_StatusT HIAI_DFree(void* dataBuffer);

    /**
    * @ingroup HIAI
    * @brief HIAI_DVPP_DMalloc  :  从内存池获取相应大小的内存块（4G地址内）
    * @param [in] data_size: 分配内存的大小
    * @return 分配的内存指针
    */
    HIAI_LIB_DEVICE_VISIBILITY void* HIAI_DVPP_DMalloc(const uint32_t dataSize);

    /**
    * @ingroup HIAI
    * @brief HIAI_DVPP_DFree: 释放通过HIAI_DVPP_DMalloc接口获取的内存
    * @param [in] dataBuffer: 内存指针
    * @return HIAI_OK成功，或者其他错误码
    */
    HIAI_LIB_DEVICE_VISIBILITY HIAI_StatusT HIAI_DVPP_DFree(void* dataBuffer);

    #ifdef __cplusplus
}
    #endif  // __cplusplus

#endif  // INC_HIAIENGINE_C_GRAPH_H_


/**
* @file power_state.h
*
* Copyright (C) Huawei Technologies Co., Ltd. 2018-2019. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_POWER_STATE_H_
#define INC_HIAIENGINE_POWER_STATE_H_

#include <set>

#include "hiaiengine/status.h"

namespace hiai {

    // 低功耗相关状态上报和处理 
    class PowerState {
      public:
        typedef enum {
            DEVICE_POWER_STATE_S0 = 0,       // 系统进入S0，设备可用，唤醒APP
            DEVICE_POWER_STATE_SUSPEND,      // 系统即将进入SUSPEND，APP释放通道、内存等资源
            DEVICE_POWER_STATE_D0,           // NPU设备进入D0，设备可用，唤醒APP（matrix业务无需处理）
            DEVICE_POWER_STATE_D3,           // NPU设备即将进入D3，APP停止与device侧通信（matrix业务无需处理）
            DEVICE_POWER_STATE_ENABLE,       // NPU设备驱动安装/启用, NPU可用
            DEVICE_POWER_STATE_DISABLE,      // NPU设备驱动卸载/禁用, NPU不可用
            DEVICE_POWER_STATE_MAX
        } DevicePowerStateT;

        // 通知用户NPU状态改变的回调函数类型
        typedef HIAI_StatusT (*PowerStateNotifyCallbackT)(uint32_t, DevicePowerStateT);

        /**
        * @ingroup hiaiengine
        * @brief GetInstance():获取单实例
        * @return PowerState实例的指针
        */
        HIAI_LIB_VISIBILITY static PowerState* GetInstance();

        /**
        * @ingroup hiaiengine
        * @brief 注册NPU状态改变时通知到用户侧的回调函数
        * @param [in]notifyCallback:用户的回调函数指针
        * @return：成功返回HIAI_OK，否则返回具体错误码
        */
        HIAI_LIB_VISIBILITY HIAI_StatusT SubscribePowerStateEvent(
            const PowerStateNotifyCallbackT& notifyCallback);

        /**
        * @ingroup hiaiengine
        * @brief 取消注册NPU状态改变时通知到用户侧的回调函数
        * @param [in]notifyCallback:用户的回调函数指针
        * @return：成功返回HIAI_OK，否则返回具体错误码
        */
        HIAI_LIB_VISIBILITY HIAI_StatusT UnsubscribePowerStateEvent(
            const PowerStateNotifyCallbackT& notifyCallback);

        /**
        * @ingroup hiaiengine
        * @brief Init():初始化PowerState
        * @return：成功返回HIAI_OK，否则返回具体错误码
        */
        HIAI_LIB_INTERNAL HIAI_StatusT Init();

        /**
        * @ingroup hiaiengine
        * @brief   DevicePowerStateChangeProc 上报统一处理函数
        * @param   [in] deviceId :deviceId
        * @param   [in] state :上报的低功耗模式 依据枚举DEVICE_POWER_STATE
        * @return：成功返回HIAI_OK，否则返回具体错误码
        */
        HIAI_LIB_INTERNAL HIAI_StatusT DevicePowerStateChangeProc(const uint32_t& deviceId,
            const DevicePowerStateT& state);

        /**
        * @ingroup hiaiengine
        * @brief 获取当前device状态
        * @return 代表Device状态的bool值
        */
        HIAI_LIB_INTERNAL bool DeviceIsPowerOn();

    private:
        HIAI_LIB_INTERNAL PowerState() : 
            devicePowerOn_(true) {}
        HIAI_LIB_INTERNAL ~PowerState() {}

        /**
        * @ingroup hiaiengine
        * @brief：对赋值和构造无意义的操作需要禁止
        */
        HIAI_LIB_INTERNAL PowerState(
            const PowerState&) = delete;
        HIAI_LIB_INTERNAL PowerState(
            PowerState&&) = delete;
        HIAI_LIB_INTERNAL PowerState& operator=(
            const PowerState&) = delete;
        HIAI_LIB_INTERNAL PowerState& operator=(
            PowerState&&) = delete;

        /**
        * @ingroup hiaiengine
        * @brief   RefreshDeviceState 记录休眠的Device的ID
        * @param   [in] deviceNewState : 当前Device状态
        * return   Status成功HIAIOK，失败：其他错误码
        */
        HIAI_LIB_INTERNAL HIAI_StatusT RefreshDeviceState(const DevicePowerStateT& deviceNewState);

        /**
        * @ingroup hiaiengine
        * @brief   NotifyDeviceChange 状态上报给上层
        * @param   [in] deviceId :deviceId
        * @param   [in] drvState :驱动上报的低功耗模式
        * return   Status成功HIAIOK，失败：其他错误码
        */
        HIAI_LIB_INTERNAL HIAI_StatusT NotifyDeviceChange(const uint32_t& deviceId,
            const DevicePowerStateT& drvState);

        /**
        * @ingroup hiaiengine
        * @brief   RegisterCallbackToDriver 注册低功耗处理函数,提供给上层使用
        * return   Status成功HIAIOK，失败：其他错误码
        */
        HIAI_LIB_INTERNAL HIAI_StatusT RegisterCallbackToDriver();

        /**
        * @ingroup hiaiengine
        * @brief   GetFirstNotifyCallback 获取第一个app回调函数的指针
        * return   获取到的app回调函数的指针 或者 是nullptr
        */
        HIAI_LIB_INTERNAL PowerStateNotifyCallbackT GetFirstNotifyCallback();

        /**
        * @ingroup hiaiengine
        * @brief   GetNextNotifyCallback 获取下一个回调函数的指针
        * @param   [in] callBackFunc : 回调函数的指针
        * return   获取到的app回调函数的指针 或者 是nullptr
        */
        HIAI_LIB_INTERNAL PowerStateNotifyCallbackT GetNextNotifyCallback(
            const PowerStateNotifyCallbackT& callBackFunc);

    private:
        std::set<PowerStateNotifyCallbackT> powerStateNotifyFunc_; // 用户注册的回调函数
        std::mutex notifyFuncMutex_;
        bool devicePowerOn_;  // 记录系统是否进入休眠
        std::mutex deviceStateMutex_;
    };
}

#endif  // INC_HIAIENGINE_POWER_STATE_H_

/**
* @file data_recv_interface.h
*
* Copyright (C) Huawei Technologies Co., Ltd. 2018-2019. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_DATA_RECV_INTERFACE_H_
#define INC_HIAIENGINE_DATA_RECV_INTERFACE_H_

#include <iostream>
#include <memory>
#include <functional>
#include "hiaiengine/status.h"

namespace hiai {
    // 接收数据的回调接口
    class DataRecvInterface {
      public:
        HIAI_LIB_INTERNAL DataRecvInterface() {}
        HIAI_LIB_INTERNAL virtual ~DataRecvInterface() {}
        /**
        * @ingroup hiaiengine
        * @brief 用于接收数据回调接口
        * @param [in] message 输入数据
        * @return HIAI Status：成功返回HIAI_OK，否则返回具体出错码
        */
        HIAI_LIB_VISIBILITY virtual HIAI_StatusT RecvData(
            const std::shared_ptr<void>& message) = 0;
      private:
        HIAI_LIB_INTERNAL DataRecvInterface(const DataRecvInterface&) = delete;
        HIAI_LIB_INTERNAL DataRecvInterface(DataRecvInterface&&) = delete;
        HIAI_LIB_INTERNAL DataRecvInterface& operator=(
            const DataRecvInterface&) = delete;
        HIAI_LIB_INTERNAL DataRecvInterface& operator=(
            DataRecvInterface&&) = delete;
    };
}
#endif  // INC_HIAIENGINE_DATA_RECV_INTERFACE_H_

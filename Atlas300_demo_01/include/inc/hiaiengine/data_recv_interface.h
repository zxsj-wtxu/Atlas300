/**
* @file data_recv_interface.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
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
    class DataRecvInterface {
      public:
        HIAI_LIB_INTERNAL DataRecvInterface() {}
        HIAI_LIB_INTERNAL virtual ~DataRecvInterface() {}
        /**
        * @ingroup hiaiengine
        * @brief 读取数据，保存
        * @param [in] message 输入数据
        * @return HIAI Status
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

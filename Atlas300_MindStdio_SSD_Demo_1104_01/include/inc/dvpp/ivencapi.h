/**
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef DVPP_IVENCAPI_H
#define DVPP_IVENCAPI_H

#include "dvpp_config.h"

class IVENCAPI {
public:
    virtual ~IVENCAPI(void) {}

    virtual int process(dvppapi_ctl_msg* MSG) = 0;
    virtual int init() = 0;
    virtual int start() = 0;

protected:
    dvppapi_ctl_msg* MSG_;
};

#endif // DVPP_IVENCAPI_H
/**
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef DVPP_IDVPPAPI_H
#define DVPP_IDVPPAPI_H

#include "dvpp_config.h"
#include "ivpcapi.h"
#include "ijpegdapi.h"
#include "ijpegeapi.h"
#include "ivdecapi.h"
#include "ivencapi.h"
#include "idvppcapability.h"
#include "export_macro.h"
#include <stdlib.h>

class IVPCAPI;
class IJPEGEAPI;
class IJPEGDAPI;
class IVDECAPI;
class IVENCAPI;
class IDVPPAPI;
class IDVPPCAPABILITY;

DVPP_EXPORT int CreateDvppApi(IDVPPAPI*& pIDVPPAPI);
DVPP_EXPORT int DvppCtl(IDVPPAPI*& pIDVPPAPI, int CMD, dvppapi_ctl_msg* MSG);
DVPP_EXPORT int DestroyDvppApi(IDVPPAPI*& pIDVPPAPI);

DVPP_EXPORT int CreateVdecApi(IDVPPAPI*& pIDVPPAPI, int singleton);
DVPP_EXPORT int VdecCtl(IDVPPAPI*& pIDVPPAPI, int CMD, dvppapi_ctl_msg* MSG, int singleton);
DVPP_EXPORT int DestroyVdecApi(IDVPPAPI*& pIDVPPAPI, int singleton);

class IDVPPAPI {
public:
    virtual ~IDVPPAPI(void) {}

    IVPCAPI* pIVPCAPI;
    IJPEGEAPI* pIJPEGEAPI;
    IJPEGDAPI* pIJPEGDAPI;
    IVDECAPI* pIVDECAPI;
    IVENCAPI* pIVENCAPI;
    IDVPPCAPABILITY* pIDVPPCAPABILITY;
};

#endif // DVPP_IDVPPAPI_H
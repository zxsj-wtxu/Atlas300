/* Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the Apache License Version 2.0.You may not use this file except in compliance with the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * Apache License for more details at
 * http://www.apache.org/licenses/LICENSE-2.0
 */
/*lint -e* */
#ifndef fmk_error_codes_h__
#define fmk_error_codes_h__

#include <string>
#include <map>
#include "framework/common/fmk_types.h"

#define SYSID_FWK           3       // 子系统ID
#define MODID_COMMON        0       // Common模块ID
#define MODID_OMG           1       // OMG模块ID
#define MODID_OME           2       // OME模块ID
#define MODID_CALIBRATION   3       // Calibration模块ID


#define DECLARE_ERRORNO(sysid, modid, name, value) \
                const domi::Status name = ((0xFF & ((uint8_t)sysid)) << 24) \
                                | ((0xFF & ((uint8_t)modid)) << 16) \
                                | (0xFFFF & ((uint16_t)value));\
                
// 各模块使用下面四个宏定义错误码
#define DECLARE_ERRORNO_COMMON(name, value) DECLARE_ERRORNO(SYSID_FWK, MODID_COMMON, name, value)
#define DECLARE_ERRORNO_OMG(name, value) DECLARE_ERRORNO(SYSID_FWK, MODID_OMG, name, value)
#define DECLARE_ERRORNO_OME(name, value) DECLARE_ERRORNO(SYSID_FWK, MODID_OME, name, value)
#define DECLARE_ERRORNO_CALIBRATION(name, value) DECLARE_ERRORNO(SYSID_FWK, MODID_CALIBRATION, name, value)

#define DEF_ERRORNO(name, desc) \
                const ErrorNoRegisterar g_##name##_errorno(name, desc);



// 获取错误码描述接口
#define GET_ERRORNO_STR(value) domi::StatusFactory::Instance()->GetErrDesc(value)

namespace domi
{
    class StatusFactory
    {
    public:
        static StatusFactory* Instance()
        {
            static StatusFactory instance;
            return &instance;
        }

        void RegisterErrorNo(uint32_t err, const std::string& desc)
        {
            // 防止重复添加
            if (err_desc_.find(err) != err_desc_.end()) return;
            err_desc_[err] = desc;
        }

        std::string GetErrDesc(uint32_t err)
        {
            auto iter_find = err_desc_.find(err);
            if (iter_find == err_desc_.end()) return "";
            return iter_find->second;
        }

    protected:
        StatusFactory() {}
        ~StatusFactory() {}
    private:
        std::map<uint32_t, std::string> err_desc_;
    };

    class ErrorNoRegisterar
    {
    public:
        ErrorNoRegisterar(uint32_t err, const std::string& desc)
        {
            StatusFactory::Instance()->RegisterErrorNo(err, desc);
        }
        ~ErrorNoRegisterar(){}
    };

    using Status = uint32_t;
    
    // general error code
    DECLARE_ERRORNO(0, 0, SUCCESS, 0);
    DECLARE_ERRORNO(0xFF, 0xFF, FAILED, 0xFFFFFFFF);

    // common errocode
    DECLARE_ERRORNO_COMMON(MEMALLOC_FAILED, 0);   // 50331648
    DECLARE_ERRORNO_COMMON(PARAM_INVALID, 1);           // 50331649
    DECLARE_ERRORNO_COMMON(CCE_FAILED, 2);           // 50331650
    DECLARE_ERRORNO_COMMON(RT_FAILED, 3);        // 50331651
    DECLARE_ERRORNO_COMMON(INTERNAL_ERROR, 4);               // 50331652
    DECLARE_ERRORNO_COMMON(CSEC_ERROR, 5);      // 50331653
    DECLARE_ERRORNO_COMMON(TEE_ERROR, 6);      // 50331653
    DECLARE_ERRORNO_COMMON(UNSUPPORTED, 100);
    DECLARE_ERRORNO_COMMON(OUT_OF_MEMORY, 101);

    // omg errorcode
    DECLARE_ERRORNO_OMG(PARSE_MODEL_FAILED, 0);
    DECLARE_ERRORNO_OMG(PARSE_WEIGHTS_FAILED, 1);
    DECLARE_ERRORNO_OMG(NOT_INITIALIZED, 2);
    DECLARE_ERRORNO_OMG(TIMEOUT, 3);

    // optimize errocode
    DECLARE_ERRORNO_OMG(TO_BE_DELETED, 200);
    DECLARE_ERRORNO_OMG(NOT_CHANGED, 201);

    // ome errorcode
    DECLARE_ERRORNO_OME(MODEL_NOT_READY, 0);
    DECLARE_ERRORNO_OME(PUSH_DATA_FAILED, 1);
    DECLARE_ERRORNO_OME(DATA_QUEUE_ISFULL, 2);
}

#endif // fmk_error_codes_h__


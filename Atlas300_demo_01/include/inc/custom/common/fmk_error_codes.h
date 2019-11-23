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

#define SYSID_FWK           3       // 子系统ID
#define MODID_COMMON        0       // Common模块ID
#define MODID_OMG           1       // OMG模块ID
#define MODID_OME           2       // OME模块ID
#define MODID_CALIBRATION   3       // Calibration模块ID

#define DEF_ERRORNO(sysid, modid, name, value, desc) \
                constexpr domi::Status name = ((0xFF & ((uint8_t)sysid)) << 24) \
                                | ((0xFF & ((uint8_t)modid)) << 16) \
                                | (0xFFFF & ((uint16_t)value));\
                const ErrorNoRegisterar g_##name##_errorno(name, desc);

// 各模块使用下面四个宏定义错误码
#define DEF_ERRORNO_COMMON(name, value,desc) DEF_ERRORNO(SYSID_FWK, MODID_COMMON, name, value, desc)
#define DEF_ERRORNO_OMG(name, value,desc) DEF_ERRORNO(SYSID_FWK, MODID_OMG, name, value, desc)
#define DEF_ERRORNO_OME(name, value,desc) DEF_ERRORNO(SYSID_FWK, MODID_OME, name, value, desc)
#define DEF_ERRORNO_CALIBRATION(name, value,desc) DEF_ERRORNO(SYSID_FWK, MODID_CALIBRATION, name, value, desc)

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


    // 通用错误码定义
    DEF_ERRORNO(0, 0, SUCCESS, 0, "success");
    DEF_ERRORNO(0xFF, 0xFF, FAILED, 0xFFFFFFFF, "failed");

    // common模块错误码定义
    DEF_ERRORNO_COMMON(MEMALLOC_FAILED, 0, "Failed to allocate memory!");   // 50331648
    DEF_ERRORNO_COMMON(PARAM_INVALID, 1, "Parameter's invalid!");           // 50331649
    DEF_ERRORNO_COMMON(CCE_FAILED, 2, "Failed to call CCE API!");           // 50331650
    DEF_ERRORNO_COMMON(RT_FAILED, 3, "Failed to call runtime API!");        // 50331651
    DEF_ERRORNO_COMMON(INTERNAL_ERROR, 4, "Internal errors");               // 50331652
    DEF_ERRORNO_COMMON(CSEC_ERROR, 5, "Failed to call libc_sec API!");      // 50331653
    DEF_ERRORNO_COMMON(TEE_ERROR, 6, "Failed to call tee API!");      // 50331653

    // TODO: Delete it after finishing error code feature
    DEF_ERRORNO_COMMON(UNSUPPORTED, 100, "Parameter's unsupported!");

    DEF_ERRORNO_COMMON(OUT_OF_MEMORY, 101, "Out of memory!");

}// namespace domi

#endif // fmk_error_codes_h__


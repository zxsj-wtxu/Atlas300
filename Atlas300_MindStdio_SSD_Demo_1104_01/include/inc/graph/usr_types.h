/**
 * Copyright (C)  2019. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the Apache License Version 2.0.You may not use this file except in compliance with the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * Apache License for more details at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * @file usr_types.h
 *
 * @brief
 *
 * @version 1.0
 *
 */

#ifndef GE_USR_TYPES_H
#define GE_USR_TYPES_H

#include <memory>
#include <vector>
#include <atomic>
namespace ge{

#define USR_TYPE_DEC(type, name) \
inline void set_##name(const type &value){ \
    name = value; \
}; \
type* mutable_##name(){ \
    return &name; \
}

#define USR_TYPE_HAS_DEC(type, name) \
inline void set_##name(const type &value){ \
    name = value; \
}; \
private:\
    bool has_mutable_##name{false};\
public:\
bool has_##name() const{ \
    return (has_mutable_##name) || QuantizeFactorHasData(name) ; \
}; \
type* mutable_##name(){ \
    has_mutable_##name = true;\
    return &name; \
}

#define USR_TYPE_BYTES_DEC(name) \
inline void clear_##name(){ \
    name.clear(); \
} \
inline void set_##name(void* value, size_t size){ \
    name.assign((uint8_t*)value, (uint8_t*)value+size); \
} \


struct UsrQuantizeFactor
{
public:
    //QuantizeScaleMode scale_mode;
    uint32_t scale_mode{0};
    std::vector<uint8_t> scale_value;
    int64_t scale_offset{0};
    std::vector<uint8_t> offset_data_value;
    int64_t offset_data_offset{0};
    std::vector<uint8_t> offset_weight_value;
    int64_t offset_weight_offset{0};
    std::vector<uint8_t> offset_pad_value;
    int64_t offset_pad_offset{0};

    USR_TYPE_DEC(uint32_t, scale_mode);
    USR_TYPE_BYTES_DEC(scale_value);


    USR_TYPE_DEC(int64_t, scale_offset);
    USR_TYPE_BYTES_DEC(offset_data_value);
    USR_TYPE_DEC(int64_t, offset_data_offset);


    USR_TYPE_BYTES_DEC(offset_weight_value);
    USR_TYPE_DEC(int64_t, offset_weight_offset);
    USR_TYPE_BYTES_DEC(offset_pad_value);
    USR_TYPE_DEC(int64_t, offset_pad_offset);


};

static inline bool QuantizeFactorHasData(const UsrQuantizeFactor& factor)
{
    return  factor.scale_value.size() > 0 || factor.offset_data_value.size() > 0
            || factor.offset_weight_value.size() > 0 || factor.offset_pad_value.size() > 0;
}

struct UsrAllOffsetQuantizeInfo {
public:
    UsrAllOffsetQuantizeInfo(){}
    UsrAllOffsetQuantizeInfo(float s ,int32_t o):scale(s),offset(o){}
    float scale{0};
    int32_t offset{0};

    USR_TYPE_DEC(float, scale);
    USR_TYPE_DEC(int32_t, offset);

};

struct UsrQuantizeCalcFactor
{
public:
    std::vector<uint8_t> offsetw;
    int64_t offsetw_offset{0};
    std::vector<uint8_t> offsetd;
    int64_t offsetd_offset{0};
    std::vector<uint8_t> scalereq;
    int64_t scaledreq_offset{0};
    std::vector<uint8_t> offsetdnext;
    int64_t offsetdnext_offset{0};

    USR_TYPE_BYTES_DEC(offsetw);
    USR_TYPE_DEC(int64_t, offsetw_offset);
    USR_TYPE_BYTES_DEC(offsetd);
    USR_TYPE_DEC(int64_t, offsetd_offset);
    USR_TYPE_BYTES_DEC(scalereq);
    USR_TYPE_DEC(int64_t, scaledreq_offset);
    USR_TYPE_BYTES_DEC(offsetdnext);
    USR_TYPE_DEC(int64_t, offsetdnext_offset);

};

static inline bool QuantizeFactorHasData(const UsrQuantizeCalcFactor& factor)
{
    return  factor.offsetw.size() > 0 || factor.offsetd.size() > 0
            || factor.scalereq.size() > 0 || factor.offsetdnext.size() > 0;
}

struct UsrQuantizeFactorParams
{
    uint32_t quantize_algo{0};
    uint32_t scale_type{0};
    UsrQuantizeFactor quantize_param;
    UsrQuantizeFactor dequantize_param;
    UsrQuantizeFactor requantize_param;
    UsrQuantizeCalcFactor quantizecalc_param;
    USR_TYPE_DEC(uint32_t, quantize_algo);
    USR_TYPE_DEC(uint32_t, scale_type);
    USR_TYPE_HAS_DEC(UsrQuantizeFactor, quantize_param);
    USR_TYPE_HAS_DEC(UsrQuantizeFactor, dequantize_param);
    USR_TYPE_HAS_DEC(UsrQuantizeFactor, requantize_param);
    USR_TYPE_HAS_DEC(UsrQuantizeCalcFactor, quantizecalc_param);

};


#undef USR_TYPE_DEC
#undef USR_TYPE_HAS_DEC
#undef USR_TYPE_BYTES_DEC

}

#endif //GE_USR_TYPES_H

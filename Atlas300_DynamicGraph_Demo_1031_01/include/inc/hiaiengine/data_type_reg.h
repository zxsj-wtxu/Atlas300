/**
* @file data_type_reg.h
*
* Copyright (C) Huawei Technologies Co., Ltd. 2018-2019. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_DATA_TYPE_REG_H_
#define INC_HIAIENGINE_DATA_TYPE_REG_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include "hiaiengine/log.h"

using std::string;
using std::vector;

// 定义序列化和反序列化接口类型
constexpr uint32_t DATY_TYPE_FUNC_DES = 0;
constexpr uint32_t DATY_TYPE_FUNC_SEAR = 1;
constexpr uint32_t DATY_TYPE_FUNC_CONVERT = 2;
constexpr uint32_t DATA_TYPE_FUNC_GET_SEAR = 3;
constexpr uint32_t DATA_TYPE_FUNC_GET_DEAR = 4;
constexpr uint32_t DATA_TYPE_FUNC_HANLDER = 4;

typedef std::shared_ptr<void> (*HIAI_DATADesearilizedFunT)(const std::string&);
typedef void(*HIAI_DATASearilizedFunT)(void* dataPtr, std::string&);
typedef std::shared_ptr<void> (*HIAI_ConvertFuncT)(void* dataPtr);

/**
* @ingroup hiaiengine
* @brief      用于注册高性能序列化接口，将序列化分成控制块和数据块
* @param [in] : inputPtr         输入的数据结构指针
* @param [out]：ctrlStr          数据结构中控制信息指针
* @param [out]：dataPtr          数据结构体中数据指针
* @param [out]：dataLen          数据结构体中数据大小
*/
typedef void(*HIAI_SerializeFuncT)(void* inputPtr, std::string& ctrlStr,
    uint8_t*& dataPtr, uint32_t& dataLen);

/**
* @ingroup hiaiengine
* @brief      用于注册高性能反序列化接口，将收到的控制块和数据块反序列化成对应数据类型
* @param [in] : ctrlPtr               数据结构中控制信息指针
* @param [in] : ctrlLen               数据结构中控制信息大小
* @param [in] : dataPtr               数据结构中数据信息指针
* @param [in] : dataLen               数据结构中数据大小
* @param [out]：std::shared_ptr<void> 反序列化输出的数据结构指针
*/
typedef std::shared_ptr<void>(*HIAI_DeserializeFuncT)(
    const char* ctrlPtr, const uint32_t& ctrlLen,
    const uint8_t* dataPtr, const uint32_t& dataLen);

/**
* @ingroup hiaiengine
* @brief DataTypeFactory, 工厂类，
* @param
*/
class HIAIDataTypeFactory {
  protected:
    HIAI_LIB_INTERNAL static std::mutex& GetMutex() {
        static std::mutex dataTypeMutex;
        return dataTypeMutex;
    }
  public:
    HIAI_LIB_INTERNAL HIAIDataTypeFactory() { }
    HIAI_LIB_INTERNAL ~HIAIDataTypeFactory() { }
  private:
    HIAI_LIB_INTERNAL HIAIDataTypeFactory(const HIAIDataTypeFactory&) = delete;
    HIAI_LIB_INTERNAL HIAIDataTypeFactory(HIAIDataTypeFactory&&) = delete;
    HIAI_LIB_INTERNAL HIAIDataTypeFactory& operator=(
        const HIAIDataTypeFactory&) = delete;
    HIAI_LIB_INTERNAL HIAIDataTypeFactory& operator=(
        HIAIDataTypeFactory&&) = delete;
  public:
    /**
    * @ingroup
    * @brief DataTypeFactory, GetInstance，
    * @param
    */
    HIAI_LIB_VISIBILITY static HIAIDataTypeFactory* GetInstance();

    /**
    * @ingroup
    * @brief DataTypeFactory, Register， 注册反序列化函数
    * @param:[in]name: DataType名称
    * @param:[in]func: 反序列化函数
    */
    HIAI_LIB_INTERNAL HIAI_StatusT Register(const std::string& name,
        HIAI_DATADesearilizedFunT func) {
        if (func == nullptr) {
            return HIAI_ENGINE_FUNCTOR_NULL;
        }

        std::lock_guard<std::mutex> lock(GetMutex());
        std::map<std::string, HIAI_DATADesearilizedFunT>::iterator iter =
            desearlizeFuncMap_.find(name);
        if (iter != desearlizeFuncMap_.end()) {
            return HIAI_ENGINE_FUNCTOR_EXIST;
        }
        desearlizeFuncMap_[name] = func;
        return HIAI_OK;
    }


    /**
    * @ingroup
    * @brief DataTypeFactory, Register， 注册序列化函数
    * @param:[in]name: DataType名称
    * @param:[in]func: 序列化函数
    */
    HIAI_LIB_INTERNAL HIAI_StatusT Register(const std::string& name,
        HIAI_DATASearilizedFunT func) {
        if (func == nullptr) {
            return HIAI_ENGINE_FUNCTOR_NULL;
        }

        std::lock_guard<std::mutex> lock(GetMutex());
        std::map<std::string, HIAI_DATASearilizedFunT>::iterator iter =
            searilizeFuncMap_.find(name);
        if (iter != searilizeFuncMap_.end()) {
            return HIAI_ENGINE_FUNCTOR_EXIST;
        }
        searilizeFuncMap_[name] = func;
        return HIAI_OK;
    }

    /**
    * @ingroup
    * @brief DataTypeFactory, Register， 注册序列化函数
    * @param:[in]name: DataType名称
    * @param:[in]func: 序列化函数
    */
    HIAI_LIB_INTERNAL HIAI_StatusT Register(const std::string& name,
        HIAI_ConvertFuncT func) {
        if (func == nullptr) {
            return HIAI_ENGINE_FUNCTOR_NULL;
        }
        std::lock_guard<std::mutex> lock(GetMutex());
        std::map<std::string, HIAI_ConvertFuncT>::iterator iter =
            convertFuncMap_.find(name);
        if (iter != convertFuncMap_.end()) {
            return HIAI_ENGINE_FUNCTOR_EXIST;
        }
        convertFuncMap_[name] = func;
        return HIAI_OK;
    }

    /**
    * @ingroup
    * @brief DataTypeFactory, Register， 注册HIAI序列化函数
    * @param:[in]name: DataType名称
    * @param:[in]hiaiSearFunc: 序列化函数
    * @param:[in]hiaiDearFunc: 反序列化函数
    */
    HIAI_LIB_INTERNAL HIAI_StatusT Register(const std::string& name,
        HIAI_SerializeFuncT hiaiSerializeFunc,
        HIAI_DeserializeFuncT hiaiDeSerializeFunc) {
        if (hiaiSerializeFunc == nullptr || hiaiDeSerializeFunc == nullptr) {
            return HIAI_ENGINE_FUNCTOR_NULL;
        }

        std::lock_guard<std::mutex> lock(GetMutex());
        std::map<std::string, HIAI_SerializeFuncT>::iterator iter =
            hiaiSerializeFuncMap.find(name);
        if (iter != hiaiSerializeFuncMap.end()) {
            return HIAI_ENGINE_FUNCTOR_EXIST;
        }
        hiaiSerializeFuncMap[name] = hiaiSerializeFunc;

        std::map<std::string, HIAI_DeserializeFuncT>::iterator dear_iter =
            hiaiDeSerializeFuncMap.find(name);
        if (dear_iter != hiaiDeSerializeFuncMap.end()) {
            return HIAI_ENGINE_FUNCTOR_EXIST;
        }
        hiaiDeSerializeFuncMap[name] = hiaiDeSerializeFunc;
        return HIAI_OK;
    }

    /**
    * @ingroup
    * @brief DataTypeFactory, UnRegister， 取消注册
    * @param:[in]name: DataType名称
    * @param:[in]func: 序列化函数
    */
    HIAI_LIB_INTERNAL HIAI_StatusT UnRegister(const std::string& name,
        const uint32_t funcType) {
        std::lock_guard<std::mutex> lock(GetMutex());
        if (funcType == DATY_TYPE_FUNC_DES) {
            desearlizeFuncMap_.erase(name);
        } else if (funcType == DATY_TYPE_FUNC_SEAR) {
            searilizeFuncMap_.erase(name);
        } else if (funcType == DATY_TYPE_FUNC_CONVERT) {
            convertFuncMap_.erase(name);
        } else if (funcType == DATA_TYPE_FUNC_HANLDER) {
            hiaiSerializeFuncMap.erase(name);
            hiaiDeSerializeFuncMap.erase(name);
        }
        return HIAI_OK;
    }

    /**
    * @ingroup
    * @brief DataTypeFactory, GetDesearFunc， 获取反序列化函数
    * @param:[in]name: 反序列化名称
    */
    HIAI_LIB_INTERNAL HIAI_DATADesearilizedFunT GetDesearFunc(
        const std::string& name) {
        auto iter = desearlizeFuncMap_.find(name);
        if (iter != desearlizeFuncMap_.end()) {
            return (*iter).second;
        }
        return nullptr;
    }

    /**
    * @ingroup
    * @brief DataTypeFactory, GetSearFunc， 获取序列化函数
    * @param:[in]name: 序列化名称
    */
    HIAI_LIB_INTERNAL HIAI_DATASearilizedFunT GetSearFunc(
        const std::string& name) {
        auto iter = searilizeFuncMap_.find(name);
        if (iter != searilizeFuncMap_.end()) {
            return (*iter).second;
        }
        return nullptr;
    }

    /**
    * @ingroup
    * @brief DataTypeFactory, GetConvcFunc， 获取转化为智能指针的函数
    * @param:[in]name: 序列化名称
    */
    HIAI_LIB_INTERNAL HIAI_ConvertFuncT GetConvcFunc(const std::string& name) {
        auto iter = convertFuncMap_.find(name);
        if (iter != convertFuncMap_.end()) {
            return (*iter).second;
        }
        return nullptr;
    }

    /**
    * @ingroup
    * @brief DataTypeFactory, GetSerializeFunc 获取序列化函数,新的接口
    * @param:[in]name: 序列化名称
    */
    HIAI_LIB_INTERNAL HIAI_SerializeFuncT GetSerializeFunc(
        const std::string& name) {
        auto iter = hiaiSerializeFuncMap.find(name);
        if (iter != hiaiSerializeFuncMap.end()) {
            return (*iter).second;
        }
        return nullptr;
    }

    /**
    * @ingroup
    * @brief DataTypeFactory, GetDeserializeFunc 获取反序列化函数,旧接口
    * @param:[in]name: 反序列化名称
    */
    HIAI_LIB_INTERNAL HIAI_DeserializeFuncT GetDeserializeFunc(
        const std::string& name) {
        auto iter = hiaiDeSerializeFuncMap.find(name);
        if (iter != hiaiDeSerializeFuncMap.end()) {
            return (*iter).second;
        }
        return nullptr;
    }

  private:
    std::map<std::string, HIAI_DATADesearilizedFunT> desearlizeFuncMap_;
    std::map<std::string, HIAI_DATASearilizedFunT> searilizeFuncMap_;
    std::map<std::string, HIAI_ConvertFuncT> convertFuncMap_;

    // 新的HIAI高性能序列化与反序列化map
    std::map<std::string, HIAI_SerializeFuncT>  hiaiSerializeFuncMap;
    std::map<std::string, HIAI_DeserializeFuncT>  hiaiDeSerializeFuncMap;
};

/**
* @ingroup hiaiengine
* @brief AutoRegister, 自动注册类
* @param
*/
class HIAIAutoRegister {
  public:
    HIAI_LIB_INTERNAL explicit HIAIAutoRegister(const std::string& name,
        HIAI_DATADesearilizedFunT func) :
        desName_(name),
        desStatus_(HIAI_INIT_FAIL),
        searStatus_(HIAI_INIT_FAIL),
        convertStatus_(HIAI_INIT_FAIL),
        hiaiHandleStatus_(HIAI_INIT_FAIL) {
        if (HIAIDataTypeFactory::GetInstance() != nullptr) {
            desStatus_ = HIAIDataTypeFactory::GetInstance()->Register(name, func);
        }
    }

    HIAI_LIB_INTERNAL explicit HIAIAutoRegister(const std::string& name,
        HIAI_DATASearilizedFunT func) :
        searName_(name),
        desStatus_(HIAI_INIT_FAIL),
        searStatus_(HIAI_INIT_FAIL),
        convertStatus_(HIAI_INIT_FAIL),
        hiaiHandleStatus_(HIAI_INIT_FAIL) {
        if (HIAIDataTypeFactory::GetInstance() != nullptr) {
            searStatus_ = HIAIDataTypeFactory::GetInstance()->Register(name, func);
        }
    }

    HIAI_LIB_INTERNAL explicit HIAIAutoRegister(const std::string& name,
        HIAI_ConvertFuncT func) :
        convertName_(name),
        desStatus_(HIAI_INIT_FAIL),
        searStatus_(HIAI_INIT_FAIL),
        convertStatus_(HIAI_INIT_FAIL),
        hiaiHandleStatus_(HIAI_INIT_FAIL) {
        if (HIAIDataTypeFactory::GetInstance() != nullptr) {
            convertStatus_ = HIAIDataTypeFactory::GetInstance()->
                Register(name, func);
        }
    }

    HIAI_LIB_INTERNAL explicit HIAIAutoRegister(const std::string& name,
        HIAI_SerializeFuncT hiaiSerializeFunc,
        HIAI_DeserializeFuncT hiaiDeSerializeFunc):
        hiaiHandleName_(name),
        desStatus_(HIAI_INIT_FAIL),
        searStatus_(HIAI_INIT_FAIL),
        convertStatus_(HIAI_INIT_FAIL),
        hiaiHandleStatus_(HIAI_INIT_FAIL) {
        if (HIAIDataTypeFactory::GetInstance() != nullptr) {
            hiaiHandleStatus_ = HIAIDataTypeFactory::GetInstance()->Register(
                name, hiaiSerializeFunc, hiaiDeSerializeFunc);
        }
    }

    HIAI_LIB_INTERNAL ~HIAIAutoRegister() {
        if (desStatus_ == HIAI_OK) {
            HIAIDataTypeFactory::GetInstance()->
                UnRegister(desName_, DATY_TYPE_FUNC_DES);
        }
        if (searStatus_ == HIAI_OK) {
            HIAIDataTypeFactory::GetInstance()->
                UnRegister(desName_, DATY_TYPE_FUNC_SEAR);
        }
        if (convertStatus_ == HIAI_OK) {
            HIAIDataTypeFactory::GetInstance()->
                UnRegister(convertName_, DATY_TYPE_FUNC_CONVERT);
        }
        if (hiaiHandleStatus_ == HIAI_OK) {
            HIAIDataTypeFactory::GetInstance()->
                UnRegister(hiaiHandleName_, DATA_TYPE_FUNC_HANLDER);
        }
    }

  private:
    HIAI_LIB_INTERNAL HIAIAutoRegister(const HIAIAutoRegister&) = delete;
    HIAI_LIB_INTERNAL HIAIAutoRegister(HIAIAutoRegister&&) = delete;
    HIAI_LIB_INTERNAL HIAIAutoRegister& operator=(
        const HIAIAutoRegister&) = delete;
    HIAI_LIB_INTERNAL HIAIAutoRegister& operator=(
        HIAIAutoRegister&&) = delete;
  private:
    std::string desName_;
    std::string searName_;
    std::string convertName_;
    std::string hiaiHandleName_;
    HIAI_StatusT desStatus_;
    HIAI_StatusT searStatus_;
    HIAI_StatusT convertStatus_;
    HIAI_StatusT hiaiHandleStatus_;
};

/**
* @ingroup hiaiengine
* @brief HIAI_REGISTER_DATA_TYPE, 自动注册数据类型，支持序列化反序列化
* @param [in]Type: 数据类型
* @param [in]name: 数据类型名字
*/
// Demo:
/*
HIAI_REGISTER_DATA_TYPE(UseDefMsg1, "UseDefMsg1")
HIAI_REGISTER_DATA_TYPE(UseDefMsg2, "UseDefMsg2")
*/
#define HIAI_REGISTER_DATA_TYPE(name, Type)                            \
std::shared_ptr<Type> HIAI_Create##Type(const std::string& inputStr) { \
    std::shared_ptr<Type> data = nullptr;                              \
    try {                                                              \
        data = std::make_shared<Type>();                               \
    } catch(...) {                                                     \
        HIAI_ENGINE_LOG(HIAI_MEMORY_EXHAUSTED,                         \
            "std::makeshard error");                                   \
        data = nullptr;                                                \
    }                                                                  \
    if (data == nullptr) {                                             \
        return nullptr;                                                \
    }                                                                  \
    std::istringstream input_stream(inputStr);                         \
    cereal::PortableBinaryInputArchive archive(input_stream);          \
    archive(*data);                                                    \
    return data;                                                       \
}                                                                      \
void  HIAI_Searized##Type(void* dataPtr, std::string& out_str) {       \
    Type* data = static_cast<Type*>(dataPtr);                          \
    std::ostringstream outputStr;                                      \
    cereal::PortableBinaryOutputArchive archive(outputStr);            \
    archive(*data);                                                    \
    out_str = outputStr.str();                                         \
    return;                                                            \
}                                                                      \
std::shared_ptr<Type> HIAI_Convert_C_##Type(void* dataPtr) {           \
    Type* data = static_cast<Type*>(dataPtr);                          \
    return std::shared_ptr<Type>(data, [](Type*){});                   \
}                                                                      \
static HIAIAutoRegister hiai_auto_dser##Type(name,                     \
     (HIAI_DATADesearilizedFunT)HIAI_Create##Type);                    \
static HIAIAutoRegister hiai_auto_ser##Type(name,                      \
    (HIAI_DATASearilizedFunT) HIAI_Searized##Type);                    \
static HIAIAutoRegister hiai_auto_convert##Type(name,                  \
    (HIAI_ConvertFuncT)HIAI_Convert_C_##Type);

/**
* @ingroup hiaiengine         HIAI_REGISTER_SERIALIZE_FUNC
* @brief                高性能序列化和反序列化的注册接口宏
* 该宏的处理函数实现数据结构中的大数据data采用传递指针地址的方式而非拷贝，
* 控制信息进行序列化处理，提高整体性能。(数据结构拆分为控制信息和数据信息)
* @param [in] : name                  注册的消息名称
* @param [in] : Type                  数据类型
* @param [in] : hiaiSerializeFunc     序列化函数
* @param [in] : hiaiDeSerializeFunc   反序列化函数
*/
#define HIAI_REGISTER_SERIALIZE_FUNC(name, Type, hiaiSerializeFunc,           \
    hiaiDeSerializeFunc)                                                      \
    static HIAIAutoRegister hiai_auto_get_hiai_func##Type(name,               \
    hiaiSerializeFunc, hiaiDeSerializeFunc);                                  \
    static std::shared_ptr<Type> HIAI_Convert_C_Fast_##Type(void* dataPtr) {  \
        Type* data = static_cast<Type*>(dataPtr);                             \
        return std::shared_ptr<Type>(data, [](Type*){});                      \
    }                                                                         \
    static HIAIAutoRegister hiai_auto_convert_Fast_##Type(name,               \
        (HIAI_ConvertFuncT)HIAI_Convert_C_Fast_##Type);

#define HIAI_INTERNAL_CONCATENATE_DIRECT(s1, s2, s3) s1##s2##s3
#define HIAI_INTERNAL_CONCATENATE(s1, s2, s3)                       \
    HIAI_INTERNAL_CONCATENATE_DIRECT(s1, s2, s3)
#define HIAI_INTERNAL_ANONYMOUS_VARIABLE(name, type)                \
    HIAI_INTERNAL_CONCATENATE(name, type, __LINE__)

#define HIAI_REGISTER_TEMPLATE_DATA_TYPE(name, BasicType, ...)      \
    std::shared_ptr<BasicType<__VA_ARGS__>>                         \
    HIAI_INTERNAL_ANONYMOUS_VARIABLE(HIAI_Create_, BasicType)       \
        (const std::string& inputStr) {                             \
    std::shared_ptr<BasicType<__VA_ARGS__>> data =  nullptr;        \
    try {                                                           \
        data = std::make_shared<BasicType<__VA_ARGS__>>();          \
    } catch(...) {                                                  \
        HIAI_ENGINE_LOG(HIAI_MEMORY_EXHAUSTED,                      \
            "std::makeshard error");                                \
        return nullptr;                                             \
    }                                                               \
    if (data == nullptr) {                                          \
        return nullptr;                                             \
    }                                                               \
    std::istringstream input_stream(inputStr);                      \
    cereal::PortableBinaryInputArchive archive(input_stream);       \
    archive(*data);                                                 \
    return data;                                                    \
}                                                                   \
void  HIAI_INTERNAL_ANONYMOUS_VARIABLE(HIAI_Sea_, BasicType)        \
      (void* dataPtr, std::string& out_str) {                       \
      BasicType<__VA_ARGS__>* data =                                \
        (BasicType<__VA_ARGS__>*)dataPtr;                           \
      std::ostringstream outputStr;                                 \
      cereal::PortableBinaryOutputArchive archive(outputStr);       \
      archive(*data);                                               \
      out_str = outputStr.str();                                    \
      return;                                                       \
}                                                                   \
std::shared_ptr<BasicType<__VA_ARGS__> >                            \
    HIAI_INTERNAL_ANONYMOUS_VARIABLE(HIAI_Convert_, BasicType)      \
        (void* dataPtr) {                                           \
    BasicType<__VA_ARGS__>* data =                                  \
        (BasicType<__VA_ARGS__>*)dataPtr;                           \
    return std::shared_ptr<BasicType<__VA_ARGS__> >(data);          \
}                                                                   \
static HIAIAutoRegister HIAI_INTERNAL_ANONYMOUS_VARIABLE(           \
    hiai_auto_dser, BasciType)(name,                                \
       (HIAI_DATADesearilizedFunT) HIAI_INTERNAL_ANONYMOUS_VARIABLE \
       (HIAI_Create_, BasicType));                                  \
static HIAIAutoRegister HIAI_INTERNAL_ANONYMOUS_VARIABLE(hiai_auto_ser,      \
        BasicType)(name,                                                     \
       (HIAI_DATASearilizedFunT) HIAI_INTERNAL_ANONYMOUS_VARIABLE(HIAI_Sea_, \
       BasicType));                                                          \
static HIAIAutoRegister HIAI_INTERNAL_ANONYMOUS_VARIABLE(hiai_auto_convert,  \
        BasicType)(name,                                                     \
        (HIAI_ConvertFuncT)HIAI_INTERNAL_ANONYMOUS_VARIABLE(HIAI_Convert_,   \
        BasicType));


#define HIAI_SERIALIZE(member, ...)  \
template <class Archive> \
void serialize(Archive& ar) {\
    ar(member, ##__VA_ARGS__); \
}
#endif  // INC_HIAIENGINE_DATA_TYPE_H_

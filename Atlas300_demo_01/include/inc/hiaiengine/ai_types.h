/**
* @file ai_types.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef _AI_ENGINE_COMMON_TYPES_H_
#define _AI_ENGINE_COMMON_TYPES_H_

#include <string>
#include <map>
#include <vector>

#ifndef HIAI_WIN_VERSION
#include <linux/limits.h>
#endif



namespace hiai
{
#ifndef HIAI_WIN_VERSION
    const uint32_t MAX_PATH = PATH_MAX;
#endif
    /*
    * AIStatus，Engine层返回的错误信息，待补充
    */
    using AIStatus = uint32_t;

    /*
     *hiai::SUCCESS 返回成功
     */
    static const AIStatus SUCCESS = 0;
    static const AIStatus FAILED = 1;
    static const AIStatus NOT_INIT = 2;
    static const AIStatus INVALID_PARA = 3;
    static const AIStatus AI_TASK_TIMEOUT = 4;


    enum AI_TENSOR_TYPE
    {
        MODEL_BUFFER,
        NEURAL_NETWORK_BUFFER,
        IMAGE_BUFFER,
        GENERAL_BUFFER,
        CUSTOM
    };
    /*
    * node 调用方式
    */
    enum AI_NODE_EXEC_MODE
    {
        SYNC,                   /*   同步调用    */
        ASYNC,                  /*   异步调用    */
        BOTH                    /*   同步异步调用*/
    };
    enum AI_OFFLINE_MODEL_TYPE
    {
        NPU_OFFLINE_MODEL,
        MAX_OFFLINE_MODEL
    };
    enum AITaskType
    {
        NPU_OFFLINE_TASK
    };

    enum AI_IDLE_STATUS
    {
        WORKING,
        IDLE_TIMEOUT,
        TIMEOUT_NOT_WORK
    };

    enum AI_TASK_STATUS
    {
        TASK_WAIT,
        TASK_RUNNING,
        TASK_TIMEOUT,
        TASK_DONE
    };

    enum NN_DATA_TYPE
    {
        HIAI_DATA_FLOAT = 0,         /**< float type */
        HIAI_DATA_HALF,              /**< fp16 type */
        HIAI_DATA_INT8,              /**< int8 type */
        HIAI_DATA_INT32,             /**< int32 type */
        HIAI_DATA_UINT8,             /**< uint8 type */
        HIAI_DATA_RESERVED
    };

    enum TensorFormat
    {
        NCHW = 0,         /**< NCHW */
        NHWC,             /**< NHWC */
        ND,               /**< Nd Tensor */
    };

    /**
    * 运行时上下文信息，用于存放额外的参数等信息
    * 仅提key-value的参数定义，不支持value复杂结构
    */
    class AIContext
    {
    public:
        /*
        * @brief 获取参数
        * @param [in] key  参数对应的key
        * @return sring key对应的值，如果不存在，则返回空字符串
        */
        const std::string GetPara(const std::string &key) const;

        /*
        * @brief 设置参数
        * @param [in] key    参数对应的key
        * @param [in] value  参数对应的value
        */
        void AddPara(const std::string &key, const std::string &value);

        /*
        * @brief 删除参数
        * @param [in] key 待删除参数对应的key
        */
        void DeletePara(const std::string &key);

        /*
        * @brief 获取所有参数
        * @param [out] keys 所有已设置的参数key
        */
        void GetAllKeys(std::vector<std::string> &keys);

    private:
        std::map<std::string, std::string> paras_; /** 参数的名值对定义 */
    };

    /*
    * 模型类型
    */
    enum MODEL_TYPE
    {
        AI_NPU_OFFLINE_MODEL = 0,
        AI_CAMBRICON_OFFLINE_MODEL
    };

    /*
    * 描述Tensor尺寸
    */
    struct TensorDimension
    {
        std::vector<uint32_t> dims;
        uint32_t format;
        uint32_t data_type;
        uint32_t size;
        std::string  name;
    };

}

#endif

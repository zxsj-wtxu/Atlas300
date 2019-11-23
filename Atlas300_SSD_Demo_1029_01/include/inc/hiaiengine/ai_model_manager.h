/**
* @file ai_model_manager.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef _HIAI_SERVER_ENGINE_MODELMANAGER_AIMODELMANANGER_H_
#define _HIAI_SERVER_ENGINE_MODELMANAGER_AIMODELMANANGER_H_

#include <condition_variable>   //lint !e7
#include "hiaiengine/ai_tensor.h"
#include "hiaiengine/ai_nn_node.h"

namespace hiai
{
    class AIModelManagerImpl;

    class AIModelManager : public IAINNNode
    {
    public:
        AIModelManager();
        /*
        * @brief 初始化接口，加载模型及其他初始化操作
        * @param [in] config 配置信息
        * @param [in] model_desc 模型信息
        * @return SUCCESS 成功
        * @return 其他 失败
        */
        virtual AIStatus Init(const AIConfig &config,
            const std::vector<AIModelDescription> &model_descs = {}) override;

        /*
        * @brief 设置监听
        * @parm [in] 如果listener设置为nullptr，表示process接口为同步调用,否则为异步
        * @return SUCCESS 成功
        * @return 其他 失败
        */
        virtual AIStatus SetListener(std::shared_ptr<IAIListener> listener) override;

        /*
        * @brief 设置动态batch接口
        * @param [in] inputDim 模型输入尺寸
        * @param [in] input 输入数据
        * @param [out] inputIndex 需要设置动态batch输入的序号, 从0开始
        * @param [in] batchNumber 动态batch数
        * @return SUCCESS 成功
        * @return 其他 失败
        */
        AIStatus SetInputDynamicBatch(const vector<TensorDimension>& inputDim, std::vector <std::shared_ptr <IAITensor> > &input,
            uint32_t inputIndex, uint32_t batchNumber);

        /*
        * @brief 设置动态AIPP接口
        * @param [in] inData 模型输入数据
        * @param [in] aippParms 动态AIPP tensor
        * @return 其他 失败
        */
        AIStatus SetInputDynamicAIPP(
                std::vector<std::shared_ptr<IAITensor>>& inData,
                std::shared_ptr<AippDynamicParaTensor> aippParms);

        /*
        * @brief 计算接口
        * @param [in] context 运行时上下文信息，包含engine运行时的一些可变参数配置
        * @param [in] in_data 输入数据
        * @param [out] out_data 输出数据
        * @param [in] timeout 超时时间，同步调用时无效
        * @return SUCCESS 成功
        * @return 其他 失败
        */
        virtual AIStatus Process(AIContext &context,
            const std::vector<std::shared_ptr<IAITensor>> &in_data,
            std::vector<std::shared_ptr<IAITensor>> &out_data, uint32_t timeout);

        /*
        * @brief 是否可以预分配输出内存，该接口由业务NNNode实现，默认值为true.
        */
        virtual bool IsPreAllocateOutputMem() override;


        static AINNNodeDescription GetDescription();

        /*
        * @brief 获取已加载模型的输入输出尺寸
        * @param [in] model_name 模型名字
        * @param [out] input_tensor 模型输入尺寸
        * @param [out] output_tensor 模型输出尺寸
        * @return SUCCESS 成功
        * @return 其他 失败
        */
        AIStatus GetModelIOTensorDim(const std::string& model_name,
            std::vector<TensorDimension>& input_tensor, std::vector<TensorDimension>& output_tensor);

        /*
        * @brief 根据模型名字查询模型使用内存大小
        * @param [in] model_name 模型名字
        * @return 模型使用内存大小
        */
        int32_t GetMaxUsedMemory(std::string model_name);

        /*
        * @brief 设置线程推理请求ID
        * @param [in] request_id 模型名字
        * @return 无
        */
        static void SetRequestId(uint64_t request_id);

        ~AIModelManager();

        /*
        * @brief 创建输入的Tensor列表
        * @param [out] in_data 输出tensor列表，计算输出时可能使用
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        AIStatus CreateInputTensor(
            std::vector<std::shared_ptr<IAITensor>> &in_data);

        /*
        * @brief 创建输出的Tensor列表
        * @param [in] in_data 输入tensor列表，计算输出时可能使用
        * @param [out] out_data 输出的tensor列表
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        virtual AIStatus CreateOutputTensor(
            const std::vector<std::shared_ptr<IAITensor>> &in_data,
            std::vector<std::shared_ptr<IAITensor>> &out_data) override;

#ifdef __LITE__
        /*
        * @brief 闲置超时调用，释放资源。实现时调用的是UnloadModels。
        */
        virtual AIStatus IdleRelease();
        /*
        * @brief 超时后恢复，包含打开设备，加载模型等。实现时调用的是LoadModels。
        */
        virtual AIStatus IdleResume();
#endif // __LITE__

    private:
        AIModelManagerImpl* impl_;
    };
}

#endif

/** 
* @file ai_nn_node.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef _HIAI_IENGINE_H_
#define _HIAI_IENGINE_H_

#include <mutex> 
#include <stdint.h>
#include <vector>
#include "ai_tensor.h"
#include "ai_types.h"
#include "proto/ai_types.pb.h"

using namespace std;

namespace hiai
{
    class IAINNNode;
    /*
    * 异步回调监听接口，由调用方实现
    */
    class IAIListener
    {
    public:
        virtual ~IAIListener(){}
        /*
        * @brief 异步回调接口
        * @param [in] context 运行时上下文信息，包含nnnode运行时的一些可变参数配置
        * @param [in] result 执行结束时任务状态
        * @param [in] out_data 执行结束时的输出数据
        */
        virtual void OnProcessDone(const AIContext &context, int result, 
            const std::vector<std::shared_ptr<IAITensor>> &out_data) = 0;

        /*
        * @brief 服务死亡监听回调接口，当客户端监听到服务端挂死时，通知应用
        */
        virtual void OnServiceDied() {};
    };

    /*
    * NNNode接口，业务提供方实现
    */
    class IAINNNode
    {
    public:
        virtual  ~IAINNNode(){}
        /*
        * @brief 初始化接口，业务在该接口中实现模型加载或其他初始化动作
        * @param [in] model_desc 模型信息，如果不需要模型，则传入空的vector
        * @param [in] config 配置参数
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        virtual AIStatus Init(const AIConfig &config, 
            const std::vector<AIModelDescription> &model_descs = {}) = 0;

        /*
        * @brief 设置监听
        * @parm [in] 如果listener设置为nullptr，表示process接口为同步调用,否则为异步
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        virtual AIStatus SetListener(std::shared_ptr<IAIListener> listener) = 0;

        /*
        * @brief 计算接口
        * @param [in] context 运行时上下文信息，包含nnnode运行时的一些可变参数配置
        * @param [in] in_data 输入数据
        * @param [out] out_data 输出数据
        * @param [in] timeout 超时时间，同步调用时无效
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        virtual AIStatus Process(AIContext &context, 
            const std::vector<std::shared_ptr<IAITensor>> &in_data, 
            std::vector<std::shared_ptr<IAITensor>> &out_data, uint32_t timeout) = 0;

        /*
        * @brief 创建输出的Tensor列表
        * @param [in] in_data 输入tensor列表，计算输出时可能使用
        * @param [out] out_data 输出的tensor列表
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        virtual AIStatus CreateOutputTensor(
            const std::vector<std::shared_ptr<IAITensor>> &in_data,
            std::vector<std::shared_ptr<IAITensor>> &out_data) { (void)in_data;(void)out_data;return SUCCESS; }

        /*
        * @brief 是否可以预分配输出内存，该接口由业务NNNode实现，默认值为true.
        */
        virtual bool IsPreAllocateOutputMem() { return true; }

        /*
        * @brief 判断nnnode是否有效
        */
        virtual AIStatus IsValid() { return SUCCESS; }

        /*
        * @brief 查询node 支持的同步方式
        * @return BOTH    支持同步异步
        * @return ASYNC   仅支持异步
        * @return SYNC    仅支持同步
        */
        virtual AI_NODE_EXEC_MODE GetSupportedExecMode() { return AI_NODE_EXEC_MODE::BOTH; }

        /*
        * @brief 获取业务最大使用内存
        * @return 业务最大使用内存大小
        */
        virtual uint32_t GetMaxUsedMemory() { return 0; }

    };

    /*
    * 定义NNNode创建函数指针
    */
    typedef std::shared_ptr<IAINNNode>(*AINNNODE_CREATE_FUN)(void);

    /*
    * AINNNodeFactory，支持业务引擎自注册，提供创建NNNode接口，提供已注册NNNode描述信息查询（按名称查询以及查询全部）
    */
    class AINNNodeFactory
    {
    public:
        static AINNNodeFactory* GetInstance();

        /*
        * @brief 根据nnnode name创建NNNode
        * @param [in] name nnnode名称
        * @return std::shared_ptr<IAINNNode> name对应的NNNode对象指针，如果返回nullptr，表示找不到对应的nnnode
        */
        std::shared_ptr<IAINNNode> CreateNNNode(const std::string &name);

        /*
        * @brief 获取所有已注册nnnode的描述信息
        * @param [out] nnnode_desc 所有已注册的nnnode描述信息
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        void GetAllNNNodeDescription(AINNNodeDescriptionList &nnnode_desc);

        /*
        * @brief 根据nnnode name获取nnnode的描述信息
        * @param [in] name nnnode名称
        * @param [out] engin_desc nnnode描述信息
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        AIStatus GetNNNodeDescription(const std::string &name, AINNNodeDescription &engin_desc);

        /*
        * @brief 注册NNNode创建函数
        * @param [in] nnnode_desc nnnode描述信息
        * @param [in] create_func nnnode创建函数
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        AIStatus RegisterNNNodeCreator(const AINNNodeDescription &nnnode_desc,
            AINNNODE_CREATE_FUN create_func);
        AIStatus RegisterNNNodeCreator(const string nnnode_str,
            AINNNODE_CREATE_FUN create_func);

        /*
        * @brief 注销NNNode
        * @param [in] name nnnode名称
        * @return SUCCESS 成功
        */
        AIStatus UnRegisterNNNode(const AINNNodeDescription &nnnode_desc);
        AIStatus UnRegisterNNNode(const string nnnode_str);

    private:
        std::map<std::string, AINNNODE_CREATE_FUN> create_func_map_;
        std::map<std::string, AINNNodeDescription> nnnode_desc_map_;
        std::mutex  map_lock_;

    };

    class AINNNodeRegisterar
    {
    public:
        AINNNodeRegisterar(const AINNNodeDescription &nnnode_desc, AINNNODE_CREATE_FUN create_func)
        {
            AINNNodeFactory::GetInstance()->RegisterNNNodeCreator(nnnode_desc, create_func);
            nnnode_desc_ = nnnode_desc;
            nnnode_str_.erase(nnnode_str_.begin(),nnnode_str_.end());
        }

        ~AINNNodeRegisterar(){
                AINNNodeFactory::GetInstance()->UnRegisterNNNode(nnnode_desc_);
        }
    private:
        AINNNodeDescription nnnode_desc_;
        string nnnode_str_;
    };

    /*
    * @brief NNNode 注册宏，业务NNNode在实现类中使用
    * 直接使用 REGISTER_ENGINE(desc, clazz)
    * @param [in] desc nnnode描述信息对象
    * @param [in] clazz nnnode类名
    */
#define REGISTER_NN_NODE(desc, name) \
    std::shared_ptr<IAINNNode> NNNode_##name##_Creator() \
    { \
        return std::make_shared<name>(); \
    } \
    AINNNodeRegisterar g_nnnode_##name##_creator(desc,NNNode_##name##_Creator)
}
#endif

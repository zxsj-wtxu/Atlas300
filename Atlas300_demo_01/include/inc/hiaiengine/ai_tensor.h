/**
* @file ai_tensor.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef _AI_ENGINE_COMMON_TENSOR_H_
#define _AI_ENGINE_COMMON_TENSOR_H_

#include <vector>
#include <memory>
#include <mutex>
#include "ai_types.h"
#include "proto/ai_types.pb.h"

namespace hiai
{
    using namespace std;

    extern uint32_t AITensorGetBytes(int32_t data_type);

    /**
    * Tensor，用于表示输入输出的数据
    */
    class IAITensor
    {
    public:
        IAITensor() {}
        virtual ~IAITensor() {};

        /*
        * @brief 通过AITensorDescription设置参数
        * @param [in] tensor_desc tensor描述
        * @return true: init成功
        * @return false: init失败，失败的可能原因比如tensor_desc与当前tensor不符
        */
        virtual bool Init(const AITensorDescription &tensor_desc) = 0;

        /*
        * @brief 获取类型名称
        */
        virtual const char* const GetTypeName() = 0;

        /*
        * @brief  获取序列化后字节长度
        */
        virtual uint32_t ByteSizeLong() = 0;
        virtual void SetBufferAttr(void *data, int32_t size, bool isowner, bool is_support_mem_share) = 0;

        virtual bool IsSupportZerocpy() = 0;
    };

    typedef std::shared_ptr<IAITensor>(*CREATE_TENSOR_FUN)(const AITensorDescription &tensor_desc);

    class AITensorFactory
    {
    public:
        static AITensorFactory* GetInstance();

        /*
        * @brief 通过参数创建Tensor，包含分配内存
        * @param [in] tensor_desc 包含Tensor参数的描述信息
        * @return shared_ptr<IAITensor> 创建完成的Tensor指针，如果创建失败，则返回nullptr
        */
        std::shared_ptr<IAITensor> CreateTensor(const AITensorDescription &tensor_desc);

        /*
        * @brief 通过type创建Tensor，不分配内存，非预分配情况下使用
        * @param [in] type Tensor注册时的类型
        * @return shared_ptr<IAITensor> 创建完成的Tensor指针，如果创建失败，则返回nullptr
        */
        std::shared_ptr<IAITensor> CreateTensor(const std::string &type);

        std::shared_ptr<IAITensor> CreateTensor(const AITensorDescription &tensor_desc,
            void *buffer, int32_t size);
        /*
        * @brief 注册Tensor
        * @param [in] tensor_desc tensor描述
        * @param [in] create_func tensor创建函数
        */
        AIStatus RegisterTensor(const AITensorDescription &tensor_desc,
            CREATE_TENSOR_FUN create_func);
        AIStatus RegisterTensor(const string tensor_str, CREATE_TENSOR_FUN create_func)
        {
            // 加锁，防止多线程并发
            AITensorDescription tensor_desc;
            tensor_desc.set_type(tensor_str);
            return RegisterTensor(tensor_desc, create_func);
        }

       /*
        * @brief 卸载注册Tensor
        * @param [in] tensor_desc tensor描述
        */
        AIStatus UnRegisterTensor(const AITensorDescription &tensor_desc);
        AIStatus UnRegisterTensor(const string tensor_str)
        {
            AITensorDescription tensor_desc;
            tensor_desc.set_type(tensor_str);
            return UnRegisterTensor(tensor_desc);
        }

        /*
        * @brief 获取所有的Tensor列表
        * @param [out] tensor_desc_list 输出Tensor描述列表
        */
        void GetAllTensorDescription(AITensorDescriptionList &tensor_desc_list);

        /*
        * @brief 获取tensor描述
        */
        AIStatus GetTensorDescription(const std::string& tensor_type_name,
            AITensorDescription &tensor_desc);

    private:
        std::map<std::string, AITensorDescription> tensor_desc_map_;
        std::map<std::string, CREATE_TENSOR_FUN> create_func_map_;
        std::mutex tensor_reg_lock_;
    };
    class AITensorRegisterar
    {
    public:
        AITensorRegisterar(const AITensorDescription &tensor_desc, CREATE_TENSOR_FUN create_func) {
            AITensorFactory::GetInstance()->RegisterTensor(tensor_desc, create_func);
            tensor_desc_ = tensor_desc;
            tensor_str_.erase(tensor_str_.begin(), tensor_str_.end());
        }
        AITensorRegisterar(const string tensor_str, CREATE_TENSOR_FUN create_func) {
            AITensorFactory::GetInstance()->RegisterTensor(tensor_str, create_func);
            tensor_str_ = tensor_str;
        }
        ~AITensorRegisterar() {
            if (tensor_str_.empty()) {
                AITensorFactory::GetInstance()->UnRegisterTensor(tensor_desc_);
            } else {
                AITensorFactory::GetInstance()->UnRegisterTensor(tensor_str_);
            }
        }
    private:
        AITensorDescription tensor_desc_;
        std::string tensor_str_;
    };

#define REGISTER_TENSOR_CREATER_UNIQUE(desc, name, func) \
    AITensorRegisterar g_##name##_tensor_registerar(desc, func)

    /*
    * Tensor注册宏，在Tensor实现中使用
    */
#define REGISTER_TENSOR_CREATER(name, desc,func) \
        REGISTER_TENSOR_CREATER_UNIQUE(desc, name, func)

    // 简单类型的数据
    class AISimpleTensor : public IAITensor
    {
    public:
        AISimpleTensor();

        ~AISimpleTensor();

        /*
        * @brief 获取类型名称
        */
        virtual const char* const GetTypeName() override;

       /*获取数据地址*/
        void* GetBuffer();

        /*
        @brief 设置数据地址
        @param [in] data 数据指针
        @param [in] size 大小
        @param [in] isown 是否在tensor生命周期结束后，由tensor释放data地址内存
        */
        void SetBuffer(void *data, const int32_t size, bool isown=false);

        /*
        @brief 获取数据占用空间大小
        */
        uint32_t GetSize();

        /*
        @brief 分配数据空间
        */
        void* MallocDataBuffer(uint32_t size);

        /*
        * @brief  获取序列化后字节长度
        */
        virtual uint32_t ByteSizeLong() override;

        /*
        * @brief 通过AITensorDescription设置参数
        * @param [in] tensor_desc tensor描述
        * @return true: init成功
        * @return false: init失败，失败的可能原因比如tensor_desc与当前tensor不符
        */
        virtual bool Init(const AITensorDescription &tensor_desc) override;

        /*
        @brief 构建tensor描述
        */
        static AITensorDescription BuildDescription(const std::string& size = "");

        /*
        @brief 根据描述创建tensor
        */
        static std::shared_ptr<IAITensor> CreateTensor(const AITensorDescription& tensor_desc);

        bool IsSupportZerocpy();
        /*
        @brief 设置数据地址
        @param [in] data 数据指针
        @param [in] size 大小
        @param [in] isown 是否在tensor生命周期结束后，由tensor释放data地址内存
        @param [in] is_data_support_mem_share_ 是否在tensor生命周期结束后，由tensor释放data地址内存
        */
        void SetBufferAttr(void *data, int32_t size, bool isowner, bool is_support_mem_share) override;

    private:
        void* data_;
        uint32_t size_;
        bool isowner_;
        bool is_data_support_mem_share_;

        static std::string type_name_;
    };

    /**
    * 用于NN的通用Buffer定义
    */
    class AINeuralNetworkBuffer : public AISimpleTensor
    {
    public:
        AINeuralNetworkBuffer()
        {
            data_type_ = 0;
            number_    = 1;
            channel_   = 1;
            height_    = 1;
            width_     = 1;
            name_      = "";
        };

        /*
        * @brief 获取类型名称
        */
        const char* const GetTypeName();

        /*
        @brief 获取size字节大小
        */
        uint32_t ByteSizeLong();

        /*
        @brief 初始化
        @param [in] tensor_desc tensor描述
        */
        bool Init(const AITensorDescription &tensor_desc);

        /*
        * @brief 获取描述信息
        */
        static AITensorDescription GetDescription(
            const std::string &size = "0",const std::string &data_type="0",
            const std::string &number="0", const std::string &channel="0",
            const std::string &height="0", const std::string &width="0");

        /*
        @brief 创建tensor
        */
        static std::shared_ptr<IAITensor> CreateTensor(const AITensorDescription& tensor_desc);

        /*
        @brief 获取number数量
        */
        int32_t GetNumber();

        /*
        @brief 设置number
        */
        void SetNumber(int32_t number);

        /*
        @brief 获取channel数量
        */
        int32_t GetChannel();

        /*
        @brief 设置channel
        */
        void SetChannel(int32_t channel);

        /*
        @brief 获取height
        */
        int32_t GetHeight();

        /*
        @brief 设置height
        */
        void SetHeight(int32_t height);

        /*
        @brief 获取width
        */
        int32_t GetWidth();

        /*
        @brief 设置width
        */
        void SetWidth(int32_t width);

        /*
        @brief 获取数据类型
        */
        int32_t GetData_type();

        /*
        @brief 设置数据类型
        */
        void SetData_type(int32_t data_type);

        /*
        @brief 获取数据类型
        */
        const std::string& GetName() const;

        /*
        @brief 设置数据类型
        */
        void SetName(const std::string& value);

    private:
        int32_t data_type_;
        int32_t number_;
        int32_t channel_;
        int32_t height_;
        int32_t width_;
        std::string name_;
    };

    // 支持的图片格式
    enum ImageFormat
    {
        BITMAP,
        PNG,
        JPEG
    };

    /**
    * 用于图像的通用Buffer定义
    */
    class AIImageBuffer : public AISimpleTensor
    {
    public:
        AIImageBuffer()
        {
            format_ = JPEG;
            width_  = 0;
            height_ = 0;
        };

        /*
        * @brief 获取类型名称
        */
        const char* const GetTypeName();

        /*
        @brief 获取size字节大小
        */
        uint32_t ByteSizeLong();

        /*
        @brief 初始化
        @param [in] tensor_desc tensor描述
        */
        bool Init(const AITensorDescription &tensor_desc);

        /*
        * @brief 获取描述信息
        */
        static AITensorDescription GetDescription(
            const std::string &size = "0",const std::string &height="0",
            const std::string &width="0", const std::string format="JPEG");

        /*
        @brief 创建tensor
        */
        static std::shared_ptr<IAITensor> CreateTensor(const AITensorDescription& tensor_desc);

        /*
        @brief 获取图像格式
        */
        ImageFormat GetFormat();
        /*
        @brief 设置图像格式
        */
        void SetFormat(ImageFormat format);

        /*
        @brief 获取height
        */
        int32_t GetHeight();

        /*
        @brief 设置height
        */
        void SetHeight(int32_t height);

        /*
        @brief 获取width
        */
        int32_t GetWidth();

        /*
        @brief 设置width
        */
        void SetWidth(int32_t width);
    private:
        ImageFormat format_;
        int32_t width_;
        int32_t height_;
    };
}

#endif

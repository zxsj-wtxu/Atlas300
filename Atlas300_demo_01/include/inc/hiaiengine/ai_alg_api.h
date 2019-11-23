/**
* @file ai_alg_api.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef _HIAI_ALG_API_H_
#define _HIAI_ALG_API_H_

#include <vector>
#include <string>
#include <map>
#include <mutex>
#include "ai_types.h"
#include "proto/ai_types.pb.h"
#include "ai_tensor.h"

namespace hiai
{
    /*
    * 通用API原型定义
    */
    typedef AIStatus (*AI_ALG_API)(AIContext &context, const AIConfig &config_param, 
        const std::vector<std::shared_ptr<IAITensor>> &in_data, 
        std::vector<std::shared_ptr<IAITensor>> &out_data);

    class AIAlgAPIFactory
    {
    public:
        static AIAlgAPIFactory* GetInstance();

        /*
        * @brief 获取API
        * @param [in] name api名称
        * @return API原型指针
        */
        AI_ALG_API GetAPI(const std::string &name);

        /*
        * @brief 注册API
        * @param [in] desc api描述
        * @param [in] func api定义
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        AIStatus RegisterAPI(const AIAPIDescription &desc, AI_ALG_API func);

        /*
        * @brief 获取所有API描述
        * @param [in] api_desc_list api描述列表
        */
        void GetAllAPIDescription(AIAPIDescriptionList &api_desc_list);

        /*
        * @brief 卸载注册API
        * @param [in] api_desc api描述
        */
        AIStatus UnRegisterApi(const AIAPIDescription &api_desc);

        /*
        * @brief 获取API描述
        * @param [in] name api名称
        * @param [in] api_desc api描述
        * @return SUCCESS 成功
        * @return 其他    失败
        */
        AIStatus GetAPIDescription(const std::string &name, AIAPIDescription &api_desc);

    private:
        std::map<std::string, AI_ALG_API> func_map_;
        std::map<std::string, AIAPIDescription> desc_map_;
        std::mutex api_reg_lock_;
    };

    /*注册*/
    class AIAlgAPIRegisterar
    {
    public:
        AIAlgAPIRegisterar(const AIAPIDescription &desc, AI_ALG_API func)
        {
            AIAlgAPIFactory::GetInstance()->RegisterAPI(desc, func);
            api_desc_ = desc;
        }

        ~AIAlgAPIRegisterar()
        {
            AIAlgAPIFactory::GetInstance()->UnRegisterApi(api_desc_);
        }
    private:
        AIAPIDescription api_desc_;
    };

#define REGISTER_ALG_API_UNIQUE(desc, ctr, func) \
    AIAlgAPIRegisterar g_##ctr##_api_registerar(desc, func)

    /*
    * API注册宏，在API实现中使用
    */
#define REGISTER_ALG_API(name, desc, func) \
    REGISTER_ALG_API_UNIQUE(desc, name, func)
};

#endif

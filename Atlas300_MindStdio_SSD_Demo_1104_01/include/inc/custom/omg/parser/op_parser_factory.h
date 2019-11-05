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
#ifndef _DOMI_OMG_PARSER_OP_PARSER_FACTORY_H_
#define _DOMI_OMG_PARSER_OP_PARSER_FACTORY_H_

#include <string>
#include <memory>
#include <mutex>
#include <map>
#include <functional>
#include "common/types.h"
#include "../omg_types.h"

using namespace std;

namespace domi {

    class OpParser;

    /**
     * @ingroup domi_omg
     * @brief 用于创建OpParser
     *
     */
    class OpParserFactory {
    public:
        /**
         * @ingroup domi_omg
         * @brief 返回Framework对应的OpParserFactory实例
         * @return OpParserFactory对象
         */
        static shared_ptr<OpParserFactory> Instance(FrameworkType framework);

        /**
         * @ingroup domi_omg
         * @brief 根据输入的type，创建OpParser
         * @param [in] op_type Op类型
         * @return 创建出的OpParser
         */
        shared_ptr<OpParser> CreateOpParser(const std::string& op_type);

        /**
         * @ingroup domi_omg
         * @return 相同类型的OP map
         */
        const map<std::string, std::vector<std::string>>& GetSameTypeOps(){ return same_type_op_map_;};

        // Factory实例由shared_ptr自动释放，shared_ptr内部会间接调用析构函数，析构函数如果不公开会产生编译错误。
        // 另外的解决方法是为shared_ptr指定deleter，然后将deleter设置为当前类的友元，但这种方法实现较复杂。
        ~OpParserFactory() {};

    protected:
        /**
         * @ingroup domi_omg
         * @brief OpParser的创建函数
         * @return 创建出的OpParser
         */
        //typedef shared_ptr<OpParser> (*CREATOR_FUN)(void);
        using CREATOR_FUN = std::function<shared_ptr<OpParser>(void)>;

        /**
         * @ingroup domi_omg
         * @brief 工厂实例只能自动创建，不能通过new的方式创建，所以构造函数不对外公开
         */
        OpParserFactory() {}

        /**
         * @ingroup domi_omg
         * @brief 注册创建函数
         * @param [in] type Op类型
         * @param [in] fun OpParser的创建函数
         */
        void RegisterCreator(const std::string& type, CREATOR_FUN fun);

        /**
         * @ingroup domi_omg
         * @brief 注册相同类型的OP
         * @param [in] same_op_type Op类型,child_op_type candidates Op类型
         */
        void RegisterSameTypeOp(const std::string& same_op_type, const std::string& child_op_type);

    private:
        /**
         * @ingroup domi_omg
         * @brief 每个Op对应一个Creator函数
         */
        map<std::string, CREATOR_FUN> creator_map_;
        map<std::string, std::vector<std::string>> same_type_op_map_;

        friend class OpParserRegisterar;
        friend class OpRegistrationData;
        friend class SameTypeOPRegisterar;
    };

    /**
     * @ingroup domi_omg
     * @brief 用于为不同类型的Op注册Creator函数
     *
     */
    class OpParserRegisterar {
    public:
        /**
         * @ingroup domi_omg
         * @brief 构造函数
         * @param [in] framework    Framework类型
         * @param [in] op_type      Op类型
         * @param [in] fun          Op对应的Creator函数
         */
        OpParserRegisterar(FrameworkType framework, const std::string& op_type, OpParserFactory::CREATOR_FUN fun)
        {
            OpParserFactory::Instance(framework)->RegisterCreator(op_type, fun);
        }
        ~OpParserRegisterar(){}
    };

    /**
     * @ingroup domi_omg
     * @brief 用于为相同类型的Op注册Map
     * @author
     */
    class SameTypeOPRegisterar {
    public:
        /**
         * @ingroup domi_omg
         * @brief 构造函数
         * @param [in] framework    Framework类型
         * @param [in] op_type      Op类型
         * @param [in] fun          Op对应的Creator函数
         */
        SameTypeOPRegisterar(FrameworkType framework, const std::string& same_op_type, const std::string& child_op_type)
        {
            OpParserFactory::Instance(framework)->RegisterSameTypeOp(same_op_type, child_op_type);
        }
        ~SameTypeOPRegisterar(){}
    };

/**
 * @ingroup domi_omg
 * @brief OpParser的注册宏
 * @param [in] framework    Framework类型
 * @param [in] op_type      Op类型
 * @param [in] clazz        OpParser的实现类
 */
#define REGISTER_OP_PARSER_CREATOR(framework, op_type, clazz)               \
    shared_ptr<OpParser> Creator_##framework##_##op_type##_Op_Parser()      \
    {                                                                       \
        shared_ptr<clazz> ptr = nullptr;                           \
        try \
        { \
            ptr = make_shared<clazz>(); \
        } \
        catch(...) \
        { \
            ptr = nullptr; \
        } \
        return shared_ptr<OpParser>(ptr);                           \
    }                                                                       \
    OpParserRegisterar g_##framework##_##op_type##_Op_Parser_Creator(       \
            framework, op_type, Creator_##framework##_##op_type##_Op_Parser)

#define REGISTER_OP_PARSER_CREATOR_SAME_TYPE(framework, same_op_type, child_op_type, clazz) \
    SameTypeOPRegisterar g_##framework##_##child_op_type##_SameType_Map(                     \
            framework, same_op_type, child_op_type);                                         \
    REGISTER_OP_PARSER_CREATOR(framework, child_op_type, clazz)

} // end namespace domi
#endif //_DOMI_OMG_PARSER_OP_PARSER_FACTORY_H_

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
#ifndef _DOMI_OMG_PARSER_CAFFE_PARSER_REGISTER_H_
#define _DOMI_OMG_PARSER_CAFFE_PARSER_REGISTER_H_

#include "common/op_def/operator.h"
#include "omg/parser/caffe/caffe_op_parser.h"
#include "omg/parser/op_parser.h"
#include "omg/parser/op_parser_factory.h"
#include "proto/caffe/caffe.pb.h"
#include <functional>
#include "common/op_def/ir_pb_converter.h"

#include "mmpa/mmpa_api.h"

namespace domi
{

class Finalizeable
{
public:
    virtual bool Finalize() = 0;
    virtual ~Finalizeable(){};
};

class Receiver
{
public:
    Receiver(Finalizeable& f)
    {
        f.Finalize();
    }
    ~Receiver(){}
};

namespace caffe_parser
{
    template <typename Param>
    class CaffeParserBuilder;

    using ShapeMapFn =
        std::function<domi::Status(const domi::ShapeDef&, domi::ShapeDef*, const domi::OpDef*)>;

    struct WeightParserConfig
    {

        // 是否是必选的权值
        bool required_;

        // 设置caffe框架中该算子权值的shape
        uint32_t origin_shape_size_;

        // shape维度和原先的保持一致
        bool shape_use_origin_;

        ShapeMapFn shape_map_fn_;
    };

    class CaffeWeightParserBuilder : public Finalizeable
    {
    public:
        virtual ~CaffeWeightParserBuilder() {}

        virtual CaffeWeightParserBuilder& AddWeightParserConfig() = 0;

        virtual CaffeWeightParserBuilder& Required() = 0;

        virtual CaffeWeightParserBuilder& Optional() = 0;

        virtual CaffeWeightParserBuilder& OriginShapeSize(uint32_t origin_shape_size) = 0;

        virtual CaffeWeightParserBuilder& ShapeUseOrigin() = 0;

        virtual CaffeWeightParserBuilder& SetShapeMapFn(ShapeMapFn shape_map_fn) = 0;
    };

    template <typename Param>
    class CaffeOpParserAdapter;

    template <typename Param>
    class CaffeParserBuilder : public CaffeWeightParserBuilder
    {
    public:
        using ParseParamsFn = std::function<domi::Status(const ::caffe::LayerParameter*, Param*)>;
        using ParseWeightsFn =
            std::function<domi::Status(const ::caffe::LayerParameter*, domi::OpDef*)>;

        CaffeParserBuilder(const std::string& om_optype) : om_optype_(om_optype) {}
        virtual ~CaffeParserBuilder() {};

        CaffeParserBuilder& SetParseParamsFn(ParseParamsFn parse_params_fn)
        {
            parse_params_fn_ = parse_params_fn;
            return *this;
        }

        CaffeWeightParserBuilder& AddWeightParserConfig()
        {
            weight_parser_configs_.push_back(WeightParserConfig());
            return *this;
        }

        virtual CaffeWeightParserBuilder& Required() override
        {
            weight_parser_configs_.back().required_ = true;
            return *this;
        }

        virtual CaffeWeightParserBuilder& Optional() override
        {
            weight_parser_configs_.back().required_ = false;
            return *this;
        }

        virtual CaffeWeightParserBuilder& OriginShapeSize(uint32_t origin_shape_size) override
        {
            weight_parser_configs_.back().origin_shape_size_ = origin_shape_size;
            return *this;
        }

        virtual CaffeWeightParserBuilder& ShapeUseOrigin() override
        {
            weight_parser_configs_.back().shape_use_origin_ = true;
            return *this;
        }

        virtual CaffeWeightParserBuilder& SetShapeMapFn(ShapeMapFn shape_map_fn) override
        {
            weight_parser_configs_.back().shape_map_fn_ = shape_map_fn;
            return *this;
        }

        CaffeParserBuilder& SetParseWeightsFn(ParseWeightsFn parser_weights_fn)
        {
            parser_weights_fn_ = parser_weights_fn;
            return *this;
        }

        virtual bool Finalize() override
        {
            std::shared_ptr<CaffeOpParserAdapter<Param>> op_parser_adapter = nullptr;
            try
            {
                op_parser_adapter = std::make_shared<CaffeOpParserAdapter<Param>>(*this);
            }
            catch(...)
            {
                op_parser_adapter = nullptr;
            }
            // 注册到OpParserFactory
            domi::OpParserRegisterar registerar __attribute__((unused)) =
                domi::OpParserRegisterar(CAFFE, om_optype_, [=] {
                    return std::shared_ptr<OpParser>(op_parser_adapter);
                });
            return true;
        }

    private:
        std::string om_optype_;  // om模型中的op type

        ParseParamsFn  parse_params_fn_;
        ParseWeightsFn parser_weights_fn_;

        std::vector<WeightParserConfig> weight_parser_configs_;

        friend class CaffeOpParserAdapter<Param>;
    };

    template <typename Param>
    class CaffeOpParserAdapter : public CaffeOpParser
    {
        using ParseParamsFn = std::function<domi::Status(const ::caffe::LayerParameter*, Param*)>;
        using ParseWeightsFn =
            std::function<domi::Status(const ::caffe::LayerParameter*, domi::OpDef*)>;

    public:
        CaffeOpParserAdapter(CaffeParserBuilder<Param> builder)
        {
            parse_params_fn_       = builder.parse_params_fn_;
            parser_weights_fn_     = builder.parser_weights_fn_;
            weight_parser_configs_ = builder.weight_parser_configs_;
        }
        virtual ~CaffeOpParserAdapter() {};

        virtual Status ParseParams(const Message* op_src, OpDef* op_dest) override
        {
            if (op_src == nullptr || op_dest == nullptr){
                return domi::PARAM_INVALID;
            }
            const ::caffe::LayerParameter* layer =
                DOMI_DYNAMIC_CAST<const ::caffe::LayerParameter*>(op_src);

            if (layer == nullptr){
                return domi::PARAM_INVALID;
            }
            std::shared_ptr<Param> param = nullptr;
            try
            {
                param = std::make_shared<Param>();
            }
            catch(...)
            {
                return domi::FAILED;
            }
            ::domi::Status result = parse_params_fn_(layer, param.get());
            if (domi::SUCCESS != result){
                return result;
            }
            param.get()->Name(layer->name());
            std::shared_ptr<Operator> op_param = static_pointer_cast<Operator>(param);

            result = ConvertToOpDef(*op_param, op_dest);
            if (domi::SUCCESS != result){
                return result;
            }
            return domi::SUCCESS;
        }

        virtual Status ParseWeights(const Message* op_src, OpDef* op_dest) override
        {
            if (op_src == nullptr || op_dest == nullptr){
                return domi::PARAM_INVALID;
            }
            const ::caffe::LayerParameter* layer =
                static_cast<const ::caffe::LayerParameter*>(op_src);
            if(parser_weights_fn_ != nullptr)
            {
                return parser_weights_fn_(layer, op_dest);
            }

            uint32_t src_blob_size = layer->blobs_size();
            for(uint32_t i = 0; i < weight_parser_configs_.size(); i++)
            {
                auto& parser_config = weight_parser_configs_[ i ];
                if(i >= src_blob_size)
                {
                    if(parser_config.required_)  // 权值必填场景下，报错
                    {
                        return domi::PARAM_INVALID;
                    }
                    else  // 权值可选场景下，直接返回
                    {
                        break;
                    }
                }

                WeightDef* weight = op_dest->add_weights();

                ::domi::Status result = ConvertWeight(layer->blobs(i), weight);
                if (domi::SUCCESS != result){
                    return result;
                }

                ShapeDef src_shape;
                src_shape.CopyFrom(weight->shape());
                if(src_shape.dim_size() != (int32_t)parser_config.origin_shape_size_)
                {
                    return domi::PARAM_INVALID;
                }

                if(!parser_config.shape_use_origin_)
                {
                    weight->mutable_shape()->clear_dim();
                    parser_config.shape_map_fn_(src_shape, weight->mutable_shape(), op_dest);
                }

                if(weight->shape().dim_size() != 4)
                {
                    return domi::PARAM_INVALID;
                }
            }

            return domi::SUCCESS;
        }

    private:
        ParseParamsFn  parse_params_fn_;
        ParseWeightsFn parser_weights_fn_;

        std::vector<WeightParserConfig> weight_parser_configs_;
    };

}  // end namespace caffe_parser

#define DOMI_REGISTER_CAFFE_PARSER(name, param_clazz) \
    DOMI_REGISTER_CAFFE_PARSER_UNIQ_HELPER(__COUNTER__, name, param_clazz)
#define DOMI_REGISTER_CAFFE_PARSER_UNIQ_HELPER(ctr, name, param_clazz) \
    DOMI_REGISTER_CAFFE_PARSER_UNIQ(ctr, name, param_clazz)
#define DOMI_REGISTER_CAFFE_PARSER_UNIQ(ctr, name, param_clazz)          \
    static Receiver register_caffe_parser##ctr __attribute__((unused)) = \
        ::domi::caffe_parser::CaffeParserBuilder<param_clazz>(name)

}  // namespace domi
// end namespace domi

#endif  //_DOMI_OMG_PARSER_CAFFE_PARSER_REGISTER_H_

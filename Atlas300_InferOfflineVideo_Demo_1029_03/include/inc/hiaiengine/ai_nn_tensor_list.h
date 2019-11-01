/**
* @file ai_nn_tensor_list.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_AI_NN_TENSOR_LIST_H_
#define INC_HIAIENGINE_AI_NN_TENSOR_LIST_H_

#include <stdint.h>
#include <vector>
#include "hiaiengine/ai_types.h"
#include "hiaiengine/ai_tensor.h"
#include "hiaiengine/status.h"

namespace hiai {
    struct NNTensorDescription {
        uint32_t dataSize;           // data length
        uint32_t tensorNumber;
        uint32_t channels;
        uint32_t height;
        uint32_t width;
        uint32_t dataType;
        std::string name;            // Tensor name
        void* data;  // 数据指针
    };

    class NNTensorList {
     public:
        /**
        * @ingroup hiaiengine
        * @brief  构造函数
        * @param 无
        * @return 无
        */
        HIAI_LIB_INTERNAL NNTensorList();

        /**
        * @ingroup hiaiengine
        * @brief  析构函数
        * @param 无
        * @return 无
        */
        HIAI_LIB_INTERNAL ~NNTensorList();

        /**
        * @ingroup hiaiengine
        * @brief  Python端调用该接口，同步数据内容和大小，为后续Host和Device侧数据Copy做准备
        * @param  std::vector<hiai::TensorDimension>
        * @return HIAI_StatusT
        */
        HIAI_LIB_INTERNAL HIAI_StatusT Resize(std::vector<hiai::TensorDimension> &tensorDimVec);

        /**
        * @ingroup hiaiengine
        * @brief  获取DMalloc分配的内存首地址
        * @param  null
        * @return void*
        */
        HIAI_LIB_INTERNAL const void* GetBuffData() const;

        /**
        * @ingroup hiaiengine
        * @brief  设置DMalloc分配的内存首地址
        * @param  null
        * @return void*
        */
        HIAI_LIB_INTERNAL void SetBuffData(void* dataPtr);

        /**
        * @ingroup hiaiengine
        * @brief  获取NNTensorList所占的内存大小
        * @param  null
        * @return uint64_t
        */
        HIAI_LIB_INTERNAL uint64_t GetBufferSize() const;

        /**
        * @ingroup hiaiengine
        * @brief  设置NNTensorList所占的内存大小
        * @param  uint64_t
        * @return void
        */
        HIAI_LIB_INTERNAL void SetBufferSize(const uint64_t size);

        /**
        * @ingroup hiaiengine
        * @brief  根据tensorIndex获取对应的AINeuralNetworkBuffer
        * @param  uint32_t
        * @return std::shared_ptr<hiai::AINeuralNetworkBuffer>
        * Note：NNTensor保存图像元数据信息和内容
        */
        HIAI_LIB_INTERNAL std::shared_ptr<hiai::AINeuralNetworkBuffer> GetNNTensor(const uint32_t tensorIndex);

        /**
        * @ingroup hiaiengine
        * @brief  获取NNTesnorList保存的NNTensor总数
        * @param  null
        * @return uint32_t
        */
        HIAI_LIB_INTERNAL uint32_t GetNNTensorNumber() const;

        /**
        * @ingroup hiaiengine
        * @brief  设置NNTesnorList保存的NNTensor总数
        * @param  uint32_t
        * @return null
        */
        HIAI_LIB_INTERNAL void SetNNTensorNumber(const uint32_t tmpNum);

        /**
        * @ingroup hiaiengine
        * @brief  获取NNTensorVector
        * @param  null
        * @return std::vector<NNTensorDescription>
        */
        HIAI_LIB_INTERNAL const std::vector<NNTensorDescription>& GetNNTensorVector() const;

        /**
        * @ingroup hiaiengine
        * @brief  设置NNTensorVector
        * @param  std::vector<NNTensorDescription>
        * @return null
        */
        HIAI_LIB_INTERNAL void SetNNTensorVector(const std::vector<NNTensorDescription>& tmpNNTensorVec);

        /**
        * @ingroup hiaiengine
        * @brief  把一段连续地址空间转换成NNTensorDescription对应的地址空间
        *         反序列化过程中使用该接口
        * @param  null
        * @return HIAI_StatusT
        */
        HIAI_LIB_INTERNAL void AlignToNNTensorDescription();

     private:
        /**
        * @ingroup hiaiengine
        * @brief  根据tensorIndex设置对应的NNTensorDescription data和data Size
        * @param  uint32_t
        * @return NNTensorDescription
        */
        HIAI_LIB_INTERNAL void SetNNTensorDescriptionData(const uint32_t tensorIndex, void* dataPtr, uint64_t dataSize);

        /**
        * @ingroup hiaiengine
        * @brief  把NNTensorDescription转换成AINeuralNetworkBuffer
        * @param  std::shared_ptr<hiai::AINeuralNetworkBuffer>
        * @param  NNTensorDescription
        * @return HIAI_StatusT
        */
        HIAI_LIB_INTERNAL HIAI_StatusT TensorDescrToNNTensor(const NNTensorDescription& tensorDescr,
                                       std::shared_ptr<hiai::AINeuralNetworkBuffer>& nnTensor);

        HIAI_LIB_INTERNAL HIAI_StatusT NNTensorToTensorDescr(
                                       const std::shared_ptr<hiai::AINeuralNetworkBuffer>& nnTensor,
                                       NNTensorDescription& tensorDescr);

        /**
        * @ingroup hiaiengine
        * @brief  把TensorDimension转换成NNTensorDescription
        * @param  hiai::TensorDimension
        * @param  NNTensorDescription
        * @return HIAI_StatusT
        */
        HIAI_LIB_INTERNAL HIAI_StatusT TensorDimensionToTensorDescr(const hiai::TensorDimension& tensorDim,
                                              NNTensorDescription& tensorDescr);
        /**
        * @ingroup hiaiengine
        * @brief  统计NNTensorList的Buffer Size用来进行数据分配
        * @param  std::vector<hiai::TensorDimension>
        * @return uint64_t
        */
        HIAI_LIB_INTERNAL uint64_t ComputeTotalTensorBufferSize(
            std::vector<hiai::TensorDimension> &tensorDimVec);

     private:
        void* buffData_;
        uint64_t bufferSize_;
        uint32_t tensorNumber_;
        std::vector<NNTensorDescription> nnTensorVec_;
    };

}
#endif


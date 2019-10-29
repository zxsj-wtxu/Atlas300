/**
*
* Copyright(c)<2018>, <Huawei Technologies Co.,Ltd>
*
* @version 1.0
*
* @date 2018-5-21
*/
#ifndef MindINFERENCEENGINE_ENGINE_H_
#define MindINFERENCEENGINE_ENGINE_H_
#include "hiaiengine/api.h"
#include "hiaiengine/ai_model_manager.h"
#include "hiaiengine/ai_types.h"
#include "hiaiengine/data_type.h"
#include "hiaiengine/engine.h"
#include "hiaiengine/multitype_queue.h"
#include "hiaiengine/data_type_reg.h"
#include "hiaiengine/ai_tensor.h"
#include "BatchImageParaWithScale.h"

#define INPUT_SIZE 2
#define OUTPUT_SIZE 1
using hiai::Engine;

class MindInferenceEngine_1 : public Engine {
public:
    MindInferenceEngine_1() :
        batch_size(0),input_que_(INPUT_SIZE-1),image_handle(nullptr),ai_model_manager_(nullptr), tran_data(nullptr),
        dataInputIn_(nullptr), modelName_("ClassifyModel"){}
    ~MindInferenceEngine_1(){}
    HIAI_StatusT Init(const hiai::AIConfig& config, const  std::vector<hiai::AIModelDescription>& model_desc);
    /**
    * @brief HIAI_DEFINE_PROCESS : override Engine Process logic
    * @[in]: define a input port, a output port
    */
   HIAI_DEFINE_PROCESS(INPUT_SIZE, OUTPUT_SIZE);
private:
    uint32_t batch_size;
    bool dynamic_aipp_flag;
    hiai::AippInputFormat input_image_format;
    bool csc_switch;
    hiai::AippModelFormat model_image_format;
    int32_t input_image_width;
    int32_t input_image_height;
    int32_t crop_width;
    int32_t crop_height;

    // Private implementation a member variable, which is used to cache the input queue
    hiai::MultiTypeQueue input_que_;
    std::shared_ptr<BatchImageParaWithScaleT> image_handle;
    std::vector<std::shared_ptr<hiai::IAITensor>> input_data_vec;
    std::vector<std::shared_ptr<hiai::IAITensor>> output_data_vec;
    std::shared_ptr<hiai::AIModelManager> ai_model_manager_;
    std::shared_ptr<EngineTransT> tran_data;
    std::shared_ptr<BatchImageParaWithScaleT> dataInputIn_;
    std::mutex memoryRecursiveMutex_;
    std::vector<uint8_t*> m_outData_;
    std::string modelName_;
    std::vector<std::map<uint8_t *, int>> predict_input_data_; // map for storing data from argX, first:buffer, second:buffer's size
    std::vector<hiai::TensorDimension> inputTensorVec;
    std::vector<hiai::TensorDimension> outputTensorVec;

    /**
    * @brief: handle the exceptions when the dataset batch failed
    * @in: error_msg: the error message
    */
    void HandleExceptions(std::string error_msg);

    /**
    * @brief: set the frame ID as -1 to indicate this model batch failed
    * @in: index of the begin of this batch
    */
    void HandleModelBatchFailure(const int batch_begin,const int image_number);

    /**
    * @brief: send sentinel image to inform the graph to destroy
    */
    HIAI_StatusT SendSentinelImage();

    /**
    * @brief: prepare the data buffer for image information
    * @in: input_buffer: buffer pointer
    * @in: image_number: total number of received images
    * @in: batch_begin: the index of the first image of each batch
    * @in: image_size: size of each image
    * @return: HIAI_StatusT
    */
    HIAI_StatusT PrepareInputBuffer(uint8_t *input_buffer, const int image_number, const int batch_begin, const int image_size);

    /**
    * @brief: prepare the data buffer for image information
    * @in: input_buffer2: buffer pointer
    * @in: image_number: total number of received images
    * @in: batch_begin: the index of the first image of each batch
    * @in: multi_input_2: the second input received from the previous engine
    * @return: HIAI_StatusT
    */
    HIAI_StatusT PrepareInforInput(uint8_t *input_buffer2, const int image_number, const int batch_begin, std::shared_ptr<hiai::BatchRawDataBuffer> multi_input_2);

    /**
    * @brief: clear buffer in m_outData_ vector
    */
    void ClearOutData();

    /**
    * @brief: call ai model manager to do the prediction
    * @return: HIAI_StatusT
    */
    HIAI_StatusT Predict();

    /**
    * @brief: set the tran_data with the result of this model batch
    * @in: index of the begin of this batch
    * @return: HIAI_StatusT
    */
    HIAI_StatusT SetOutputStruct(const int batch_begin);

    /**
    * @brief: send the predicted result for one batch
    */
    void SendResult();
};

#define MULTI_INPUT_ARG_PUSH(index) \
    std::shared_ptr<hiai::BatchRawDataBuffer> _multi_input_##index; \
    input_que_.PushData(index-1, arg##index) \

#define MULTI_INPUT_ARG_POP_3 \
    MULTI_INPUT_ARG_PUSH(2); \
do{ \
    if (!input_que_.PopAllData(image_handle, _multi_input_2)) \
    { \
        HIAI_ENGINE_LOG("[MindInferenceEngine_1] fail to PopAllData"); \
        return HIAI_ERROR; \
    } \
}while(0)

#define MULTI_INPUT_ARG_POP_4 \
    MULTI_INPUT_ARG_PUSH(2); \
    MULTI_INPUT_ARG_PUSH(3); \
do{ \
    if (!input_que_.PopAllData(image_handle, _multi_input_2, _multi_input_3)) \
    { \
        HIAI_ENGINE_LOG("[MindInferenceEngine_1] fail to PopAllData"); \
        return HIAI_ERROR; \
    } \
}while(0)

#define MULTI_INPUT_ARG_POP_5 \
    MULTI_INPUT_ARG_PUSH(2); \
    MULTI_INPUT_ARG_PUSH(3); \
    MULTI_INPUT_ARG_PUSH(4); \
do{ \
    if (!input_que_.PopAllData(image_handle, _multi_input_2, _multi_input_3, _multi_input_4)) \
    { \
        HIAI_ENGINE_LOG("[MindInferenceEngine_1] fail to PopAllData"); \
        return HIAI_ERROR; \
    } \
}while(0)

#define MULTI_INPUT_ARG_POP_6 \
    MULTI_INPUT_ARG_PUSH(2); \
    MULTI_INPUT_ARG_PUSH(3); \
    MULTI_INPUT_ARG_PUSH(4); \
    MULTI_INPUT_ARG_PUSH(5); \
do{ \
    if (!input_que_.PopAllData(image_handle, _multi_input_2, _multi_input_3, _multi_input_4, _multi_input_5)) \
    { \
        HIAI_ENGINE_LOG("[MindInferenceEngine_1] fail to PopAllData"); \
        return HIAI_ERROR; \
    } \
}while(0)

#define MULTI_INPUT_ARG_POP_7 \
    MULTI_INPUT_ARG_PUSH(2); \
    MULTI_INPUT_ARG_PUSH(3); \
    MULTI_INPUT_ARG_PUSH(4); \
    MULTI_INPUT_ARG_PUSH(5); \
    MULTI_INPUT_ARG_PUSH(6); \
do{ \
    if (!input_que_.PopAllData(image_handle, _multi_input_2, _multi_input_3, _multi_input_4, _multi_input_5, \
        _multi_input_6)) \
    { \
        HIAI_ENGINE_LOG("[MindInferenceEngine_1] fail to PopAllData"); \
        return HIAI_ERROR; \
    } \
}while(0)

#define MULTI_INPUT_ARG_POP_8 \
    MULTI_INPUT_ARG_PUSH(2); \
    MULTI_INPUT_ARG_PUSH(3); \
    MULTI_INPUT_ARG_PUSH(4); \
    MULTI_INPUT_ARG_PUSH(5); \
    MULTI_INPUT_ARG_PUSH(6); \
    MULTI_INPUT_ARG_PUSH(7); \
do{ \
    if (!input_que_.PopAllData(image_handle, _multi_input_2, _multi_input_3, _multi_input_4, _multi_input_5, \
        _multi_input_6, _multi_input_7)) \
    { \
        HIAI_ENGINE_LOG("[MindInferenceEngine_1] fail to PopAllData"); \
        return HIAI_ERROR; \
    } \
}while(0)

#define MULTI_INPUT_ARG_POP_9 \
    MULTI_INPUT_ARG_PUSH(2); \
    MULTI_INPUT_ARG_PUSH(3); \
    MULTI_INPUT_ARG_PUSH(4); \
    MULTI_INPUT_ARG_PUSH(5); \
    MULTI_INPUT_ARG_PUSH(6); \
    MULTI_INPUT_ARG_PUSH(7); \
    MULTI_INPUT_ARG_PUSH(8); \
do{ \
    if (!input_que_.PopAllData(image_handle, _multi_input_2, _multi_input_3, _multi_input_4, _multi_input_5, \
        _multi_input_6, _multi_input_7, _multi_input_8)) \
    { \
        HIAI_ENGINE_LOG("[MindInferenceEngine_1] fail to PopAllData"); \
        return HIAI_ERROR; \
    } \
}while(0)

#define MULTI_INPUT_ARG_POP_10 \
    MULTI_INPUT_ARG_PUSH(2); \
    MULTI_INPUT_ARG_PUSH(3); \
    MULTI_INPUT_ARG_PUSH(4); \
    MULTI_INPUT_ARG_PUSH(5); \
    MULTI_INPUT_ARG_PUSH(6); \
    MULTI_INPUT_ARG_PUSH(7); \
    MULTI_INPUT_ARG_PUSH(8); \
    MULTI_INPUT_ARG_PUSH(9); \
do{ \
    if (!input_que_.PopAllData(image_handle, _multi_input_2, _multi_input_3, _multi_input_4, _multi_input_5, \
        _multi_input_6, _multi_input_7, _multi_input_8, _multi_input_9)) \
    { \
        HIAI_ENGINE_LOG("[MindInferenceEngine_1] fail to PopAllData"); \
        return HIAI_ERROR; \
    } \
}while(0)

#define DEFINE_MULTI_INPUT_ARGS_POP(index) MULTI_INPUT_ARG_POP_##index

#define MULTI_INPUT_ARG(index) \
do{ \
    std::shared_ptr<hiai::AINeuralNetworkBuffer> _neural_buffer##index = \
        std::make_shared<hiai::AINeuralNetworkBuffer>(); \
    _neural_buffer##index->SetBuffer((void*)(_input_buffer##index), \
        (uint32_t)(_input_buffer##index##_size), false); \
    std::shared_ptr<hiai::IAITensor> _input_data##index = \
        std::static_pointer_cast<hiai::IAITensor>(_neural_buffer##index); \
    input_data_vec.push_back(_input_data##index); \
}while(0)

#define MULTI_INPUT_ARGS_2 \
    MULTI_INPUT_ARG(1);

#define MULTI_INPUT_ARGS_3 \
    MULTI_INPUT_ARG(1); \
    MULTI_INPUT_ARG(2);

#define MULTI_INPUT_ARGS_4 \
    MULTI_INPUT_ARG(1); \
    MULTI_INPUT_ARG(2); \
    MULTI_INPUT_ARG(3);

#define MULTI_INPUT_ARGS_5 \
    MULTI_INPUT_ARG(1); \
    MULTI_INPUT_ARG(2); \
    MULTI_INPUT_ARG(3); \
    MULTI_INPUT_ARG(4);

#define MULTI_INPUT_ARGS_6 \
    MULTI_INPUT_ARG(1); \
    MULTI_INPUT_ARG(2); \
    MULTI_INPUT_ARG(3); \
    MULTI_INPUT_ARG(4); \
    MULTI_INPUT_ARG(5);

#define MULTI_INPUT_ARGS_7 \
    MULTI_INPUT_ARG(1); \
    MULTI_INPUT_ARG(2); \
    MULTI_INPUT_ARG(3); \
    MULTI_INPUT_ARG(4); \
    MULTI_INPUT_ARG(5); \
    MULTI_INPUT_ARG(6);

#define MULTI_INPUT_ARGS_8 \
    MULTI_INPUT_ARG(1); \
    MULTI_INPUT_ARG(2); \
    MULTI_INPUT_ARG(3); \
    MULTI_INPUT_ARG(4); \
    MULTI_INPUT_ARG(5); \
    MULTI_INPUT_ARG(6); \
    MULTI_INPUT_ARG(7);

#define MULTI_INPUT_ARGS_9 \
    MULTI_INPUT_ARG(1); \
    MULTI_INPUT_ARG(2); \
    MULTI_INPUT_ARG(3); \
    MULTI_INPUT_ARG(4); \
    MULTI_INPUT_ARG(5); \
    MULTI_INPUT_ARG(6); \
    MULTI_INPUT_ARG(8);

#define MULTI_INPUT_ARGS_10 \
    MULTI_INPUT_ARG(1); \
    MULTI_INPUT_ARG(2); \
    MULTI_INPUT_ARG(3); \
    MULTI_INPUT_ARG(4); \
    MULTI_INPUT_ARG(5); \
    MULTI_INPUT_ARG(6); \
    MULTI_INPUT_ARG(8); \
    MULTI_INPUT_ARG(9);

#define DEFINE_MULTI_INPUT_ARGS(index) MULTI_INPUT_ARGS_##index

#endif

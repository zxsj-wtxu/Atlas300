/**
*
* Copyright(c)<2018>, <Huawei Technologies Co.,Ltd>
*
* @version 1.0
*
* @date 2018-4-25
*/
#ifndef SSDPostProcess_1_ENGINE_H_
#define SSDPostProcess_1_ENGINE_H_
#include "hiaiengine/api.h"
#include "hiaiengine/data_type.h"
#include "hiaiengine/engine.h"
#include "hiaiengine/data_type_reg.h"
#include <hiaiengine/multitype_queue.h>
#include "BatchImageParaWithScale.h"
#include <unordered_map>

#define INPUT_SIZE 1
#define OUTPUT_SIZE 1

using hiai::Engine;
using namespace hiai;

class SSDPostProcess_1 : public Engine {
private:
    typedef struct PostprocessConfig_{
        std::string path;
        std::string mode;
        std::string info_file;
        std::vector<bool> port_list;
        std::unordered_map<float, std::vector<std::pair<int, float> > > cls_port_dict;
        std::string output_node;
        PostprocessConfig_() :path(""), info_file(""), port_list(OUTPUT_SIZE - 1, false), output_node(""){};
    }PostprocessConfig;
public:
    SSDPostProcess_1() :
        store_path(""), input_que_(INPUT_SIZE), postprocess_config_(nullptr), batchDetected(nullptr) {}
    ~SSDPostProcess_1(){}
    HIAI_StatusT Init(const hiai::AIConfig& config, const  std::vector<hiai::AIModelDescription>& model_desc);
    /**
    * @brief HIAI_DEFINE_PROCESS : override Engine Process logic
    * @[in]: define a input port, a output port
    */
    HIAI_DEFINE_PROCESS(INPUT_SIZE, OUTPUT_SIZE)
private:
    const std::string RESULT_FOLDER = "result_files";
    const std::string ENGINE_NAME = "SSDPostProcess_1";
    std::string store_path;
    std::unordered_map<int, ImageInfor> id_img_correlation;
    hiai::MultiTypeQueue input_que_;
    std::shared_ptr<PostprocessConfig> postprocess_config_;
    std::shared_ptr<hiai::BatchDetectedObjectPara<hiai::Rectangle<Point2D>, float>> batchDetected;
    std::unordered_map<int, ObjectLocation<Rectangle<Point2D>, float> > object_locs;
    std::unordered_map<int, std::shared_ptr<hiai::BatchDetectedObjectPara<hiai::Rectangle<Point2D>, float>> > outputs;


    /**
    * @brief: get the output ports configuration
    * @[in]: value, the port setting string, format is 'attribute,confidence,port',e.g.: "14,50,0;7,70,1;"
    */
    void SetOutputFilter(const std::string value);

    /**
    * @brief: write the detection result as json format file
     * @[in]: img_infor: image information
    * @[in]: attribute_number: predicted classes of the boxes in one image
    * @[in]: confidence: predicted confidence of the boxes in one image
    * @[in]: bbox: coordinates of all boxes in one image
    * @
    */
    void WriteResultFile(ImageInfor img_infor, const std::vector<float>& attribute_number, const std::vector<float>& confidence, const std::vector<hiai::Rectangle<Point2D>>& bbox);

    /**
    * @brief: init the output structs according to port settings
    * @in: tran: output struct received from previous engine
    */
    void InitOutputs(std::shared_ptr<EngineTransT> tran);

    /**
    * @brief: filt out the detection output according to the tensor name
    * @[in]: out_num: struct of the number of detected bboxes
    * @[in]: out_bbox: struct of the detected content, including bbox coordinates, predicted classes and confidence
    * @[in]: output_data_vec: output of inference engine for one batch
    */
    void GetRealInputs(OutputT& out_num, OutputT& out_bbox, const std::vector<OutputT>& output_data_vec);

    /**
    * @brief: resolve the detection results from buffer and set the engine outputs struct
    * @[in]: ptr: pointer in the buffer, indicates start of one bbox in the buffer
    * @[in]: attribute_number: predicted classes of the boxes in one image
    * @[in]: confidence: predicted confidence of the boxes in one image
    * @[in]: bbox: coordinates of all boxes in one image
    * @[in]: crop: transformation information
    */
    void ResolveBBox(float* ptr, std::vector<float>& attribute_number, std::vector<float>& confidence, std::vector<hiai::Rectangle<Point2D>>& bbox, const CropInfo& crop);

    /**
    * @brief: resolve the detection results for caffe engine
    * @[in]: ptr: pointer in the buffer, indicates start of one bbox in the buffer
    * @[in]: ptrSize: ptr array size
    * @[in]: img_infor: image information
    * @[in]: attribute_number: predicted classes of the boxes in one image
    * @[in]: confidence: predicted confidence of the boxes in one image
    * @[in]: bbox: coordinates of all boxes in one image
    * @[in]: object_loc: the output struct
    */
    void CaffeResolveBBox(float* ptr, int ptrSize, const ImageInfor img_infor, std::vector<float>& attribute_number, std::vector<float>& confidence, std::vector<hiai::Rectangle<Point2D>>& bbox, ObjectLocation<Rectangle<Point2D>, float>& object_loc);

    /**
    * @brief: post process for caffe engine
    * @in: output_data_vec: output tensor of the previous engine
    * @in: tran: output struct received from previous engine
    */
    HIAI_StatusT CaffeSSDProcess(std::vector<OutputT> &output_data_vec, std::shared_ptr<EngineTransT> tran);

    /**
    * @brief: build the outputs struct of all images in one batch
    */
    void BuildBatch();

    /**
    * @brief: send the outputs struct to corresponding port
    */
    void SendDataToPorts();

    /**
    * @brief: handle the exceptions, i.e. send empty batchDetected struct to all the ports
    * @in: output_size: the number of ports plus one (there's one port reserved for recall functor)
    */
    void HandleExceptions(int output_size);

    /**
    * @brief: check status and print error log
    * @in: ret_status: hiaiengine defined return status code
    * @in: error_msg: the error log string
    */
    void CheckStatus(HIAI_StatusT ret_status, std::string error_msg);

    /**
    * @brief: send sentinel image to inform the graph to destroy
    */
    HIAI_StatusT SendSentinelImage();

    /**
    * @brief: reset the cropinfo to make sure the output bbox coordinates are always relative to the original image
    * @in: crop: the crop info got from preprocess, which will be reseted for later transformation process
    * @in: img_infor: the image information of the current image
    */
    void SetTransformationInfor(CropInfo& crop, const ImageInfor img_infor);
};
#endif


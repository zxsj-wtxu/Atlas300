/**
*
* Copyright(c)<2018>, <Huawei Technologies Co.,Ltd>
*
* @version 1.0
*
* @date 2018-5-30
*/
#include "SSDPostProcess_1.h"
#include <hiaiengine/log.h>
#include <vector>
#include <thread>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <cmath>
#include <fcntl.h>

static const int g_sizePerSSD = 7; //caffe inference uses 7 float to show every detection info

/**
* @brief: get the output ports configuration
* @[in]: value, the port setting string, format is 'attribute,confidence,port',e.g.: "14,50,0;7,70,1;"
*/
void SSDPostProcess_1::SetOutputFilter(const std::string value) {
    std::string output_config_text(value.begin(), --value.end());
    std::stringstream output_config_ss(output_config_text);
    std::string cls_config;
    while (std::getline(output_config_ss, cls_config, ';')) {
        std::stringstream cls_config_ss(cls_config);
        int port, cls;
        float confidence;
        while (cls_config_ss >> cls) {
            cls_config_ss.ignore();
            cls_config_ss >> confidence;
            cls_config_ss.ignore();
            cls_config_ss >> port;
            if(port <0 || port>9){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] the number of ports exceeds the range of 0~9!");
                return;
            }
            postprocess_config_->port_list[port] = true;
            postprocess_config_->cls_port_dict[cls].push_back(std::make_pair(port, confidence / 100)); //attribute->port and confidence_threshold
            HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] test output settings: port: %d, conf: %f, cls: %d", port, confidence / 100, cls);
        }
    }
}


/**
* @brief: write the detection result as json format file
* @[in]: img_infor: image information
* @[in]: attribute_number: predicted classes of the boxes in one image
* @[in]: confidence: predicted confidence of the boxes in one image
* @[in]: bbox: coordinates of all boxes in one image
*/
void SSDPostProcess_1::WriteResultFile(ImageInfor img_infor, const std::vector<float>& attribute_number, const std::vector<float>& confidence, const std::vector<hiai::Rectangle<Point2D>>& bbox) {
    std::string img_infor_tfilename = store_path + "/" + img_infor.tfilename;
    int fd = open(img_infor_tfilename.c_str(), O_CREAT| O_WRONLY, S_IRUSR|S_IWUSR);
    int ret = 0;
    if(fd == -1){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] failed to open result file!");
        return;
    }
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] result file open successfully: %s", img_infor.tfilename.c_str());
    std::string img_infor0 = "";
    img_infor0 = img_infor0 + "{\n";
    img_infor0 = img_infor0 + "\"format\":" + to_string(img_infor.format) + ",\n";
    img_infor0 = img_infor0 +  "\"width\":" + to_string(img_infor.width) + ",\n" ;
    img_infor0 = img_infor0 +  "\"height\":" + to_string(img_infor.height) + ",\n";
    img_infor0 = img_infor0 +  "\"attribute_number\": [" + "\n";

    ret = write(fd, img_infor0.c_str(), img_infor0.length());
    if(ret == -1){
       HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
       ret = close(fd);
       if(ret == -1){
           HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
        }
        return;
    }
    for (unsigned int idx = 0; idx != attribute_number.size(); ++idx){
        if (std::isnan(attribute_number[idx])) {
            ret = write(fd, "-1", 1);
            if(ret == -1){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
                ret = close(fd);
                if(ret == -1){
                    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
                }
                return;
            }
        }
        else{
            std::string value = std::to_string(int(attribute_number[idx]));
            ret = write(fd, value.c_str(), value.length());
            if(ret ==-1){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
                ret = close(fd);
                if(ret == -1){
                    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
                }
                return;
            }
        }
        if (idx + 1 != attribute_number.size()) {
            ret = write(fd, ",", 1);
            if(ret ==-1){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
                ret = close(fd);
                if(ret == -1){
                    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
                }
                return;
            }
        }
    }
    std::string img_infor1 = "";
    img_infor1 = img_infor1 + "\n],\n" + "\"confidence\": [" + "\n";
    ret = write(fd, img_infor1.c_str(), img_infor1.length());
    if(ret == -1){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
        ret = close(fd);
        if(ret == -1){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
            }
            return;
    }
    for (unsigned int idx = 0; idx != confidence.size(); ++idx){
        if (std::isnan(confidence[idx])){
            ret = write(fd, "-1", 1);
            if(ret ==-1){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
                ret = close(fd);
                if(ret == -1){
                    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
                }
                return;
            }
        }
        else {
            std::string value1 = std::to_string(confidence[idx]);
            ret = write(fd, value1.c_str(), value1.length());
            if(ret ==-1){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
                ret = close(fd);
                if(ret == -1){
                    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
                }
                return;
            }
        }
        if (idx + 1 != confidence.size()) {
            ret = write(fd, ",", 1);
            if(ret ==-1){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
                ret = close(fd);
                if(ret == -1){
                    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
                }
                return;
            }
        }
    }
     std::string img_infor2 = "";
     img_infor2 = img_infor2 + "\n],\n" + "\"bbox\": [" + "\n";
     ret = write(fd, img_infor2.c_str(), img_infor2.length());
     if(ret == -1){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
        ret = close(fd);
        if(ret == -1){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
        }
        return;
     }
     for (unsigned int idx = 0; idx != bbox.size(); ++idx){
        std::string img_infor3 = "";
        img_infor3 = img_infor3 + "[\n" + to_string(bbox[idx].anchor_lt.x) + ",\n";
        img_infor3 = img_infor3 + to_string(bbox[idx].anchor_lt.y) + ",\n";
        img_infor3 = img_infor3 + to_string(bbox[idx].anchor_rb.x) + ",\n";
        img_infor3 = img_infor3 + to_string(bbox[idx].anchor_rb.y) + "\n" + "]";
        ret = write(fd, img_infor3.c_str(), img_infor3.length());
        if(ret == -1){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
            ret = close(fd);
            if(ret == -1){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
            }
            return;
        }
        if(idx + 1 != bbox.size()){
            ret = write(fd, ",", 1);
            if(ret ==-1){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
                ret = close(fd);
                if(ret == -1){
                    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
                }
                return;
            }
        }
        ret = write(fd, "\n", 1);
        if(ret ==-1){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
            ret = close(fd);
            if(ret == -1){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
            }
            return;
        }
    }
    std::string img_infor4 = "";
    img_infor4 = img_infor4 + "]\n" + "}\n";
    ret = write(fd, img_infor4.c_str(), img_infor4.length());
    if(ret == -1){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] write file error!");
        ret = close(fd);
        if(ret == -1){
           HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
        }
        return;
    }
    ret = close(fd);
    if(ret == -1){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] close file error!");
    }
}

/**
* @brief: init the output structs according to port settings
* @in: tran: output struct received from previous engine
*/
void SSDPostProcess_1::InitOutputs(std::shared_ptr<EngineTransT> tran) {
    outputs.clear();
    for (unsigned int j = 0; j < postprocess_config_->port_list.size(); j++) {
        if (postprocess_config_->port_list[j] == true) {
            outputs[j] = std::make_shared<hiai::BatchDetectedObjectPara<hiai::Rectangle<Point2D>, float>>();
            outputs[j]->b_info = tran->b_info;
        }
        else if (outputs.count(-1) == 0) {
            outputs[-1] = std::make_shared<hiai::BatchDetectedObjectPara<hiai::Rectangle<Point2D>, float>>();
            outputs[-1]->b_info = tran->b_info;
        }

    }
}

/**
* @brief: filt out the detection output according to the tensor name
* @[in]: out_num: struct of the number of detected bboxes
* @[in]: out_bbox: struct of the detected content, including bbox coordinates, predicted classes and confidence
* @[in]: output_data_vec: output of inference engine for one batch
*/
void SSDPostProcess_1::GetRealInputs(OutputT& out_num, OutputT& out_bbox, const std::vector<OutputT>& output_data_vec) {
    for (unsigned int i = 0; i<output_data_vec.size(); i++) {
        OutputT out = output_data_vec[i];
        std::string index;
        std::string cur_name;
        GetLayerName(out.name, index, cur_name);
        std::string match_name = postprocess_config_->output_node;
        if (match_name == cur_name) {
            if (index == "0") {
                out_num = output_data_vec[i];
            }
            else if (index == "2") {
                out_bbox = output_data_vec[i];
            }
        }
    }
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1]: Real input tensor name: %s", out_bbox.name.c_str());
}

/**
* init
*/
HIAI_StatusT SSDPostProcess_1::Init(const hiai::AIConfig& config, const  std::vector<hiai::AIModelDescription>& model_desc)
{
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] start init!");
    if (nullptr == postprocess_config_)
    {
        postprocess_config_ = std::make_shared<PostprocessConfig>();
    }
    for (int index = 0; index < config.items_size(); ++index)
    {
        const ::hiai::AIConfigItem& item = config.items(index);
        std::string name = item.name();
        if (name == "path") {
            postprocess_config_->path = item.value();
        } else if (name == "output_settings") {
            if (item.value() != "") {
                SetOutputFilter(item.value());
            }
        } else if (name == "output_name") {
            postprocess_config_->output_node = item.value();
        } else if (name == "mode"){
            postprocess_config_->mode = item.value();
        }
    }

    if(postprocess_config_->mode != "publish"){
        std::string info_file_ = GetInfoFilePath(postprocess_config_->path);
        id_img_correlation.clear();
        id_img_correlation = SetImgPredictionCorrelation(info_file_, ".json");
        if (id_img_correlation.empty()) {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] scan info file failed!");
            return HIAI_ERROR;
        }
    }

    uint32_t graph_id = Engine::GetGraphId();
    std::shared_ptr<Graph> graph = Graph::GetInstance(graph_id);
    if (nullptr == graph)
    {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "Fail to get the graph");
        return HIAI_ERROR;
    }
    std::ostringstream deviceId;
    deviceId << graph->GetDeviceID();
    string device_dir = RESULT_FOLDER + "/" + deviceId.str();
    store_path = device_dir + "/" + ENGINE_NAME;
    if (HIAI_OK != CreateFolder(RESULT_FOLDER, PERMISSION)) {
        return HIAI_ERROR;
    }
    if (HIAI_OK != CreateFolder(device_dir, PERMISSION)) {
        return HIAI_ERROR;
    }
    if (HIAI_OK != CreateFolder(store_path, PERMISSION)) {
        return HIAI_ERROR;
    }
    batchDetected = std::make_shared<hiai::BatchDetectedObjectPara<hiai::Rectangle<Point2D>, float>>();
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] end init!");
    return HIAI_OK;
}

/**
* @brief: resolve the detection results from buffer and set the engine outputs struct
* @[in]: ptr: pointer in the buffer, indicates start of one bbox in the buffer
* @[in]: attribute_number: predicted classes of the boxes in one image
* @[in]: confidence: predicted confidence of the boxes in one image
* @[in]: bbox: coordinates of all boxes in one image
* @[in]: crop: transformation information
*/
void SSDPostProcess_1::ResolveBBox(float* ptr, std::vector<float>& attribute_number, std::vector<float>& confidence, std::vector<hiai::Rectangle<Point2D>>& bbox, const CropInfo& crop) {
    if(ptr == nullptr){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] ptr is nullptr");
        return;
    }
    ptr[3] = ptr[3]<0.0 ? 0.0 : (ptr[3]>1.0 ? 1.0 : ptr[3]);
    ptr[4] = ptr[4]<0.0 ? 0.0 : (ptr[4]>1.0 ? 1.0 : ptr[4]);
    ptr[5] = ptr[5]<0.0 ? 0.0 : (ptr[5]>1.0 ? 1.0 : ptr[5]);
    ptr[6] = ptr[6]<0.0 ? 0.0 : (ptr[6]>1.0 ? 1.0 : ptr[6]);
    float attr = ptr[1];
    float score = ptr[2];
    hiai::Rectangle<Point2D> rect;
    if ((int)attr == 0)
    {
        return;
    }
    //write the result file
    attr -= 1;
    rect.anchor_lt.x = ((int)(ptr[3] * crop.crop_width + crop.point_x)) / 2 * 2;
    rect.anchor_lt.y = ((int)(ptr[4] * crop.crop_height + crop.point_y)) / 2 * 2;
    rect.anchor_rb.x = ((int)(ptr[5] * crop.crop_width + crop.point_x)) / 2 * 2;
    rect.anchor_rb.y = ((int)(ptr[6] * crop.crop_height + crop.point_y)) / 2 * 2;
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1]: test for coordinates: %d, %d, %d, %d, %f", rect.anchor_lt.x, rect.anchor_lt.y, rect.anchor_rb.x, rect.anchor_rb.y, ptr[3]);

    attribute_number.emplace_back(attr);
    confidence.emplace_back(score);
    bbox.emplace_back(rect);

    if (OUTPUT_SIZE > 1) {
        //no filter
        if (outputs.count(-1) != 0) {
            object_locs[-1].v_obj_id.push_back(attr);
            object_locs[-1].range.push_back(rect);
            object_locs[-1].confidence.push_back(score);
        }
        if (postprocess_config_->cls_port_dict.count(attr) != 0) {
            for (auto settings : postprocess_config_->cls_port_dict[int(attr)]) {
                if (settings.second <= score) {
                    object_locs[settings.first].v_obj_id.push_back(attr);
                    object_locs[settings.first].range.push_back(rect);
                    object_locs[settings.first].confidence.push_back(score);
                }
            }
        }
    }
}

/**
* @brief: resolve the detection results for caffe engine
* @[in]: ptr: pointer in the buffer, indicates start of one bbox in the buffer
* @[in]: img_infor: image information
* @[in]: attribute_number: predicted classes of the boxes in one image
* @[in]: confidence: predicted confidence of the boxes in one image
* @[in]: bbox: coordinates of all boxes in one image
* @[in]: object_loc: the output struct
*/

void SSDPostProcess_1::CaffeResolveBBox(float* ptr, int ptrSize, const ImageInfor img_infor, std::vector<float>& attribute_number, std::vector<float>& confidence,
        std::vector<hiai::Rectangle<Point2D>>& bbox, ObjectLocation<Rectangle<Point2D>, float>& object_loc)
{
    if (nullptr == ptr || ptrSize != g_sizePerSSD) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] CaffeResolveBBox param error");
        return;
    }

    float attr = ptr[1];
    float score = ptr[2];
    hiai::Rectangle<Point2D> rect = {{0, 0}};

    if ((int)attr == 0) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] CaffeResolveBBox attr param error");
        return;
    }

    attr -= 1;
    rect.anchor_lt.x = ptr[3] * img_infor.width;
    rect.anchor_lt.y = ptr[4] * img_infor.height;
    rect.anchor_rb.x = ptr[5] * img_infor.width;
    rect.anchor_rb.y = ptr[6] * img_infor.height;

    attribute_number.emplace_back(attr);
    confidence.emplace_back(score);
    bbox.emplace_back(rect);

    object_loc.v_obj_id.push_back(attr);
    object_loc.range.push_back(rect);
    object_loc.confidence.push_back(score);
}

/**
* @brief: build the outputs struct of all images in one batch
*/
void SSDPostProcess_1::BuildBatch() {
    if (outputs.count(-1) != 0) {
        outputs[-1]->v_location.push_back(object_locs[-1]);
    }
    for (unsigned int i = 0; i < postprocess_config_->port_list.size(); i++) {
        if (postprocess_config_->port_list[i] == true) {
            HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] filter! port: %d, number of boxes: %d", i, object_locs[i].range.size());
            outputs[i]->v_location.push_back(object_locs[i]);
        }
    }
}
/**
* @brief: send the outputs struct to corresponding ports
*/
void SSDPostProcess_1::SendDataToPorts() {
    for (unsigned int i = 0; i < postprocess_config_->port_list.size(); i++) {
        if (postprocess_config_->port_list[i] == false) {
            HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] no filter! port: %d", i);
            HIAI_StatusT ret = SendData(i, "BatchDetectedObjectPara_Rectangle_Point2D_float", std::static_pointer_cast<void>(outputs[-1]));
            CheckStatus(ret, "[SSDPostProcess_1] failed to send");
        }
        else {
            HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] filter! port: %d", i);
            HIAI_StatusT send_ret = SendData(i, "BatchDetectedObjectPara_Rectangle_Point2D_float", std::static_pointer_cast<void>(outputs[i]));
            CheckStatus(send_ret, "[SSDPostProcess_1] failed to send");
        }
    }
}

/**
* @brief: handle the exceptions, i.e. send empty batchDetected struct to all the ports
* @in: output_size: the number of ports plus one (there's one port reserved for recall functor)
*/
void SSDPostProcess_1::HandleExceptions(int output_size) {
    if (output_size != 1) {
        for (int j = 0; j < output_size - 1; j++) {
            HIAI_StatusT ret = SendData(j, "BatchDetectedObjectPara_Rectangle_Point2D_float", std::static_pointer_cast<void>(batchDetected));
            CheckStatus(ret, "[SSDPostProcess_1] failed to send data");
        }
    }
};

/**
* @brief: check status and print error log
* @in: ret_status: hiaiengine defined return status code
* @in: error_msg: the error log string
*/
void SSDPostProcess_1::CheckStatus(HIAI_StatusT ret_status, std::string error_msg) {
    if (HIAI_OK != ret_status) {
        HIAI_ENGINE_LOG(error_msg.c_str());
    }
};

/**
* @brief: send sentinel image to inform the graph to destroy
*/
HIAI_StatusT SSDPostProcess_1::SendSentinelImage() {
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] sentinel image, process over.");
    HIAI_StatusT hiai_ret = HIAI_OK;
    std::map<int, bool> port_map;
    if (OUTPUT_SIZE == 1) {
        port_map[0] = false;
    }
    else {
        for (int i = 0; i < OUTPUT_SIZE - 1; i++) {
            port_map[i] = false;
        }
    }
    do {
        for (std::map<int, bool>::iterator iter = port_map.begin(); iter != port_map.end() && !iter->second; iter++) {
            hiai_ret = SendData(iter->first, "BatchDetectedObjectPara_Rectangle_Point2D_float", batchDetected);
            if (HIAI_OK != hiai_ret)
            {
                if (HIAI_ENGINE_NULL_POINTER == hiai_ret || HIAI_HDC_SEND_MSG_ERROR == hiai_ret || HIAI_HDC_SEND_ERROR == hiai_ret
                    || HIAI_GRAPH_SRC_PORT_NOT_EXIST == hiai_ret || HIAI_GRAPH_ENGINE_NOT_EXIST == hiai_ret) {
                    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] SendData error[%d], break.", hiai_ret);
                    break;
                }
                HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[SSDPostProcess_1] port[%d] send fail, sleep 200ms, hiai_ret:%d.", iter->first, hiai_ret);
                usleep(SEND_DATA_INTERVAL_MS);
                continue;
            }
            iter->second = true;
        }
    } while (HIAI_OK != hiai_ret);
    return hiai_ret;
}

/**
* @brief: post process for caffe engine
* @in: output_data_vec: output tensor of the previous engine
* @in: tran: output struct received from previous engine
*/
HIAI_StatusT SSDPostProcess_1::CaffeSSDProcess(std::vector<OutputT> &output_data_vec, std::shared_ptr<EngineTransT> tran)
{
    if (nullptr == tran || nullptr == tran.get() || tran->b_info.batch_size == 0 || output_data_vec.size() <= 0) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[CaffeSSDProcess]: param error!");
        return HIAI_ERROR;
    }

    OutputT out = { 0, "", nullptr };
    for (unsigned int i = 0; i < output_data_vec.size(); i++) {
        out = output_data_vec[i];
        std::string index = "";
        std::string cur_name = "";
        GetLayerName(out.name, index, cur_name);
        std::string match_name = postprocess_config_->output_node;
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[CaffeSSDProcess]: index:%d, match_name(%s) VS cur_name(%s)", i, match_name.c_str(), cur_name.c_str());
    }
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[CaffeSSDProcess]: %s", out.name.c_str());

    uint32_t size = out.size / sizeof(float);
    if (0 >= size) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[CaffeSSDProcess] size error.");
        return HIAI_ERROR;
    }
    float result_all[size];
    int ret = memset_s(result_all, size, 0.0, size);
    if (0 != ret) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[CaffeSSDProcess] memset_s error.");
        return HIAI_ERROR;
    }
    if (nullptr == out.data || nullptr == out.data.get()) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[CaffeSSDProcess]out.data is null");
        return HIAI_ERROR;
    }
    ret = memcpy_s(result_all, sizeof(float) * size, out.data.get(), sizeof(float) * size);
    if (ret != 0) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[CaffeSSDProcess]: memcpy_s out data error!");
        return HIAI_ERROR;
    }

    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[CaffeSSDProcess]: size:%d, batch_size:%d", size, tran->b_info.batch_size);
    batchDetected->b_info = tran->b_info;
    std::vector<uint32_t> frame_ID = tran->b_info.frame_ID;

    // 1.0 firstly, get detection numbers in every pic which stored in numPerPic
    unsigned int numPerPic[tran->b_info.batch_size]; //the detection numbers of every pic in one batch
    ret = memset_s(numPerPic, tran->b_info.batch_size * sizeof(unsigned int), 0, tran->b_info.batch_size * sizeof(unsigned int));
    if (0 != ret) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[CaffeSSDProcess] numPerPic memset_s error.");
        return HIAI_ERROR;
    }
    unsigned int offsetSize = 0; //the first float of every detection result(7 floats) means batch id, get detection numbers by top_data[0] of evey 7 floats
    for (unsigned int ind = 0; ind < tran->b_info.batch_size; ind++) {
        while ((unsigned int)(result_all[offsetSize]) == ind) {
            offsetSize += g_sizePerSSD;
            numPerPic[ind]++;
            if (offsetSize >= size) {
                break;
            }
        }
    }

    // 2.0 analyse the detection result of each image in the batch
    for (unsigned int ind = 0; ind < tran->b_info.batch_size; ind++) {
        ImageInfor img_infor;
        if(postprocess_config_->mode == "publish"){
            img_infor.width = tran->v_img[ind].img.width;
            img_infor.height = tran->v_img[ind].img.height;
            img_infor.format = tran->v_img[ind].img.format;
            img_infor.tfilename = "davinci_" + std::to_string(frame_ID[ind]) + ".json";
        } else{
            img_infor = id_img_correlation[tran->b_info.frame_ID[ind]];
        }

        unsigned int offset = 0; //the offset of evey pic's detection result from result_all which stored all detection results
        for (unsigned int i = 0; i < ind; i++) {
            offset += numPerPic[i];
        }
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[CaffeSSDProcess]: numPerPic[%d] offset %d", ind, offset);

        float *result = result_all + offset * g_sizePerSSD; //get data start ptr addr of detection result of every pic
        ObjectLocation<Rectangle<Point2D>, float> object_loc;

        std::vector<float> attribute_number;
        std::vector<float> confidence;
        std::vector<hiai::Rectangle<Point2D>> bbox;
        float *ptr = result;
        for (unsigned int k = 0; k < numPerPic[ind] * g_sizePerSSD; k += g_sizePerSSD) {
            ptr = result + k;
            CaffeResolveBBox(ptr, g_sizePerSSD, img_infor, attribute_number, confidence, bbox, object_loc);
        }
        batchDetected->v_location.push_back(object_loc);

        // write output file
        WriteResultFile(img_infor, attribute_number, confidence, bbox);
    }
    return HIAI_OK;
}

/**
* @brief: reset the cropinfo to make sure the output bbox coordinates are always relative to the original image
* @in: crop: the crop info got from preprocess, which will be reseted for later transformation process
* @in: img_infor: the image information of the current image
*/
void SSDPostProcess_1::SetTransformationInfor(CropInfo& crop, const ImageInfor img_infor) {
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1]: test for crop info before: %d, %d, %d, %d", crop.point_x, crop.point_x, crop.crop_width, crop.crop_height);
    if (crop.point_x == -1 || crop.point_y == -1 || crop.crop_width == -1 || crop.crop_height == -1) {
        crop.point_x = crop.point_y = 0;
        crop.crop_width = img_infor.width;
        crop.crop_height = img_infor.height;
    }
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1]: test for crop info after: %d, %d, %d, %d", crop.point_x, crop.point_x, crop.crop_width, crop.crop_height);
}

HIAI_IMPL_ENGINE_PROCESS("SSDPostProcess_1", SSDPostProcess_1, INPUT_SIZE)
{
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] start process!");
    if (nullptr == arg0)
    {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] fail to process invalid message");
        HandleExceptions(OUTPUT_SIZE);
        return HIAI_ERROR;
    }
    std::shared_ptr<EngineTransT> tran = std::static_pointer_cast<EngineTransT>(arg0);
    std::vector<OutputT> output_data_vec = tran->output_data_vec;

    //add sentinel image for showing this data in dataset are all sended, this is last step.
    BatchImageParaWithScaleT image_handle = { tran->b_info, tran->v_img };
    if (isSentinelImage(std::make_shared<BatchImageParaWithScaleT>(image_handle)))
    {
        return SendSentinelImage();
    }

    if (!tran->status)
    {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, tran->msg.c_str());
        HandleExceptions(OUTPUT_SIZE);
        return HIAI_ERROR;
    }

    InitOutputs(tran);
    const int sizePerClassify = 7; //mind inference uses 7 float to show classification info

    if (std::string::npos != (tran->msg).find("CaffeInferenceEngine"))
    {
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] caffe ssd post process");
        return CaffeSSDProcess(output_data_vec, tran);
    }
    OutputT out_num;
    OutputT out_bbox;
    GetRealInputs(out_num, out_bbox, output_data_vec);

    int batch_buffer_size = out_bbox.size/sizeof(float);
    float batch_result[batch_buffer_size];
    int ret = memcpy_s(batch_result, sizeof(float)*batch_buffer_size, out_bbox.data.get(), sizeof(float)*batch_buffer_size);
    if(ret != 0){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1]: memcpy_s out data error!");
        return HIAI_ERROR;
    }
    int buffer_size = batch_buffer_size / tran->b_info.max_batch_size;
    int number_offset = out_num.size / tran->b_info.max_batch_size;
    std::vector<uint32_t> frame_ID = tran->b_info.frame_ID;
    //the structs for storaging results
    std::vector<float> attribute_number;
    std::vector<float> confidence;
    std::vector<hiai::Rectangle<Point2D>> bbox;
    //process each image in the batch
    for (unsigned int ind = 0; ind < tran->b_info.batch_size; ind++)
    {
        attribute_number.clear();
        confidence.clear();
        bbox.clear();
        object_locs.clear();
        if ((int)frame_ID[ind] == -1) {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1] image number %d failed, add empty struct", ind);
            BuildBatch();
            continue;
        }

        ImageInfor img_infor;
        if(postprocess_config_->mode == "publish"){
            img_infor.width = tran->v_img[ind].img.width;
            img_infor.height = tran->v_img[ind].img.height;
            img_infor.format = tran->v_img[ind].img.format;
            img_infor.tfilename = "davinci_" + std::to_string(frame_ID[ind]) + ".json";
        } else{
            img_infor = id_img_correlation[frame_ID[ind]];
        }

        //get crop information
        auto crop = tran->v_img[ind].crop_info;
        SetTransformationInfor(crop, img_infor);

        float bbox_count[1];
        if (memcpy_s(bbox_count, sizeof(float), out_num.data.get() + number_offset*ind, sizeof(float))) {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[SSDPostProcess_1]: memcpy_s bbox_count error!");
            BuildBatch();
            continue;
        }
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1]: number of bbox: %f", *bbox_count);
        float *result = batch_result + buffer_size*ind;//the start addr of the result buffer of each image
        float *ptr = result; //the start addr of the result buffer of each bbox

        //each bbox  in one image
        int bbox_buffer_size = (*bbox_count)*sizePerClassify;
        for (int k = 0; k < bbox_buffer_size; k += sizePerClassify)
        {
            ptr = result + k;
            ResolveBBox(ptr, attribute_number, confidence, bbox, crop);
        }
        //add the current result into batch result
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] frameid: %d, name: %s", frame_ID[ind], img_infor.tfilename.c_str());
        BuildBatch();
        // write output file
        WriteResultFile(img_infor, attribute_number, confidence, bbox);
    }
    if (OUTPUT_SIZE > 1) {
        SendDataToPorts();
    }
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SSDPostProcess_1] end process!");
    return HIAI_OK;
}

/**
*
* Copyright(c)<2018>, <Huawei Technologies Co.,Ltd>
*
* @version 1.0
*
* @date 2018-5-19
*/
#include "Mind_BicycleImages.h"
#include "hiaiengine/log.h"
#include "hiaiengine/data_type_reg.h"
#include "hiaiengine/ai_memory.h"
#include <memory>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <map>

const static std::string SIMULATOR_LOCAL = "Simulator_local";
const static std::string OI = "OI";
const int DVPP_BUFFER_ALIGN_SIZE = 128;

/**
* @brief: check whether the string is number
* @[in]: value
* @[return]: bool, if value is number, return true, else return false
*/
bool Mind_BicycleImages::CheckNumber(const std::string& value){
    //if value is "", this is not number
    if(value == ""){
        return false;
    }
    bool isDigit = true;
    for(unsigned int i = 0; i < value.size(); ++i){
        if(!isdigit(value[i])){
            isDigit = false;
            break;
        }
    }
    return isDigit;
}

/**
* @brief: get the select images
* @[in]: value, the select images string, format is '1,2,3,'
*/
void Mind_BicycleImages::GetSelectImages(const std::string& value){
    //if value is "", no select images
    if(value == ""){
        HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[Mind_BicycleImages] select images info is null!");
        return;
    }
    std::string token;
    std::istringstream tokenStream(value);
    while (std::getline(tokenStream, token, ','))
    {
        if(CheckNumber(token)){
            uint32_t frameId = atoi(token.data());
            if(frameId >= 0 && frameId < dataset_info_.size()){
                select_images_.push_back(frameId);
            }else{
                HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[Mind_BicycleImages] select image frame id must be in [0, %u), but the input is %u !", dataset_info_.size(), frameId);
            }
        }else{
            HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[Mind_BicycleImages] select image info must be positive integer, but the input is '%s' !", token.c_str());
        }
    }
    if(select_images_.size() > 0){
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] select %u images success!", select_images_.size());
    }
}

/**
* @brief: get the random images
*/
void Mind_BicycleImages::GetRandomImages(){
    srand((unsigned)time(NULL));
    int totalCount = dataset_info_.size();
    int frameIds[totalCount];
    for(int i = 0; i < totalCount; ++i){
        frameIds[i] = i;
    }
    for(int i = 0; i < data_config_->randomNumber; ++i){
        int index = (rand() % (totalCount - i)) + i;
        int temp = frameIds[index];
        frameIds[index] = frameIds[i];
        frameIds[i] = temp;
        select_images_.push_back(temp);
    }
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] select random %u images success!", data_config_->randomNumber);
}

/**
* @brief: read data.info file, then convert to dataset_info_
* @[return]: HIAI_StatusT
*/
HIAI_StatusT Mind_BicycleImages::MakeDatasetInfo(){
    //get the data info file path
    std::string datainfo_path = data_config_->path;
    while(datainfo_path.back() == '/' || datainfo_path.back() == '\\'){
        datainfo_path.pop_back();
    }
    std::size_t datasetNameIndex = datainfo_path.find_last_of("/\\");
    std::string dataInfoPath = data_config_->path + "." + datainfo_path.substr(datasetNameIndex + 1) + "_data.info";

    //open file
    ifstream fin(dataInfoPath.c_str());
    if (!fin.is_open())
    {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "Open %s failed!", dataInfoPath.c_str());
        return HIAI_ERROR;
    }

    std::string line_content;

    //read first line
    std::getline(fin, line_content);
    std::stringstream lineStr1(line_content);
    std::string datasetName;
    int totalFileNum = 0;
    lineStr1 >> datasetName >> totalFileNum;
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "%s total count is %u.", datasetName.c_str(), totalFileNum);

    int format = -2;
    int fileNumByFormat = 0;
    dataset_info_.clear();
    int count = 0;
    while(count < totalFileNum){
        //read format and image count
        std::getline(fin, line_content);
        std::stringstream lineStrFormat(line_content);
        lineStrFormat >> format >> fileNumByFormat;
        for (int i = 0; i < fileNumByFormat; i++) {
            //read each image info
            std::getline(fin, line_content);
            ImageInfo imageInfo;
            imageInfo.format = format;
            std::stringstream lineStr(line_content);
            lineStr >> imageInfo.id >> imageInfo.path >> imageInfo.width >> imageInfo.height >> imageInfo.size;
            dataset_info_.push_back(imageInfo);
        }
        count += fileNumByFormat;
    }
    //close file
    fin.close();
    return HIAI_OK;
}

/**
* @brief: get the select images or random images
* @[in]: selectImages, the select images string, format is '1,2,3,'
* @[in]: randomNumber, the random number
*/
void Mind_BicycleImages::GetSelectOrRandomImages(const std::string& selectImages, const std::string& randomNumber){
    //clear vector
    select_images_.clear();

    if(data_config_->runMode == "random"){
        if(CheckNumber(randomNumber)){
            uint32_t randomNum = atoi(randomNumber.data());
            //random number must be in [1, images size)
            if(randomNum > 0 && randomNum < dataset_info_.size()){
                data_config_->randomNumber = randomNum;
                GetRandomImages();
            }else{
                HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[Mind_BicycleImages] random number must be in (0, %u), but the input is %u !", dataset_info_.size(), randomNum);
            }
        }else{
            HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[Mind_BicycleImages] random number must be positive integer, but the input is '%s' !", randomNumber.c_str());
        }
    }else if(data_config_->runMode == "specify"){
        //get the specify select images
        GetSelectImages(selectImages);
    }
    //if select_images_ is empty, use all images
    if(select_images_.size() == 0){
        HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[Mind_BicycleImages] There is no random or select images, run all images!");
        data_config_->runMode = "all";
    }
}

/**
* @brief: get batch size
* @[in]: value, the batch size
*/
void Mind_BicycleImages::GetBatchSize(const std::string& value){
    bool check = false;
    if(CheckNumber(value)){
        int batchSize = atoi(value.data());
        if(batchSize > 0){
            data_config_->batchSize = batchSize;
            check = true;
        }
    }
    if(!check){
        HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[Mind_BicycleImages] batch size must be positive integer, but the input is '%s'. Use default batch size %u !", value.c_str(), data_config_->batchSize);
    }
}

HIAI_StatusT Mind_BicycleImages::Init(const hiai::AIConfig& config, const  std::vector<hiai::AIModelDescription>& model_desc)
{
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] start init!");

    data_config_ = std::make_shared<DataConfig>();
    if(data_config_ == NULL){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] make shared for DataConfig error!");
        return HIAI_ERROR;
    }
    data_config_->batchSize = DEFAULT_BATCH_SIZE;
    data_config_->randomNumber = DEFAULT_RANDOM_NUMBER;
    data_config_->runMode = "all";
    std::string selectImages("");
    std::string randomNumber("");

    //read the config of dataset
    for (int index = 0; index < config.items_size(); ++index)
    {
        const ::hiai::AIConfigItem& item = config.items(index);
        std::string name = item.name();
        if(name == "batch"){
            GetBatchSize(item.value());
        }else if(name == "path"){
            data_config_->path = item.value();
        }else if(name == "useAll"){
            data_config_->runMode = item.value();
        }else if(name == "target"){
            data_config_->target = item.value();
        }else if(name == "randomNumber"){
            randomNumber = item.value();
        }else if(name == "selectImages"){
            selectImages = item.value();
        }
    }
    char path[PATH_MAX] = {0};
    if(realpath(data_config_->path.c_str(), path) == NULL){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] can not find path!");
        return HIAI_ERROR;
    }

    //get the dataset image info
    HIAI_StatusT ret = MakeDatasetInfo();

    if(HIAI_OK == ret){
        //if runMode is specify or random , get the select images or random images
        if(data_config_->runMode == "specify" || data_config_->runMode == "random"){
            GetSelectOrRandomImages(selectImages, randomNumber);
        }else{
            data_config_->runMode = "all";
        }
    }
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] end init!");
    return ret;
}

/**
* @brief: get the image buffer
* @[in]: path, the image path;
* @[in]: imageBufferPtr, the point of image buffer;
* @[in]: imageBufferLen, the buffer length;
* @[in]: frameId, the start of file offset
* @[return]: bool, if success return true, else return false
*/
bool Mind_BicycleImages::GetImageBuffer(const char* path, uint8_t* imageBufferPtr, uint32_t imageBufferLen, uint32_t frameId){
    bool ret = false;
    FILE * file = fopen64(path, "r");
    if (NULL == file)
    {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] Error: open file %s failed", path);
        return ret;
    }
    do
    {
        unsigned long imageFseek = ((unsigned  long)frameId)*((unsigned  long)imageBufferLen);
        if (0 != fseeko64(file, imageFseek, SEEK_SET))
        {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] fseeko64 offset = %u failed in GetImageBuffer", frameId * imageBufferLen);
            break;
        }
        if (imageBufferLen != fread(imageBufferPtr, 1, imageBufferLen, file))
        {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] fread length = %u failed in GetImageBuffer", imageBufferLen);
            break;
        }
        ret = true;
    } while (0);

    fclose(file);
    return ret;
}

/**
* @brief: convert image info to EvbImageInfo
* @[in]: index, index of image in dataset_info_
* @[return]: shared_ptr<EvbImageInfo>, if null, means error
*/
shared_ptr<EvbImageInfo> Mind_BicycleImages::makeEvbImageInfo(int index){
    if(index < 0 || (unsigned int)index >= dataset_info_.size()){
        return NULL;
    }
    shared_ptr<EvbImageInfo> evbImageInfo = std::make_shared<EvbImageInfo>();
    if(evbImageInfo == NULL){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] make shared for EvbImageInfo error!");
        return NULL;
    }
    evbImageInfo->format = (IMAGEFORMAT)dataset_info_[index].format;
    evbImageInfo->width = dataset_info_[index].width ;
    evbImageInfo->height = dataset_info_[index].height;
    std::string imageFullPath = data_config_->path + dataset_info_[index].path;
    if((ImageType)dataset_info_[index].format == IMAGE_TYPE_JPEG){
        // transfer jepg to imagepreprocess use dvpp jepgd need to add 8 bit for check
        evbImageInfo->size = dataset_info_[index].size + 8;
    }else{
        evbImageInfo->size = dataset_info_[index].size;
    }
    uint8_t * imageBufferPtr = NULL;
    HIAI_StatusT get_ret = hiai::HIAIMemory::HIAI_DMalloc(evbImageInfo->size, (void*&)imageBufferPtr);
    if(HIAI_OK != get_ret || NULL == imageBufferPtr){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] DMalloc buffer error!");
        return NULL;
    }
    bool ret = GetImageBuffer(imageFullPath.c_str(), imageBufferPtr, dataset_info_[index].size, 0);
    if(!ret){
        get_ret = hiai::HIAIMemory::HIAI_DFree(imageBufferPtr);
        if(HIAI_OK != get_ret){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] DFree buffer error!");
        }
        imageBufferPtr = NULL;
        return NULL;
    }
    evbImageInfo->pucImageData = imageBufferPtr;
    evbImageInfo->frame_ID = dataset_info_[index].id;
    return evbImageInfo;
}

/**
* @brief free the buffer malloced by HIAI:MALLOC
*/
static void FreeImageBuffer(uint8_t* ptr){
    if (ptr == NULL) {
        return;
    }
    HIAI_StatusT ret = HIAI_OK;
    #if defined(IS_OI)
        ret = hiai::HIAIMemory::HIAI_DVPP_DFree(ptr);
    #else
        ret = hiai::HIAIMemory::HIAI_DFree(ptr);
    #endif
    if (HIAI_OK != ret) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] DFree buffer error!");
    }
    ptr = NULL;
}

/**
* @brief: convert image info to NewImageParaT
* @[in]: index, index of image in dataset_info_
* @[out]: imgData, the point of data image
* @[return]: HIAI_StatusT
*/
HIAI_StatusT Mind_BicycleImages::makeImageInfo(NewImageParaT* imgData, int index) {
    if(index < 0 || (uint32_t)index >= dataset_info_.size()){
        return HIAI_ERROR;
    }
    imgData->img.format = (IMAGEFORMAT)dataset_info_[index].format;
    imgData->img.width = dataset_info_[index].width ;
    imgData->img.height = dataset_info_[index].height;
    std::string imageFullPath = data_config_->path + dataset_info_[index].path;

    uint8_t * imageBufferPtr = NULL;
    HIAI_StatusT get_ret = HIAI_OK;
    #if defined(IS_OI)
        //run on same side with dvpp
        if((ImageType)dataset_info_[index].format == IMAGE_TYPE_JPEG){
        // transfer jepg to imagepreprocess use dvpp jepgd need to add 8 bit for check
            imgData->img.size = dataset_info_[index].size + 8;
        }else{
            imgData->img.size = dataset_info_[index].size;
        }
        //run on same side with dvpp need to make the mem align to 128(dvpp need)
        int alignBufferSize = (int)ceil(1.0 * imgData->img.size / DVPP_BUFFER_ALIGN_SIZE) * DVPP_BUFFER_ALIGN_SIZE;
        get_ret = hiai::HIAIMemory::HIAI_DVPP_DMalloc(alignBufferSize, (void*&)imageBufferPtr);
    #else
        imgData->img.size = dataset_info_[index].size;
        get_ret = hiai::HIAIMemory::HIAI_DMalloc(imgData->img.size, (void*&)imageBufferPtr);
    #endif

    if(HIAI_OK != get_ret || NULL == imageBufferPtr){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] DMalloc buffer error!");
        return HIAI_ERROR;
    }
    if(imageBufferPtr == NULL){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] alloc buffer error in makeImageInfo");
        return HIAI_ERROR;
    }

    bool ret = GetImageBuffer(imageFullPath.c_str(), imageBufferPtr, dataset_info_[index].size, 0);

    if(!ret){
        FreeImageBuffer(imageBufferPtr);
        return HIAI_ERROR;
    }
    // free imageBufferPtr with function FreeImageBuffer()
    shared_ptr<uint8_t> data(imageBufferPtr, FreeImageBuffer);
    imgData->img.data = data;
    return HIAI_OK;
}

/**
* @brief: send batch for Emulator and OI
* @[in]: batchId, batchId;
* @[in]: batchNum, the total number of batch;
* @[in]: imageInfoBatch, the send data;
* @[return]: HIAI_StatusT
*/
HIAI_StatusT Mind_BicycleImages::SendBatch(int batchId, int batchNum, std::shared_ptr<BatchImageParaWithScaleT> imageInfoBatch){
    HIAI_StatusT hiai_ret = HIAI_OK;
    imageInfoBatch->b_info.batch_size = imageInfoBatch->v_img.size();
    imageInfoBatch->b_info.max_batch_size = data_config_->batchSize;
    imageInfoBatch->b_info.batch_ID = batchId;
    imageInfoBatch->b_info.is_first = (batchId == 0 ? true : false);
    imageInfoBatch->b_info.is_last = (batchId == batchNum - 1 ? true : false);

    do{
        hiai_ret = SendData(DEFAULT_DATA_PORT, "BatchImageParaWithScaleT", std::static_pointer_cast<void>(imageInfoBatch));
        if(HIAI_QUEUE_FULL == hiai_ret)
        {
            HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] queue full, sleep 200ms");
            usleep(SEND_DATA_INTERVAL_MS);
        }
    }while(hiai_ret == HIAI_QUEUE_FULL);

    if(HIAI_OK != hiai_ret){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] SendData batch %u failed! error code: %u", batchId, hiai_ret);
    }
    return hiai_ret;
}

/**
* @brief: send batch for EVB
* @[in]: batchId, batchId;
* @[in]: batchNum, the total number of batch;
* @[in]: evbImageInfoBatch, the send data;
* @[return]: HIAI_StatusT
*/
HIAI_StatusT Mind_BicycleImages::SendEvbBatch(int batchId, int batchNum, std::vector<std::shared_ptr<EvbImageInfo>>& evbImageInfoBatch){
    HIAI_StatusT hiai_ret = HIAI_OK;
    for(unsigned int i = 0; i < evbImageInfoBatch.size(); ++i){
        evbImageInfoBatch[i]->batch_size = evbImageInfoBatch.size();
        evbImageInfoBatch[i]->batch_index = i;
        evbImageInfoBatch[i]->max_batch_size = data_config_->batchSize;
        evbImageInfoBatch[i]->batch_ID = batchId;
        evbImageInfoBatch[i]->is_first = (batchId == 0 ? true : false);
        evbImageInfoBatch[i]->is_last = (batchId == batchNum - 1 ? true : false);
        do{
            hiai_ret = SendData(DEFAULT_DATA_PORT, "EvbImageInfo", std::static_pointer_cast<void>(evbImageInfoBatch[i]));
            if(HIAI_GRAPH_NO_USEFUL_MEMORY == hiai_ret || HIAI_GRAPH_INVALID_VALUE == hiai_ret)
            {
                HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] hiai graph no useful memory, sleep 200ms");
                usleep(SEND_DATA_INTERVAL_MS);
            }
        }while(HIAI_GRAPH_NO_USEFUL_MEMORY == hiai_ret  || HIAI_GRAPH_INVALID_VALUE == hiai_ret);

        if(HIAI_OK != hiai_ret){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] SendData image frame id %u failed! error code: %u", evbImageInfoBatch[i]->frame_ID, hiai_ret);
            FreeEvbBuffer(evbImageInfoBatch, i);
            return hiai_ret;
        }
    }
    return hiai_ret;
}

/**
* @brief: get the frame id
* @[in]: index, if runMode is all, index is index of image in dataset_info_, else index is index of select_images_
* @[return]: index of image in dataset_info_
*/
int Mind_BicycleImages::GetFrameId(int index){
    int frameId = index;
    if(data_config_->runMode != "all"){
        frameId = select_images_[index];
    }
    return frameId;
}

/**
* free evb buffer
**/
void Mind_BicycleImages::FreeEvbBuffer(const std::vector<std::shared_ptr<EvbImageInfo>>& evbImageInfoBatch, int start){
    HIAI_StatusT ret = HIAI_OK;
    for(unsigned int i = start; i < evbImageInfoBatch.size(); ++i){
        if(evbImageInfoBatch[i]->pucImageData != NULL){
            ret = hiai::HIAIMemory::HIAI_DFree(evbImageInfoBatch[i]->pucImageData);
            if(HIAI_OK != ret){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] free buffer error!");
            }
            evbImageInfoBatch[i]->pucImageData = NULL;
        }       
    }
}

/**
* @brief: get the total count
* @[return]: if runMode is all, return the size of dataset_info_, else return the size of select_images_
*/
int Mind_BicycleImages::GetTotalCount(){
    int totalCount = dataset_info_.size();
    if(data_config_->runMode != "all"){
        totalCount = select_images_.size();
    }
    return totalCount;
}

/**
* @brief: run images on same side, all engine at same side
* @[return]: HIAI_StatusT
*/
HIAI_StatusT Mind_BicycleImages::RunOnSameSide(){
    HIAI_StatusT ret = HIAI_OK;
    int totalCount = GetTotalCount();
    int batchNum = totalCount % data_config_->batchSize == 0 ? totalCount / data_config_->batchSize : totalCount / data_config_->batchSize + 1;
    int i = 0;
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] run on %s, run %u images, batch size is %u", data_config_->target.c_str(), totalCount, data_config_->batchSize);
    for(int batchId = 0; batchId < batchNum && i < totalCount; batchId++){
        //convert batch image infos to BatchImageParaWithScaleT
        std::shared_ptr<BatchImageParaWithScaleT> imageInfoBatch = std::make_shared<BatchImageParaWithScaleT>();
        if(imageInfoBatch == NULL){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] make shared for BatchImageParaWithScaleT error!");
            return HIAI_ERROR;
        }
        for(int j = 0 ; j < data_config_->batchSize && i < totalCount; j++){
            NewImageParaT imgData;
            int frameId = GetFrameId(i);
            ret = makeImageInfo(&imgData, frameId);
            if(HIAI_OK != ret) {
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] Error: make image info frame id %u for batch id %u failed! Stop to send images!",frameId, batchId);
                return ret;
            }
            imageInfoBatch->v_img.push_back(imgData);
            imageInfoBatch->b_info.frame_ID.push_back(dataset_info_[frameId].id);
            i++;
        }
        //then send data
        ret = SendBatch(batchId, batchNum, imageInfoBatch);
        if(HIAI_OK != ret) {
            return ret;
        }
    }
    return  HIAI_OK;
}


/**
* @brief: run images on different side, part of engines on host side, other engines on device side
* @[return]: HIAI_StatusT
*/
HIAI_StatusT Mind_BicycleImages::RunOnDifferentSide(){
    HIAI_StatusT ret = HIAI_OK;
    int totalCount = GetTotalCount();
    int batchNum = totalCount % data_config_->batchSize == 0 ? totalCount / data_config_->batchSize : totalCount / data_config_->batchSize + 1;
    int i = 0;
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] run on %s, run %u images, batch size is %u", data_config_->target.c_str(), totalCount, data_config_->batchSize);
    for(int batchId = 0; batchId < batchNum && i < totalCount; batchId++){
        //convert batch image infos to std::vector<std::shared_ptr<EvbImageInfo>>
        std::vector<std::shared_ptr<EvbImageInfo>> evbImageInfoBatch;
        for(int j = 0 ; j < data_config_->batchSize && i < totalCount; j++){
            int frameId = GetFrameId(i);
            shared_ptr<EvbImageInfo> evbImageInfo = makeEvbImageInfo(frameId);
            if(NULL == evbImageInfo){
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] Error: make image info frame id %u for batch id %u failed! Stop to send images!",frameId, batchId);
                FreeEvbBuffer(evbImageInfoBatch, 0);
                return HIAI_ERROR;
            }
            evbImageInfoBatch.push_back(evbImageInfo);
            i++;
        }
        //then send data
        ret = SendEvbBatch(batchId, batchNum, evbImageInfoBatch);
        if(HIAI_OK != ret){
            return ret;
        }
    }
    return  HIAI_OK;
}

/**
* @brief: check whether run images on same side
* @[return]: HIAI_StatusT
*/
bool Mind_BicycleImages::isOnSameSide(){
    if (data_config_->target == SIMULATOR_LOCAL
        || data_config_->target == OI) {
        return true;
    }
    return false;
}

/**
* @brief: Send Sentinel Image
*/
HIAI_StatusT Mind_BicycleImages::SendSentinelImage()
{
    HIAI_StatusT ret = HIAI_OK;
    //all data send ok, then send a Sentinel info to other engine for end
    if(isOnSameSide()){
        //if all engine on one side, use BatchImageParaWithScaleT to send data
        shared_ptr<BatchImageParaWithScaleT> image_handle = std::make_shared<BatchImageParaWithScaleT>();
        if(image_handle == NULL){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] make shared for BatchImageParaWithScaleT error!");
            return HIAI_ERROR;
        }
        ret = SendBatch(-1, 1, image_handle);
    }else{
        //if part of engines on host side, other engines on device side, use EvbImageInfo to send data
        std::vector<std::shared_ptr<EvbImageInfo>> image_handle;
        shared_ptr<EvbImageInfo> imageInfo = std::make_shared<EvbImageInfo>();     
        if(imageInfo == NULL){
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[Mind_BicycleImages] make shared for EvbImageInfo error!");
            return HIAI_ERROR;
        }
        image_handle.push_back(imageInfo);
        ret = SendEvbBatch(-1, 1, image_handle);
    }
    return ret;
}

/**
* @ingroup hiaiengine
* @brief HIAI_DEFINE_PROCESS : Overloading Engine Process processing logic
* @[in]: Define an input port, an output port
*/
HIAI_IMPL_ENGINE_PROCESS("Mind_BicycleImages", Mind_BicycleImages, INPUT_SIZE)
{
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] start process!");
    std::static_pointer_cast<string>(arg0);
    HIAI_StatusT ret = HIAI_OK;
    if(isOnSameSide()){
        //all engine on one side
        ret = RunOnSameSide();
    }else{
        //part of engines on host side, other engines on device side
        ret = RunOnDifferentSide();
    }
    //send sentinel image
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] send sentinel image!");
    ret = SendSentinelImage();
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[Mind_BicycleImages] end process!");
    return ret;
}

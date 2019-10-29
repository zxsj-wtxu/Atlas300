/**
* @file ImagePreProcess_1.cpp
*
* Copyright(c)<2018>, <Huawei Technologies Co.,Ltd>
*
* @version 1.0
*
* @date 2018-4-25
*/

#include "ImagePreProcess_1.h"
#include <math.h>
#include <sstream>
#include <ctime>
#include "hiaiengine/log.h"
#include "hiaiengine/ai_types.h"
#include "hiaiengine/c_graph.h"
#include "hiaiengine/ai_memory.h"
#include "custom/toolchain/ide_daemon_api.h"

const static int SEND_DATA_SLEEP_MS = 100000;
const static int DVPP_SUPPORT_MAX_WIDTH = 4096;
const static int DVPP_SUPPORT_MIN_WIDTH = 16;
const static int DVPP_SUPPORT_MAX_HEIGHT = 4096;
const static int DVPP_SUPPORT_MIN_HEIGHT = 16;

#define CHECK_ODD(NUM)      (((NUM) % 2 != 0) ? (NUM) : ((NUM) - 1))
#define CHECK_EVEN(NUM)     (((NUM) % 2 == 0) ? (NUM) : ((NUM) - 1))


ImagePreProcess_1::~ImagePreProcess_1()
{
    if (NULL != pidvppapi_) {
        DestroyDvppApi(pidvppapi_);
        pidvppapi_ = NULL;
    }
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1]ImagePreProcess_1 Engine Destory");
}

HIAI_StatusT ImagePreProcess_1::Init(const hiai::AIConfig& config, const std::vector<hiai::AIModelDescription>& modelDesc)
{
    if (NULL == dvppConfig_) {
        dvppConfig_ = std::make_shared<DvppConfig>();
        if (NULL == dvppConfig_ || NULL == dvppConfig_.get()) {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] call dvppConfig_ make_shared failed");
            return HIAI_ERROR;
        }
    }

    //get config from ImagePreProcess_1 Property setting of user.
    std::stringstream ss;
    for (int index = 0; index < config.items_size(); ++index) {
        const ::hiai::AIConfigItem& item = config.items(index);
        std::string name = item.name();
        ss << item.value();
        if ("point_x" == name) {
            ss >> (*dvppConfig_).point_x;
        } else if ("point_y" == name) {
            ss >> (*dvppConfig_).point_y;
        } else if ("crop_width" == name) {
            ss >> (*dvppConfig_).crop_width;
        } else if ("crop_height" == name) {
            ss >> (*dvppConfig_).crop_height;
        } else if ("self_crop" == name) {
            ss >> (*dvppConfig_).self_crop;
        } else if ("dump_value" == name) {
            ss >> (*dvppConfig_).dump_value;
        } else if ("project_name" == name) {
            ss >> (*dvppConfig_).project_name;
        } else if ("resize_height" == name) {
            ss >> (*dvppConfig_).resize_height;
        } else if ("resize_width" == name) {
            ss >> (*dvppConfig_).resize_width;
        } else if ("crop_before_resize" == name) {
            ss >> (*dvppConfig_).crop_before_resize;
        } else if ("yuv420_need" == name) {
            ss >> (*dvppConfig_).yuv420_need;
        } else if ("v_before_u" == name) {
            ss >> (*dvppConfig_).v_before_u;
        } else if ("transform_flag" == name) {
            ss >> (*dvppConfig_).transform_flag;
        } else if ("dvpp_parapath" == name) {
            ss >> (*dvppConfig_).dvpp_para;
        } else if ("userHome" == name) {
            ss >> (*dvppConfig_).userHome;
        } else {
            std::string userDefined = "";
            ss >> userDefined;
            HIAI_ENGINE_LOG(HIAI_IDE_INFO, "userDefined:name[%s], value[%s]", name.c_str(), userDefined.c_str());
        }
        ss.clear();
    }

    // check crop param
    if (NeedCrop()) {
        if (dvppConfig_->point_x > DVPP_SUPPORT_MAX_WIDTH - DVPP_SUPPORT_MIN_WIDTH - 1
            || dvppConfig_->crop_width > DVPP_SUPPORT_MAX_WIDTH
            || dvppConfig_->crop_width < DVPP_SUPPORT_MIN_WIDTH
            || dvppConfig_->point_y > DVPP_SUPPORT_MAX_HEIGHT - DVPP_SUPPORT_MIN_HEIGHT - 1
            || dvppConfig_->crop_height > DVPP_SUPPORT_MAX_HEIGHT
            || dvppConfig_->crop_height < DVPP_SUPPORT_MIN_HEIGHT) {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "crop param error");
            return HIAI_ERROR;
        }
    }

    if (DVPP_SUCCESS != CreateDvppApi(pidvppapi_)) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]Create DVPP pidvppapi_ fail");
        return HIAI_ERROR;
    }

    return HIAI_OK;
}

//jpeg pic process flow:
//  1. DVPP_CTL_JPEGD_PROC
//  2. DVPP_CTL_TOOL_CASE_GET_RESIZE_PARAM
//  3. DVPP_CTL_VPC_PROC
int ImagePreProcess_1::HandleJpeg(const ImageData<u_int8_t> &img)
{
    if (NULL == pidvppapi_) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] pidvppapi_ is null!\n");
        return HIAI_ERROR;
    }

    struct JpegdIn jpegdInData; // input data
    jpegdInData.jpegData = (unsigned char*)(img.data.get()); //the pointer addr of jpeg pic data
    jpegdInData.jpegDataSize = img.size; //the size of jpeg pic data
    jpegdInData.isYUV420Need = false; //(*dvppConfig_).yuv420_need;true:output yuv420 data, otherwize:raw format.
    jpegdInData.isVBeforeU = true; //currently, only support V before U, reserved

    struct JpegdOut jpegdOutData; //output data

    dvppapi_ctl_msg dvppApiCtlMsg; //create inputdata and outputdata for jpegd process
    dvppApiCtlMsg.in = (void *)&jpegdInData;
    dvppApiCtlMsg.in_size = sizeof(struct JpegdIn);
    dvppApiCtlMsg.out = (void *)&jpegdOutData;
    dvppApiCtlMsg.out_size = sizeof(struct JpegdOut);

    if (0 != DvppCtl(pidvppapi_, DVPP_CTL_JPEGD_PROC, &dvppApiCtlMsg)) { //if this single jpeg pic is processed with error, return directly, and then process next pic if there any.
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] JPEGDERROR, FrameID:%u", imageFrameID_);
        jpegdOutData.cbFree(); //release memory from caller.
        return HIAI_ERROR;
    }

    int ret = HandleVpcWithParam(jpegdOutData.yuvData, jpegdOutData.imgWidthAligned, jpegdOutData.imgHeightAligned,
        jpegdOutData.yuvDataSize, img, FILE_TYPE_PIC_JPEG, jpegdOutData.outFormat);
    jpegdOutData.cbFree(); //release memory from caller.
    if (HIAI_OK != ret) { //if vpc process with error, return directly, and then process next pic if there any.
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]VPCERROR, FrameID:%u", imageFrameID_);
        return HIAI_ERROR;
    }
    return HIAI_OK;
}

int ImagePreProcess_1::UpdateCropPara(const Rectangle<Point2D> &rect)
{
    (*dvppConfig_).point_x = (int)rect.anchor_lt.x;
    (*dvppConfig_).point_y = (int)rect.anchor_lt.y;
    (*dvppConfig_).crop_width = (int)(rect.anchor_rb.x - rect.anchor_lt.x);
    (*dvppConfig_).crop_height = (int)(rect.anchor_rb.y - rect.anchor_lt.y);

    HIAI_ENGINE_LOG("[ImagePreProcess_1]: %d, %d, %d, %d", (*dvppConfig_).point_x, (*dvppConfig_).point_y,(*dvppConfig_).crop_width,(*dvppConfig_).crop_height);

    if ((*dvppConfig_).point_x < 0
        || (*dvppConfig_).point_y < 0
        || (*dvppConfig_).crop_height <= 0
        || (*dvppConfig_).crop_width <= 0) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]Some crop parms are invalid!");
        return HIAI_ERROR;
    }
    return HIAI_OK;
}

bool ImagePreProcess_1::SendPreProcessData()
{
    if (NULL == dvppOut_) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]Nothing to send!");
        return false;
    }

    if (dvppOut_->v_img.size() > 0) {
        dvppOut_->b_info.batch_size = dvppOut_->v_img.size();
        HIAI_StatusT hiaiRet = HIAI_OK;
        do {
            hiaiRet = SendData(0, "BatchImageParaWithScaleT", std::static_pointer_cast<void>(dvppOut_));
            if (HIAI_QUEUE_FULL == hiaiRet) {
                HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1] queue full, sleep 200ms");
                usleep(SEND_DATA_INTERVAL_MS);
            }
        } while (hiaiRet == HIAI_QUEUE_FULL);

        if (HIAI_OK != hiaiRet) {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] SendData failed! error code: %d", hiaiRet);
            return false;
        }
    }
    return true;
}

void ImagePreProcess_1::ClearData()
{
    dvppIn_ = NULL;
    dvppOut_ = NULL;
}

//png pic process flow:
//  1. DVPP_CTL_PNGD_PROC
//  2. DVPP_CTL_TOOL_CASE_GET_RESIZE_PARAM
//  3. DVPP_CTL_VPC_PROC
int ImagePreProcess_1::HandlePng(const ImageData<u_int8_t> &img)
{
    if (NULL == pidvppapi_) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] pidvppapi is null");
        return HIAI_ERROR;
    }

    struct PngInputInfoAPI inputPngData; //input data
    inputPngData.inputData = (void *)(img.data.get()); //input data pointer addr
    inputPngData.inputSize = img.size; //input data length
    inputPngData.transformFlag = (*dvppConfig_).transform_flag; //whether to transform

    struct PngOutputInfoAPI outputPngData; //output data

    dvppapi_ctl_msg dvppApiCtlMsg;
    dvppApiCtlMsg.in = (void *)(&inputPngData);
    dvppApiCtlMsg.in_size = sizeof(struct PngInputInfoAPI);
    dvppApiCtlMsg.out = (void *)(&outputPngData);
    dvppApiCtlMsg.out_size = sizeof(struct PngOutputInfoAPI);

    if (0 != DvppCtl(pidvppapi_, DVPP_CTL_PNGD_PROC, &dvppApiCtlMsg)) { //if this single jpeg pic is processed with error, return directly, and process next pic
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]PNGDERROR, FrameID:%u", imageFrameID_);
        if (outputPngData.address != NULL) { //release memory from caller.
            munmap(outputPngData.address, ALIGN_UP(outputPngData.size + VPC_OUT_WIDTH_STRIDE, MAP_2M));
            outputPngData.address = NULL;
        }
        return HIAI_ERROR;
    }

    int ret = HandleVpcWithParam((unsigned char *)outputPngData.outputData, outputPngData.widthAlign, outputPngData.highAlign,
        outputPngData.outputSize, img, FILE_TYPE_PIC_PNG, outputPngData.format);

    if (outputPngData.address != NULL) { //release memory from caller.
        munmap(outputPngData.address, ALIGN_UP(outputPngData.size + VPC_OUT_WIDTH_STRIDE, MAP_2M));
        outputPngData.address = NULL;
    }
    if (HIAI_OK != ret) { //if vpc process with error, return directly, and then process next.
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]VPCERROR, FrameID:%u", imageFrameID_);
        return HIAI_ERROR;
    }
    return HIAI_OK;
}

bool ImagePreProcess_1::NeedCrop()
{
    bool crop = true;
    if (-1 == (*dvppConfig_).point_x
        || -1 == (*dvppConfig_).point_y
        || -1 == (*dvppConfig_).crop_width
        || -1 == (*dvppConfig_).crop_height) {
        crop = false;
    }
    return crop;
}

bool ImagePreProcess_1::ProcessCrop(VpcUserCropConfigure &area, const int &width, const int &height, const int &realWidth, const int &realHeight)
{
    //default no crop
    int leftOffset = 0; //the left side of the cropped image
    int rightOffset = realWidth - 1; //the right side of the cropped image
    int upOffset = 0; //the top side of the cropped image
    uint32_t downOffset = realHeight - 1; //the bottom side of the cropped image
    int fixedWidth = dvppConfig_->crop_width; //the actual crop width
    int fixedHeight = dvppConfig_->crop_height; //the actual crop height

    //user crop
    if (NeedCrop()) {
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1] User crop, System crop");
        //amend input offset to avoid the processed range to exceed real range of image
        if ((dvppConfig_->point_x + dvppConfig_->crop_width) > realWidth) {
            fixedWidth = realWidth - dvppConfig_->point_x;
        }
        if ((dvppConfig_->point_y + dvppConfig_->crop_height) > realHeight) {
            fixedHeight = realHeight - dvppConfig_->point_y;
        }

        leftOffset = dvppConfig_->point_x;
        rightOffset = leftOffset + fixedWidth - 1;
        upOffset = dvppConfig_->point_y;
        downOffset = upOffset + fixedHeight - 1;
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1] leftOffset: %u, rightOffset: %u, upOffset: %u, downOffset: %u",
            leftOffset, rightOffset, upOffset, downOffset);
    }

    // check param validity(range---max:4096*4096, min:16*16), leftOffset and upOffset cannot larger than real width and height
    // -1 is the reason that if point_x, point_y is (0, 0), it represent the 1*1 area.
    if (leftOffset >= realWidth || upOffset >= realHeight
        || rightOffset - leftOffset > DVPP_SUPPORT_MAX_WIDTH - 1 || rightOffset - leftOffset < DVPP_SUPPORT_MIN_WIDTH - 1
        || downOffset - upOffset > DVPP_SUPPORT_MAX_HEIGHT - 1 || downOffset - upOffset < DVPP_SUPPORT_MIN_HEIGHT - 1) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "crop range error");
        return false;
    }

    //restriction: leftOffset and upOffset of inputputArea must be even, rightOffset and downOffset of inputputArea must be odd.
    area.leftOffset = CHECK_EVEN(leftOffset);
    area.rightOffset = CHECK_ODD(rightOffset);
    area.upOffset = CHECK_EVEN(upOffset);
    area.downOffset = CHECK_ODD(downOffset);
    return true;
}

int ImagePreProcess_1::HandleVpc(const ImageData<u_int8_t> &img)
{
    if (NULL == img.data || NULL == img.data.get()) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]img.data is null, ERROR");
        return HIAI_ERROR;
    }

    if (IMAGE_TYPE_NV12 != (ImageTypeT)img.format) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]the format is not yuv, ERROR");
        return HIAI_ERROR;
    }

    unsigned char *alignBuffer = img.data.get();
    int alignWidth = img.width;
    int alignHigh = img.height;
    int alignImageLen = alignWidth * alignHigh * 3 / 2; //the size of yuv data is 1.5 times of width*height
    bool alignMmapFlag = false;

    //if width or height is not align, copy memory
    if (0 != (img.width % VPC_OUT_WIDTH_STRIDE) || 0 != (img.height % VPC_OUT_HIGH_STRIDE)) {
        alignMmapFlag = true;
        alignWidth = ALIGN_UP(img.width, VPC_OUT_WIDTH_STRIDE);
        alignHigh = ALIGN_UP(img.height, VPC_OUT_HIGH_STRIDE);
        alignImageLen = alignWidth * alignHigh * 3 / 2;
        alignBuffer = (unsigned char *)HIAI_DVPP_DMalloc(alignImageLen);
        if (NULL == alignBuffer) {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]HIAI_DVPP_DMalloc alignBuffer is null, ERROR");
            return HIAI_ERROR;
        }
        for (unsigned int i = 0; i < img.height; i++) {
            int ret = memcpy_s(alignBuffer + i * alignWidth, alignWidth, img.data.get() + i * img.width, img.width);
            if (0 != ret) {
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]memcpy_s error in copy Y");
                HIAI_DVPP_DFree(alignBuffer);
                alignBuffer = NULL;
                return HIAI_ERROR;
            }
        }
        for (unsigned int i = 0; i < img.height / 2; i++) {
            int ret = memcpy_s(alignBuffer + i * alignWidth + alignWidth * alignHigh, alignWidth,
                        img.data.get() + i * img.width + img.width * img.height, img.width);
            if (0 != ret) {
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]memcpy_s error in copy UV");
                HIAI_DVPP_DFree(alignBuffer);
                alignBuffer = NULL;
                return HIAI_ERROR;
            }
        }
    }

    int ret = HandleVpcWithParam(alignBuffer, alignWidth, alignHigh, alignImageLen, img, FILE_TYPE_YUV, 3);
    if (HIAI_OK != ret) { //if vpc process with error, return directly, and then process next.
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] call DVPP_CTL_VPC_PROC process faild");
        if (alignMmapFlag) { //release memory
            HIAI_DVPP_DFree(alignBuffer);
            alignBuffer = NULL;
        }
        return HIAI_ERROR;
    }
    if (alignMmapFlag) { //release memory
        HIAI_DVPP_DFree(alignBuffer);
        alignBuffer = NULL;
    }
    return HIAI_OK;
}

int ImagePreProcess_1::HandleVpcWithParam(const unsigned char* buffer, const int &width, const int &height, const long &bufferSize,
        const ImageData<u_int8_t> &img, const FILE_TYPE &type, const int &format)
{
    int realWidth = img.width;
    int realHeight = img.height;

    HIAI_ENGINE_LOG("[ImagePreProcess_1] realWidth: %d, realHeight: %d, width: %d, height: %d", realWidth, realHeight, width, height);

    if (NULL == pidvppapi_ || NULL == buffer) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]pidvppapi_ is null");
        return HIAI_ERROR;
    }

    VpcUserImageConfigure userImage;
    string paraSetPath[1];
    if (type == FILE_TYPE_PIC_JPEG) {
       switch(format) { //format is responding to color sapce after decoder, which is needed to match the input color space of vpc
            case DVPP_JPEG_DECODE_OUT_YUV444:
                userImage.inputFormat = INPUT_YUV444_SEMI_PLANNER_VU;
                break;
            case DVPP_JPEG_DECODE_OUT_YUV422_H2V1:
                userImage.inputFormat = INPUT_YUV422_SEMI_PLANNER_VU;
                break;
            case DVPP_JPEG_DECODE_OUT_YUV420:
                userImage.inputFormat = INPUT_YUV420_SEMI_PLANNER_VU;
                break;
            case DVPP_JPEG_DECODE_OUT_YUV400:
                userImage.inputFormat = INPUT_YUV400;
                break;
            default:
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]Jpegd out format[%d] is error", format);
                break;
       }
    } else if (type == FILE_TYPE_PIC_PNG) {
        switch(format) {
            case DVPP_PNG_DECODE_OUT_RGB:
                userImage.inputFormat = INPUT_RGB;
                break;
            case DVPP_PNG_DECODE_OUT_RGBA:
                userImage.inputFormat = INPUT_RGBA;
                break;
            default:
                HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]Pngd out format[%d] is error", format);
                break;
        }
    } else {
        userImage.inputFormat = INPUT_YUV420_SEMI_PLANNER_UV;
    }
    userImage.widthStride = width;
    userImage.heightStride = height;
    userImage.outputFormat = OUTPUT_YUV420SP_UV;
    userImage.bareDataAddr = (uint8_t *)buffer;
    userImage.bareDataBufferSize = bufferSize;

    VpcUserRoiConfigure roiConfigure;
    roiConfigure.next = nullptr;
    VpcUserRoiInputConfigure *inputConfigure = &roiConfigure.inputConfigure;
    inputConfigure->cropArea.leftOffset = 0;
    inputConfigure->cropArea.rightOffset = realWidth - 1;
    inputConfigure->cropArea.upOffset = 0;
    inputConfigure->cropArea.downOffset = realHeight - 1;

    uint32_t resizeWidth = (uint32_t)dvppConfig_->resize_width;
    uint32_t resizeHeight = (uint32_t)dvppConfig_->resize_height;
    if (0 == resizeWidth || 0 == resizeHeight) {
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1] user donnot need resize, resize width/height use real size of pic");
        resizeWidth = realWidth;
        resizeHeight = realHeight;
    }
    if (resizeWidth > DVPP_SUPPORT_MAX_WIDTH || resizeWidth < DVPP_SUPPORT_MIN_WIDTH
        || resizeHeight > DVPP_SUPPORT_MAX_HEIGHT || resizeHeight < DVPP_SUPPORT_MIN_HEIGHT) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]resize range error, resizeWidth:%u, resizeHeight:%u", resizeWidth, resizeHeight);
        return HIAI_ERROR;
    }

    VpcUserRoiOutputConfigure *outputConfigure = &roiConfigure.outputConfigure;
    if (ProcessCrop(inputConfigure->cropArea, width, height, realWidth, realHeight)) {
        //restriction: leftOffset and upOffset of outputArea must be even, rightOffset and downOffset of outputArea must be odd.
        outputConfigure->outputArea.leftOffset = 0;
        outputConfigure->outputArea.rightOffset = CHECK_ODD(resizeWidth - 1);
        outputConfigure->outputArea.upOffset = 0;
        outputConfigure->outputArea.downOffset = CHECK_ODD(resizeHeight - 1);
        outputConfigure->widthStride = ALIGN_UP(resizeWidth, VPC_OUT_WIDTH_STRIDE);
        outputConfigure->heightStride = ALIGN_UP(resizeHeight, VPC_OUT_HIGH_STRIDE);
        outputConfigure->bufferSize = outputConfigure->widthStride * outputConfigure->heightStride * 3 / 2;
        outputConfigure->addr = static_cast<uint8_t*>(HIAI_DVPP_DMalloc(outputConfigure->bufferSize));
        if (nullptr == outputConfigure->addr) {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]outputConfigure->addr is null");
            return HIAI_ERROR;
        }
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1]inputConfigure cropArea:%u, %u, %u, %u, outputConfigure outputArea:%u, %u, %u, %u, stride:%u, %u, %u",
            inputConfigure->cropArea.leftOffset, inputConfigure->cropArea.rightOffset, inputConfigure->cropArea.upOffset, inputConfigure->cropArea.downOffset,
            outputConfigure->outputArea.leftOffset, outputConfigure->outputArea.rightOffset, outputConfigure->outputArea.upOffset, outputConfigure->outputArea.downOffset,
            outputConfigure->widthStride, outputConfigure->heightStride, outputConfigure->bufferSize);
    } else {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]ProcessCrop error");
        return HIAI_ERROR;
    }
    userImage.roiConfigure = &roiConfigure;

    if (!dvppConfig_->dvpp_para.empty())
    {
        paraSetPath[0] += dvppConfig_->dvpp_para.c_str();
        userImage.yuvScalerParaSetAddr = reinterpret_cast<uint64_t>(paraSetPath);
        userImage.yuvScalerParaSetSize = 1;
        userImage.yuvScalerParaSetIndex = 0;
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1_1]dvpp_para:%s", dvppConfig_->dvpp_para.c_str());
    }

    dvppapi_ctl_msg dvppApiCtlMsg;
    dvppApiCtlMsg.in = (void *)&userImage;
    dvppApiCtlMsg.in_size = sizeof(VpcUserImageConfigure);
    if(0 != DvppCtl(pidvppapi_, DVPP_CTL_VPC_PROC, &dvppApiCtlMsg)) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] call dvppctl process:VPC faild");
        HIAI_DVPP_DFree(outputConfigure->addr);
        outputConfigure->addr = nullptr;
        return HIAI_ERROR;
    }

    if(NULL == dvppOut_) {
        dvppOut_ = std::make_shared<BatchImageParaWithScaleT>();
        if (NULL == dvppOut_ || NULL == dvppOut_.get()) {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] call dvppOut_ make_shared failed");
            HIAI_DVPP_DFree(outputConfigure->addr);
            outputConfigure->addr = nullptr;
            return HIAI_ERROR;
        }
        dvppOut_->b_info = dvppIn_->b_info;
        dvppOut_->b_info.frame_ID.clear();
    }

    std::shared_ptr<NewImageParaT> image = std::make_shared<NewImageParaT>();
    if (NULL == image || NULL == image.get()) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] call image make_shared failed");
        HIAI_DVPP_DFree(outputConfigure->addr);
        outputConfigure->addr = nullptr;
        return HIAI_ERROR;
    }

    image->img.width = outputConfigure->outputArea.rightOffset - outputConfigure->outputArea.leftOffset + 1;
    image->img.height = outputConfigure->outputArea.downOffset - outputConfigure->outputArea.upOffset + 1;
    image->img.size = outputConfigure->bufferSize;
    image->img.channel = img.channel;
    image->img.format = img.format;
    image->scale_info.scale_width = (1.0 * image->img.width)/img.width;
    image->scale_info.scale_height = (1.0 * image->img.height)/img.height;
    image->resize_info.resize_width = dvppConfig_->resize_width;
    image->resize_info.resize_height = dvppConfig_->resize_height;
    image->crop_info.point_x = dvppConfig_->point_x;
    image->crop_info.point_y = dvppConfig_->point_y;
    image->crop_info.crop_width = dvppConfig_->crop_width;
    image->crop_info.crop_height = dvppConfig_->crop_height;

    uint8_t *tmp = nullptr;
    try {
        tmp = new uint8_t[image->img.size];
    }
    catch (const std::bad_alloc& e) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] failed to allocate buffer for outBuffer");
        return HIAI_ERROR;
    }
    std::shared_ptr<u_int8_t> outBuffer(tmp);
    if (NULL == outBuffer || NULL == outBuffer.get()) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] call outBuffer shared_ptr failed");
        HIAI_DVPP_DFree(outputConfigure->addr);
        outputConfigure->addr = nullptr;
        return HIAI_ERROR;
    }
    int ret = memcpy_s(outBuffer.get(), image->img.size, outputConfigure->addr, image->img.size);
    HIAI_DVPP_DFree(outputConfigure->addr);
    outputConfigure->addr = nullptr;
    if (ret != 0) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] memcpy_s out buffer data error");
        outBuffer = nullptr;
        return HIAI_ERROR;
    }
    image->img.data = outBuffer;
    dvppOut_->v_img.push_back(*image);
    dvppOut_->b_info.frame_ID.push_back(imageFrameID_);

    if (1 == dvppConfig_->dump_value) {
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1]preprocess need dump, width:%u, height:%u", image->img.width, image->img.height);
        DvppPreprocessInfo info = {resizeWidth, resizeHeight, outputConfigure->widthStride, outputConfigure->heightStride, imageFrameID_, orderInFrame_};
        return StorePreprocessImage(outBuffer.get(), image->img.size, info);
    }

    return HIAI_OK;
}

int ImagePreProcess_1::StorePreprocessImage(const u_int8_t *outBuffer, const uint32_t &size, const DvppPreprocessInfo &info)
{
    if(NULL == outBuffer) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]StorePreprocessImage, outBuffer is null!");
        return HIAI_ERROR;
    }
    time_t now = time(0);
    //important:after dvpp, the output data is that width is aligned with 128, and height is aligned with 16.
    std::string fileName = dvppConfig_->userHome + "/projects/" + dvppConfig_->project_name + "/out/PreProcess/ImagePreProcess_1/"
        + std::to_string(imageFrameID_) + "_" + std::to_string(orderInFrame_ + 1) + "_" + std::to_string(now) + "_preprocessYUV";
    IDE_SESSION session = ideOpenFile(NULL, (char *)fileName.c_str());
    HIAI_ENGINE_LOG("save fileName:%s!", fileName.c_str());
    if (NULL == session) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]StorePreprocessImage, ideOpenFile is null!");
        return HIAI_ERROR;
    }

    uint32_t sendSize = 0;
    char *tmp = (char *)outBuffer;
    while (sendSize < size) {
        uint32_t everySize = (sendSize + SEND_BUFFER_SIZE > size)  ? (size - sendSize) : SEND_BUFFER_SIZE;
        if (IDE_DAEMON_NONE_ERROR != ideWriteFile(session, tmp, everySize)) {
            HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]StorePreprocessImage, ideWriteFile error!");
            ideCloseFile(session);
            return HIAI_ERROR;
        }
        tmp += everySize;
        sendSize += everySize;
    }

    HIAI_ENGINE_LOG("preprocess info, resiz_width:%u, resize_height:%u, preprocess_width:%u, preprocess_height:%u, frameID:%u, order:%u",
        info.resize_width, info.resize_height, info.preprocess_width, info.preprocess_height, info.frameID, info.orderInFrame);
    //add info of yuv file to file end which are used to transfer to jpg
    if (IDE_DAEMON_NONE_ERROR != ideWriteFile(session, &info, sizeof(info))) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]StorePreprocessImage, ideWriteFile error!");
        ideCloseFile(session);
        return HIAI_ERROR;
    }

    usleep(SEND_DATA_SLEEP_MS); //assure ide-daemon-host catch all data.
    if (IDE_DAEMON_NONE_ERROR != ideCloseFile(session)) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]StorePreprocessImage, ideCloseFile error!");
        return HIAI_ERROR;
    }
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1]StorePreprocessImage, storage success!, sendSize:%u, size:%u", sendSize, size);
    return HIAI_OK;
}

int ImagePreProcess_1::HandleDvpp()
{
    if (NULL == dvppIn_ || NULL == dvppIn_.get()) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]__hanlde_dvpp, input data is null!");
        return HIAI_ERROR;
    }

    int indexCrop = 0;
    int i = 0;
    orderInFrame_ = 0;
    for (std::vector<NewImageParaT>::iterator iter = dvppIn_->v_img.begin(); iter != dvppIn_->v_img.end(); ++iter, i++) {
        imageFrameID_ = dvppIn_->b_info.frame_ID[i];
        if (IMAGE_TYPE_JPEG == (ImageTypeT)(*iter).img.format) {
            if(NULL != dvppCropIn_ && NULL != dvppCropIn_.get()) {
                if(dvppCropIn_->v_location.empty() || dvppCropIn_->v_location[indexCrop].range.empty()) {
                    continue;
                }
                for(std::vector<Rectangle<Point2D>>::iterator iterRect = dvppCropIn_->v_location[indexCrop].range.begin(); iterRect != dvppCropIn_->v_location[indexCrop].range.end(); ++iterRect) {
                    orderInFrame_++;
                    if (HIAI_OK != UpdateCropPara(*iterRect) || HIAI_OK != HandleJpeg((*iter).img)) {
                        HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[ImagePreProcess_1]Handle One Image Error, Continue Next");
                        continue;
                    }
                }
            } else {
                if (HIAI_OK != HandleJpeg((*iter).img)) {
                    HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[ImagePreProcess_1]Handle One Image Error, Continue Next");
                    continue;
                }
            }
        } else if (IMAGE_TYPE_PNG == (ImageTypeT)(*iter).img.format) {
            if(NULL != dvppCropIn_ && NULL != dvppCropIn_.get()) {
                if(dvppCropIn_->v_location.empty() || dvppCropIn_->v_location[indexCrop].range.empty()){
                    continue;
                }
                for(std::vector<Rectangle<Point2D>>::iterator iterRect = dvppCropIn_->v_location[indexCrop].range.begin(); iterRect != dvppCropIn_->v_location[indexCrop].range.end(); ++iterRect) {
                    orderInFrame_++;
                    if (HIAI_OK != UpdateCropPara(*iterRect) || HIAI_OK != HandlePng((*iter).img)) {
                        HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[ImagePreProcess_1]Handle One Image Error, Continue Next");
                        continue;
                    }
                }
            } else {
                if (HIAI_OK != HandlePng((*iter).img)) {
                    HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[ImagePreProcess_1]Handle One Image Error, Continue Next");
                    continue;
                }
            }
        } else { // default IMAGE_TYPE_NV12
            if (NULL != dvppCropIn_ && NULL != dvppCropIn_.get()) {
                if(dvppCropIn_->v_location.empty() || dvppCropIn_->v_location[indexCrop].range.empty()) {
                    continue;
                }
                for (std::vector<Rectangle<Point2D>>::iterator iterRect = dvppCropIn_->v_location[indexCrop].range.begin(); iterRect != dvppCropIn_->v_location[indexCrop].range.end(); ++iterRect) {
                    orderInFrame_++;
                    if (HIAI_OK != UpdateCropPara(*iterRect) || HIAI_OK != HandleVpc((*iter).img)) {
                        HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[ImagePreProcess_1]Handle One Image Error, Continue Next");
                        continue;
                    }
                }
            } else {
                if (HIAI_OK != HandleVpc((*iter).img)) {
                     HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[ImagePreProcess_1]Handle One Image Error, Continue Next");
                        continue;
                }
            }
        }
        indexCrop++;
    }

    return HIAI_OK;
}

/**
* @ingroup hiaiengine
* @brief HIAI_DEFINE_PROCESS: realize ImagePreProcess_1
* @[in]: define a input and output,
*        and register Engine named ImagePreProcess_1
*/
HIAI_IMPL_ENGINE_PROCESS("ImagePreProcess_1", ImagePreProcess_1, INPUT_SIZE)
{
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1] start process!");
    int errCode = HIAI_OK;
    std::shared_ptr<BatchImageParaWithScaleT> errorHandler = NULL;

    if(arg0 != nullptr){
        std::shared_ptr<BatchImageParaWithScaleT> dataInput = std::static_pointer_cast<BatchImageParaWithScaleT>(arg0);
        if (!isSentinelImage(dataInput)) {
            if (dataInputIn_ != nullptr) {
                if (dataInputIn_->b_info.batch_ID == dataInput->b_info.batch_ID && !dataInput->v_img.empty() && !dataInput->b_info.frame_ID.empty()) {
                    dataInputIn_->v_img.push_back(dataInput->v_img[0]);
                    dataInputIn_->b_info.frame_ID.push_back(dataInput->b_info.frame_ID[0]);
                }
            } else {
                dataInputIn_ = std::make_shared<BatchImageParaWithScaleT>();
                if (dataInputIn_ == nullptr){
                    HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[ImagePreProcess_1] malloc error");
                    return HIAI_ERROR;
                }
                for (int i = 0; i < dataInput->b_info.frame_ID.size(); ++i){
                    dataInputIn_->b_info.frame_ID.push_back(dataInput->b_info.frame_ID[i]);
                }
                dataInputIn_->b_info.batch_size = dataInput->b_info.batch_size;
                dataInputIn_->b_info.max_batch_size = dataInput->b_info.max_batch_size;
                dataInputIn_->b_info.batch_ID = dataInput->b_info.batch_ID;
                dataInputIn_->b_info.is_first = dataInput->b_info.is_first;
                dataInputIn_->b_info.is_last = dataInput->b_info.is_last;
                for(int i = 0; i < dataInput->v_img.size(); ++i){
                    dataInputIn_->v_img.push_back(dataInput->v_img[i]);
                }
            }
            if (dataInputIn_->v_img.size() != dataInputIn_->b_info.batch_size) {
                HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1] Wait for other %d batch image info!", (dataInputIn_->b_info.batch_size - dataInputIn_->v_img.size()));
                return HIAI_OK;
            }
            inputQue_.PushData(0, dataInputIn_);
            dataInputIn_ = nullptr;
        } else {
            inputQue_.PushData(0, arg0);
        }
    }
#if INPUT_SIZE == 2
    inputQue_.PushData(1, arg1);
    if (!inputQue_.PopAllData(dvppIn_, dvppCropIn_)) {
        HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[ImagePreProcess_1]fail to pop all data");
        return HIAI_ERROR;
    }
#else
    if (!inputQue_.PopAllData(dvppIn_)) {
        HIAI_ENGINE_LOG(HIAI_IDE_WARNING, "[ImagePreProcess_1]fail to pop all data");
        return HIAI_ERROR;
    }
#endif
    //add sentinel image for showing this data in dataset are all sended, this is last step.
    if (isSentinelImage(dvppIn_)) {
        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1]sentinel Image, process over.");
        HIAI_StatusT hiaiRet = HIAI_OK;
        do{
            hiaiRet = SendData(0, "BatchImageParaWithScaleT", std::static_pointer_cast<void>(dvppIn_));
            if (HIAI_OK != hiaiRet) {
                if (HIAI_ENGINE_NULL_POINTER == hiaiRet || HIAI_HDC_SEND_MSG_ERROR == hiaiRet || HIAI_HDC_SEND_ERROR == hiaiRet
                    || HIAI_GRAPH_SRC_PORT_NOT_EXIST == hiaiRet || HIAI_GRAPH_ENGINE_NOT_EXIST == hiaiRet) {
                    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1] SendData error[%d], break.", hiaiRet);
                    break;
                }
                HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1] SendData return value[%d] not OK, sleep 200ms", hiaiRet);
                usleep(SEND_DATA_INTERVAL_MS);
            }
        } while (HIAI_OK != hiaiRet);
        return hiaiRet;
    }

    dvppOut_ = NULL;

    // 1.check the pending data is jpeg, png, yuv or none
    // 2.preprocess before vpc(DVPP_CTL_JPEGD_PROC, DVPP_CTL_PNGD_PROC, or something else)
    // 3.calculate width, height, and so on, according to the user config of crop and resize, and DVPP_CTL_TOOL_CASE_GET_RESIZE_PARAM
    // 4.DVPP_CTL_VPC_PROC after create dvppapi_ctl_msg para
    // 5.dump yuv data to project dir out after dvpp, according to dump_value config(from device to IDE with IDE-deamon-client&IDE-deamon-hiai process)
    // 6.send data to next engine after the process of dvpp
    errCode = HandleDvpp();
    if (HIAI_ERROR == errCode) {
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[ImagePreProcess_1]dvpp process error!");
        goto ERROR;
    }

    if(!SendPreProcessData()) { // send to next engine after dvpp process
        goto ERROR;
    }
    ClearData();
    HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[ImagePreProcess_1] end process!");
    return HIAI_OK;

ERROR:
    //send null to next node to avoid blocking when to encounter abnomal situation.
    SendData(0, "BatchImageParaWithScaleT", std::static_pointer_cast<void>(errorHandler));
    ClearData();
    return HIAI_ERROR;
}


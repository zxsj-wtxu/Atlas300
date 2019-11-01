
/**
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef DVPP_VPC_H
#define DVPP_VPC_H

#include <cstdint>

// Supported Input Format
enum VpcInputFormat {
    INPUT_YUV400, // 0
    INPUT_YUV420_SEMI_PLANNER_UV, // 1
    INPUT_YUV420_SEMI_PLANNER_VU, // 2
    INPUT_YUV422_SEMI_PLANNER_UV, // 3
    INPUT_YUV422_SEMI_PLANNER_VU, // 4
    INPUT_YUV444_SEMI_PLANNER_UV, // 5
    INPUT_YUV444_SEMI_PLANNER_VU, // 6
    INPUT_YUV422_PACKED_YUYV, // 7
    INPUT_YUV422_PACKED_UYVY, // 8
    INPUT_YUV422_PACKED_YVYU, // 9
    INPUT_YUV422_PACKED_VYUY, // 10
    INPUT_YUV444_PACKED_YUV, // 11
    INPUT_RGB, // 12
    INPUT_BGR, // 13
    INPUT_ARGB, // 14
    INPUT_ABGR, // 15
    INPUT_RGBA, // 16
    INPUT_BGRA, // 17
    INPUT_YUV420_SEMI_PLANNER_UV_10BIT, // 18
    INPUT_YUV420_SEMI_PLANNER_VU_10BIT, // 19
};

// Supported Output Format
enum VpcOutputFormat {
    OUTPUT_YUV420SP_UV,
    OUTPUT_YUV420SP_VU
};

// Data Configuration in the Compressed Format
struct VpcCompressDataConfigure {
    uint64_t lumaHeadAddr;    
    uint64_t chromaHeadAddr; 
    uint32_t lumaHeadStride;
    uint32_t chromaHeadStride;
    uint64_t lumaPayloadAddr;  
    uint64_t chromaPayloadAddr;
    uint32_t lumaPayloadStride;
    uint32_t chromaPayloadStride;

    VpcCompressDataConfigure() {
        lumaHeadAddr = 0;
        chromaHeadAddr = 0;
        lumaHeadStride = 0;
        chromaHeadStride = 0;
        lumaPayloadAddr = 0;
        chromaPayloadAddr = 0;
        lumaPayloadStride = 0;
        chromaPayloadStride = 0;
    }
};

// Configuration of the crop area
struct VpcUserCropConfigure {
    uint32_t leftOffset;
    uint32_t rightOffset;
    uint32_t upOffset;
    uint32_t downOffset;
    uint64_t reserve1;

    VpcUserCropConfigure() {
        leftOffset = 0;
        rightOffset = 0;
        upOffset = 0;
        downOffset = 0;
        reserve1 = 0;
    }
};

// AI the core has this requirement, the total amount of each component of the output yuv image needs to be calculated.
struct VpcUserYuvSum {
    uint32_t ySum;
    uint32_t uSum;
    uint32_t vSum;
    uint64_t reserve1;

    VpcUserYuvSum() {
        ySum = 0;
        uSum = 0;
        vSum = 0;
        reserve1 = 0;
    }
};

// Configuration of the input data in the drawing part of the user
struct VpcUserRoiInputConfigure {
    VpcUserCropConfigure cropArea;
    uint64_t reserve1;

    VpcUserRoiInputConfigure() {
        reserve1 = 0;
    }
};

// Configuration of the output data in the drawing part of the user
struct VpcUserRoiOutputConfigure {
    uint8_t* addr; // Initial address of the output picture.
    uint32_t bufferSize; // Size of the output buffer. The value is calculated based on the value of yuv420sp.
    uint32_t widthStride; // Width stride of the output picture.
    uint32_t heightStride; // If the output is an yuv420sp picture, the start address of the uv data needs to be calculated based on the heightStride.

    //Specifies the coordinates of the output region specified by the user.
    VpcUserCropConfigure outputArea;
    uint64_t reserve1;

    VpcUserRoiOutputConfigure() {
        addr = nullptr;
        bufferSize = 0;
        widthStride = 0;
        heightStride = 0;
        reserve1 = 0;
    }
};

struct VpcUserRoiConfigure {
    VpcUserRoiInputConfigure inputConfigure;
    VpcUserRoiOutputConfigure outputConfigure;
    VpcUserRoiConfigure* next; 
    uint64_t reserve1;

    VpcUserRoiConfigure () {
        next = nullptr;
        reserve1 = 0;
    }
};

// Reserved interface for performance optimization
struct VpcUserPerformanceTuningParameter {
    uint64_t reserve1;
    uint64_t reserve2;
    uint64_t reserve3;
    uint64_t reserve4;
    uint64_t reserve5;

    VpcUserPerformanceTuningParameter() {
        reserve1 = 0;
        reserve2 = 0;
        reserve3 = 0;
        reserve4 = 0;
        reserve5 = 0;
    }
};

// Configuration for the user to invoke the VPC
struct VpcUserImageConfigure {
    //Enter the image address in the non-compressed format. If the image is in compressed format, you do not need to set this parameter.
    uint8_t* bareDataAddr;
    // The memory size is calculated based on the width and height of the image. 
    // If the image is in compressed format, you do not need to set this parameter.
    uint32_t bareDataBufferSize;
    // Calculate the address of the next row of images based on widthStride. 
    // Pay attention to different image formats. The stride and width ratio are different.
    uint32_t widthStride;
    // If the picture is an yuv semi planner picture, the start address of the uv data is calculated based on the heightStride.
    uint32_t heightStride; 
    enum VpcInputFormat inputFormat;
    enum VpcOutputFormat outputFormat;
    // Configuring a Picking Area to Support One Image and Multiple Frames
    VpcUserRoiConfigure* roiConfigure;

    // Configuration of the input picture data compressed by the hfbc. 
    // Multiple values need to be configured for the compressed picture data. 
    // This source is the vdec module.
    bool isCompressData;
    VpcCompressDataConfigure compressDataConfigure;

    // The AI CORE collects statistics on the total amount of each component of the output picture. 
    // If this requirement is met, only one picture and one frame are supported.
    // To obtain the value of yuvSum, set yuvSumEnable to ture.
    // If yuvSum is not required, leave this parameter blank. The default value is false.
    bool yuvSumEnable;
    VpcUserYuvSum yuvSum;
    
    VpcUserPerformanceTuningParameter tuningParameter; // This variable is reserved for parameter optimization.
    uint32_t* cmdListBufferAddr; // This variable is reserved.
    uint32_t cmdListBufferSize; // This variable is reserved.
    
    uint64_t yuvScalerParaSetAddr;
    uint16_t yuvScalerParaSetSize;
    uint16_t yuvScalerParaSetIndex;

    uint8_t isUseMultiCoreAccelerate;
    uint8_t reserve1;
    uint16_t reserve2;

    VpcUserImageConfigure() {
        bareDataAddr = nullptr;
        bareDataBufferSize = 0;
        widthStride = 0;
        heightStride = 0;
        inputFormat = INPUT_YUV420_SEMI_PLANNER_UV;
        outputFormat = OUTPUT_YUV420SP_UV;
        roiConfigure = nullptr;
        isCompressData = false;
        yuvSumEnable = false;
        cmdListBufferAddr = nullptr;
        cmdListBufferSize = 0;
        yuvScalerParaSetAddr = 0;
        yuvScalerParaSetSize = 1;
        yuvScalerParaSetIndex = 0;
        isUseMultiCoreAccelerate = 0;
        reserve1 = 0;
        reserve2 = 0;
    }
};

#endif // DVPP_VPC_H
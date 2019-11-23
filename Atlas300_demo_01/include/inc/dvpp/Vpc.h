
/**
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef DVPP_VPC_H
#define DVPP_VPC_H

#include <cstdint>

// 支持的输入格式
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

// 支持的输出格式
enum VpcOutputFormat {
    OUTPUT_YUV420SP_UV,
    OUTPUT_YUV420SP_VU
};

// 压缩格式的数据配置
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

// crop区域的配置
struct VpcUserCropConfigure {
    uint32_t leftOffset; // 左偏移
    uint32_t rightOffset; // 右偏移
    uint32_t upOffset; // 上偏移
    uint32_t downOffset; // 下偏移
    uint64_t reserve1;

    VpcUserCropConfigure() {
        leftOffset = 0;
        rightOffset = 0;
        upOffset = 0;
        downOffset = 0;
        reserve1 = 0;
    }
};

// AI CORE有这个需求，需要统计输出yuv图像每个分量的总量
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

// 用户抠图部分的输入数据的相关配置
struct VpcUserRoiInputConfigure {
    VpcUserCropConfigure cropArea;
    uint64_t reserve1;

    VpcUserRoiInputConfigure() {
        reserve1 = 0;
    }
};

// 用户抠图部分的输出数据的相关配置
struct VpcUserRoiOutputConfigure {
    uint8_t* addr; // 输出图片的首地址
    uint32_t bufferSize; // 输出buffer的大小,根据yuv420sp计算
    uint32_t widthStride; // 输出图片的宽stride
    uint32_t heightStride; // 输出为yuv420sp图像，需要根据heightStride计算出uv数据的起始地址

    //用户指定输出区域坐标
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

// 性能调优预留接口
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

// 用户调用vpc的配置
struct VpcUserImageConfigure {
    uint8_t* bareDataAddr; // 非压缩格式输入图片地址,当图像为压缩格式时，不需要填
    uint32_t bareDataBufferSize; // 内存大小是根据图像的宽高stride计算出来的，当图像为压缩格式时，不需要填
    uint32_t widthStride; // 根据widthStride计算出下一行图像的地址,注意不同图像格式，stride与宽比例不同
    uint32_t heightStride; // 如果是yuv semi planner图像，根据heightStride计算出uv数据的起始地址
    enum VpcInputFormat inputFormat;
    enum VpcOutputFormat outputFormat;
    VpcUserRoiConfigure* roiConfigure; // 配置抠图区域，支持一图多框

    // 进过hfbc压缩过的输入图片数据的配置，压缩过的图片数据要配置多项值，此来源为vdec模块。
    bool isCompressData;
    VpcCompressDataConfigure compressDataConfigure;

    // AI CORE有统计输出图像每个分量的总量的需求，有这个需求时，只支持一图一框
    // 如果想要得到yuvSum值，需要将yuvSumEnable 赋值为ture
    bool yuvSumEnable; // 不要yuvSum值时，可以不填，默认值为false
    VpcUserYuvSum yuvSum;
    
    VpcUserPerformanceTuningParameter tuningParameter; // 此变量为预留,用于参数调优
    uint32_t* cmdListBufferAddr; // 此变量为预留
    uint32_t cmdListBufferSize; // 此变量为预留
    
    uint64_t yuvScalerParaSetAddr;
    uint16_t yuvScalerParaSetSize;
    uint16_t yuvScalerParaSetIndex;
    uint32_t reserve1;

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
        reserve1 = 0;
    }
};

#endif // DVPP_VPC_H
/**
 * ============================================================================
 *
 * Copyright (C) 2018, Hisilicon Technologies Co., Ltd. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1 Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   2 Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   3 Neither the names of the copyright holders nor the names of the
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * ============================================================================
 */

#ifndef ASCENDDK_ASCEND_EZDVPP_DVPP_DATA_TYPE_H_
#define ASCENDDK_ASCEND_EZDVPP_DVPP_DATA_TYPE_H_

#include "dvpp/Vpc.h"
#include "dvpp/dvpp_config.h"

// The memory size of the BGR image is 3 times that of width*height.
#define DVPP_BGR_BUFFER_MULTIPLE 3

// The memory size of the YUV image is 1.5 times that of width*height.
#define DVPP_YUV420SP_SIZE_MOLECULE    3
#define DVPP_YUV420SP_SIZE_DENOMINATOR 2

namespace ascend {
namespace utils {
// The width of image in vpc interface need 128-byte alignment
const int kVpcWidthAlign = 128;

// The height of image in vpc interface need 16-byte alignment
const int kVpcHeightAlign = 16;

// The address of input data in vpc interface need 128-byte alignment
const int kVpcAddressAlign = 128;

// call dvpp interface error
const int kDvppReturnError = -1;

// call dvpp interface success
const int kDvppReturnOk = 0;

// The address of input data in JpegE interface need 128-byte alignment
const unsigned int kJpegEAddressAlgin = 128;

// The address of input data in JpegD interface need 128-byte alignment
const unsigned int kJpegDAddressAlgin = 128;

// The width of image in JpegE interface need 16-byte alignment
const int kJpegEWidthAlgin = 16;

// The height of image in JpegE interface need 16-byte alignment
const int kJpegEHeightAlign = 16;

// The width of image in JpegE interface need to be compatible with 16-byte
// alignment
const int kJpegECompatWidthAlign = 128;

// The memory size of each line in yuv422sp is twice the width
const int kYuv422SPWidthMul = 2;

// The memory size of each line in yuv444sp is twice the width
const int kYuv444SPWidthMul = 2;

// The memory size of each line in yuv422 packed is twice the width
const int kYuv422PackedWidthMul = 2;

// The memory size of each line in yuv444 packed is three times the width
const int kYuv444PackedWidthMul = 3;

// The memory size of each line in rgb888 is three times the width
const int kRgb888WidthMul = 3;

// The memory size of each line in xrgb8888 is four times the width
const int kXrgb888WidthMul = 4;

// Bgr image need memory align
const int kImageNeedAlign = 0;

// Bgr image don't need memory align
const int kImageNotNeedAlign = 1;

// max increase scale in resize function
const double kMaxIncrease = 16.0;

// min increase scale in resize function
const double kMinIncrease = 0.03125;

// Use xrgb image(max width:4096; max height:4096) memory as validation
// standard: 4096 * 4096 * 4 = 67108864 (64M)
const int kAllowedMaxImageMemory = 67108864;

struct ErrorDescription {
    int code;
    std::string code_info;
};

enum DvppEncodeType {
    kH265Main = 0,  // H265-main level
    kH264Base = 1,  // H264-baseline level
    kH264Main = 2,  // H264-main level
    kH264High = 3,  // H264-high level
};

enum YuvType {
    kYuv420sp,  // YUV420 semi-planner
    kYvu420sp,  // YVU420 semi-planner
};

enum CaptureObjFlag {
    kJpeg,  // convert to jpg
    kH264,  // convert to h264
    kYuv,  // bgr convert to yuv
    kCropOrResize,  // crop or resize image
    kJpegD,  // jpg convert to yuv
    kBasicVpc,
};

enum DvppErrorCode {
    kDvppOperationOk = 0,
    kDvppErrorInvalidParameter = -1,
    kDvppErrorMallocFail = -2,
    kDvppErrorCreateDvppFail = -3,
    kDvppErrorDvppCtlFail = -4,
    kDvppErrorNoOutputInfo = -5,
    kDvppErrorMemcpyFail = -6,
    kDvppErrorNewFail = -7,
    kDvppErrorCheckMemorySizeFail = -8,
}
;

struct ResolutionRatio {
    int width = 0;
    int height = 0;
};

struct DvppToJpgPara {
    // used to indicate the input format.
    eEncodeFormat format = JPGENC_FORMAT_NV12;

    // used to indicate the output quality while output is jpg.
    int level = 100;

    // image resolution.
    ResolutionRatio resolution;

    // false: Input image is not aligned; true: Input image is aligned
    bool is_align_image = false;
};

struct DvppOutput {
    unsigned char *buffer;  // output buffer
    unsigned int size;  // size of output buffer
};

struct DvppBasicVpcPara {
    // input image format
    VpcInputFormat input_image_type = INPUT_YUV420_SEMI_PLANNER_UV;

    // src image resolution
    ResolutionRatio src_resolution;

    // x-axis of upper left corner
    int crop_left = 0;

    // y-axis of upper left corner
    int crop_up = 0;

    // x-axis of lower right corner
    int crop_right = 0;

    // y-axis of lower right corner
    int crop_down = 0;

    // output image format
    VpcOutputFormat output_image_type = OUTPUT_YUV420SP_UV;

    // dest image resolution
    ResolutionRatio dest_resolution;

    // false:input image is not aligned; true:input image is aligned
    bool is_input_align = false;

    // true:output image need alignment;
    // false:output image does not need alignment
    bool is_output_align = true;
};

struct DvppVpcOutput {
    uint8_t *buffer;
    uint32_t size;
};

struct DvppJpegDInPara {
    bool is_convert_yuv420 = false;  // true: jpg convert to yuv420sp
// false:jpg retain original sampling format
};

struct DvppJpegDOutput {
    unsigned char *buffer;  // output buffer
    uint32_t buffer_size;  // output buffer size
    uint32_t width;  // the width of output image
    uint32_t height;  // the height of output image
    uint32_t aligned_width;  // the aligned width of output image
    uint32_t aligned_height;  // the aligned height of output image
    VpcInputFormat image_format;  //output image format
};

struct DvppPara {
    DvppToJpgPara jpg_para;
    DvppJpegDInPara jpegd_para;
    DvppBasicVpcPara basic_vpc_para;
};
}
}

#endif /* ASCENDDK_ASCEND_EZDVPP_DVPP_DATA_TYPE_H_ */

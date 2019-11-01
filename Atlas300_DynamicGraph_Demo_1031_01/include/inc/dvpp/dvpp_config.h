/**
 * Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef DVPP_DVPP_CONFIG_H
#define DVPP_DVPP_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <memory>
#include <vector>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include "securec.h"
#include "vdec_hiai.h"

using namespace std;

#define API_MAP_VA32BIT 0x200
#define DVPP_SUCCESS (0)
#define DVPP_FAIL (-1)

#define DVPP_CTL_VPC_PROC 0
#define DVPP_CTL_PNGD_PROC 1
#define DVPP_CTL_JPEGE_PROC 2
#define DVPP_CTL_JPEGD_PROC 3
#define DVPP_CTL_VDEC_PROC 4
#define DVPP_CTL_VENC_PROC 5
#define DVPP_CTL_DVPP_CAPABILITY 6
#define DVPP_CTL_CMDLIST_PROC 7
#define DVPP_CTL_TOOL_CASE_GET_RESIZE_PARAM 8
// default param
#define VPC_OUT_WIDTH_STRIDE (128)
#define VPC_OUT_HIGH_STRIDE (16)
#define SMOKE_TEST_RES_BASE_DIR  "/root/device/dvpp/"

const int32_t GET_JPEGE_OUT_PARAMETER = 0;
const int32_t GET_JPEGD_OUT_PARAMETER = 1;
const int32_t GET_PNGD_OUT_PARAMETER  = 2;

// vpc param
const int vpc_default_width = 16;
const int vpc_default_high = 16;
const int vpc_default_bit_width = 8;
const int vpc_max_width = 4096;
const int vpc_min_width = 16;
const int vpc_max_high = 8192;
const int vpc_min_high = 16;
const int vpc_default_stride = 128;
const int vpc_default_high_stride = 16;
const int vpc_default_H_max = 0x1fff;
const int vpc_default_H_min = 0;
const int vpc_default_V_max = 0x1fff;
const int vpc_default_V_min = 0;
const int vpc_rdma_channel = 0;
const int vpc_cvdr_channel = 1;
const int collage_num = 4;
const double vpc_min_hinc = 0.03125;
const double vpc_min_vinc = 0.03125;
const double vpc_max_hinc = 4.0;
const double vpc_max_vinc = 4.0;
const double vpc_yuv_scale_max_inc = 4.0;
const double vpc_yuv_scale_min_inc =0.25;

// old vpc interface input param
struct resize_param_in_msg {
    int src_width; // Width of the original image (128-aligned width, 1080, 128-aligned 1152, 1152 as width)
    int src_high; // Height of the original image (width after 16 alignment, 1080, 128-aligned 1088, and 1088 as width)
    int hmax; // Odd number
    int hmin; // Even number
    int vmax; // Odd number
    int vmin; // Even number
    int dest_width; // 2: Alignment
    int dest_high; // 2: Alignment
};

// old vpc interface output param
struct resize_param_out_msg {
    int dest_width_stride; // Output image width alignment value
    int dest_high_stride; // The height alignment of the output image
    double hinc; // Horizontal scaling coefficient
    double vinc; // Vertical scaling coefficient
    int hmax; // Odd number
    int hmin; // Even number
    int vmax; // Odd number
    int vmin; // Even number
};

// pngd input param
struct PngInputInfoAPI {
    void* inputData; // input data image
    unsigned long inputSize; // buf size for verifying input data
    void* address; // Memory address
    unsigned long size; // Memory size
    int transformFlag; // Conversion flag, 1 means RGBA to RGB. 0 means keep the original format.

    PngInputInfoAPI() :
        inputData(nullptr),
        inputSize(0),
        address(nullptr),
        size(0),
        transformFlag(0) {}
};

// pngd output param
struct PngOutputInfoAPI {
    void* outputData; //output data image.
    unsigned long outputSize; // output data size.
    void* address; // Memory address.
    unsigned long size; // Memory size.
    int format; // Output format, 2 means rgb output, 6 means rgba output.
    unsigned int width; // output width.
    unsigned int high; // output high.
    unsigned int widthAlign; // Width alignment.
    unsigned int highAlign; // High alignment.

    PngOutputInfoAPI() :
        outputData(nullptr),
        outputSize(0),
        address(nullptr),
        size(0),
        format(0),
        width(0),
        high(0),
        widthAlign(0),
        highAlign(0) {}
};

// vdec param
const int vdec_default_width = 16;
const int vdec_default_high = 16;
const int vdec_default_frame = 30;
#define PAGE_SIZE          (4096)
#define MAP_2M             (2*1024*1024)

#ifndef ALIGN_UP
#define ALIGN_UP(x, align) ((((x) + ((align)-1))/(align))*(align))
#endif

typedef enum {
    JPEG_ERROR               = -1,
    JPEG_SUCCESS             = 0,
    JPEG_ERROR_INVALID_PARAM = 1,
    JPEG_ERROR_HARDWARE      = 2,
    JPEG_ERROR_RUNTIME       = 3
} JpegState;


// jpege supports the following format input
typedef enum {
    JPGENC_FORMAT_UYVY = 0x0,
    JPGENC_FORMAT_VYUY = 0x1,
    JPGENC_FORMAT_YVYU = 0x2,
    JPGENC_FORMAT_YUYV = 0x3,
    JPGENC_FORMAT_NV12 = 0x10,
    JPGENC_FORMAT_NV21 = 0x11
} eEncodeFormat;

// jpege input yuv format
typedef enum {    
    JPGENC_FORMAT_YUV422 = 0x0,
    JPGENC_FORMAT_YUV420 = 0x10,
    JPGENC_FORMAT_BIT    = 0xF0
} eFormat;

// The application and release of the memory for hard decoding and soft decoding are inconsistent. 
// The function object is encapsulated to mask the release details of the output memory. 
// In addition, the function needs to be bound. 
// The binary function and the one-meta-function are processed as non-parametric functions, 
// this function is provided for the invoker to invoke as a callback function.
// usage: outBuf.cbFree = JpegCalBackFree( .. )
//        outBuf.cbFree()
class JpegCalBackFree {
public:
    JpegCalBackFree() : isMunmapUse_(false), addr4Free_(nullptr), siz4Free_(0) {}
    ~JpegCalBackFree() {}

    JpegCalBackFree(JpegCalBackFree& others);

    void setBuf(void* addr4Free);
    void setBuf(void* addr4Free, size_t siz4Free);

    void operator() ();
    const JpegCalBackFree& operator= (JpegCalBackFree& others);

private:
    bool isMunmapUse_;
    void* addr4Free_;
    size_t siz4Free_;
};


// jpeg encoder input data
typedef struct _sJpegeIn {
    eEncodeFormat format; // input YUV data format, which supports YUV422 packed and YUV420 Semi-planar.
    unsigned char* buf; // alloced with mmap with huge memory table, and aligned to 128.
    unsigned int bufSize; // ALIGN_UP( stride*height, 4096) for yuv422, ALIGN_UP( stride*height*3/2, 4096) for yuv420.
    unsigned int width; // input image width.
    unsigned int height; // input image height.
    unsigned int stride; // input image width.ALIGN_UP( width*2, 16) for yuv422, ALIGN_UP( width, 16) for yuv420.
    unsigned int heightAligned; // the height of the image after alignment.
    unsigned int level; // the qulitity of the jpg, 0~100, the larger the clearer img.
    _sJpegeIn()
    : format(JPGENC_FORMAT_UYVY), buf( nullptr ), bufSize(0), 
    width(0), height(0), stride(0), heightAligned(0), level(100) {}
} sJpegeIn;


// jpeg encoder output data.
typedef struct _sJpegeOut {
    unsigned char* jpgData; // The starting address of the jpg data in the output buffer.
    unsigned int   jpgSize; // Encoded jpg image data size.
    JpegCalBackFree cbFree; // callback function to free the output memory, when encode fail, no need to free.
    _sJpegeOut() : jpgData(nullptr), jpgSize(0), cbFree() {}
} sJpegeOut;


//jpegd param
enum jpegd_raw_format {
    DVPP_JPEG_DECODE_RAW_YUV_UNSUPPORT = -1,
    DVPP_JPEG_DECODE_RAW_YUV444 = 0,
    DVPP_JPEG_DECODE_RAW_YUV422_H2V1 = 1, // YUV422
    DVPP_JPEG_DECODE_RAW_YUV422_H1V2 = 2, // YUV440
    DVPP_JPEG_DECODE_RAW_YUV420 = 3,
    DVPP_JPEG_DECODE_RAW_YUV400 = 4,
    DVPP_JPEG_DECODE_RAW_MAX
};

enum jpegd_color_space{
    DVPP_JPEG_DECODE_OUT_UNKNOWN = -1,
    DVPP_JPEG_DECODE_OUT_YUV444 = 0, // Y/Cb/Cr (also known as YUV)
    DVPP_JPEG_DECODE_OUT_YUV422_H2V1 = 1, // YUV422
    DVPP_JPEG_DECODE_OUT_YUV422_H1V2 = 2, // YUV440
    DVPP_JPEG_DECODE_OUT_YUV420 = 3,
    DVPP_JPEG_DECODE_OUT_YUV400 = 4,
    DVPP_JPEG_DECODE_OUT_FORMAT_MAX
};

const uint32_t JPEGD_IN_BUFFER_SUFFIX = 8;

// jpegd in struct
struct JpegdIn {
    unsigned char* jpegData; // the input jpg data.
    uint32_t jpegDataSize; // the length of the input buf.
    bool isYUV420Need; // isYUV420Need == true, output yuv420 data, otherwize, raw format.
    bool isVBeforeU; // currently, only support V before U, reserved.
    JpegdIn() : 
        jpegData(nullptr), 
        jpegDataSize(0),
        isYUV420Need(true), 
        isVBeforeU(true) {}
};

// jpegd out struct
struct JpegdOut {
    // Output yuv image data buf, the width and height of the image are the width and height after alignment.
    unsigned char* yuvData;
    uint32_t yuvDataSize;
    uint32_t imgWidth;
    uint32_t imgHeight;
    uint32_t imgWidthAligned; // Aligned width of the output image, align imgWidth up to 128.
    uint32_t imgHeightAligned; // Aligned height of the output image, align imgHeight up to 16.
    JpegCalBackFree cbFree; // Release the callback function of the output buf.
    enum     jpegd_color_space outFormat; // Output yuv data format.
    JpegdOut() : 
        yuvData(nullptr), 
        yuvDataSize(0), 
        imgWidth(0),
        imgHeight(0), 
        imgWidthAligned(0), 
        imgHeightAligned(0), 
        cbFree(),
        outFormat( DVPP_JPEG_DECODE_OUT_UNKNOWN ) {}
};

// jpegd input data
struct jpegd_raw_data_info {
    unsigned char* jpeg_data;
    uint32_t jpeg_data_size; // 8 byte larger than jpeg file length, start address aligned to 128.
    enum jpegd_raw_format in_format; // The sampling format of yuv in the image.
    bool IsYUV420Need; // IsYUV420Need == true, user need yuv420 semi-planar output data, otherwize, do not need.
    bool isVBeforeU; // Currently, only support V before U, reserved.
    jpegd_raw_data_info(): jpeg_data(nullptr), jpeg_data_size(0), \
        in_format( DVPP_JPEG_DECODE_RAW_YUV_UNSUPPORT ), IsYUV420Need(true), isVBeforeU(true)
    {}
};

// jpegd output struct
struct jpegd_yuv_data_info {
    // Output buf of YUV picture data, the width and height of the picture are the width and height after alignment.
    unsigned char* yuv_data;
    uint32_t yuv_data_size; // The output data length is calculated from the width and height of the alignment.
    uint32_t img_width;
    uint32_t img_height;
    uint32_t img_width_aligned; // The width of the output image after aligned to 128.
    uint32_t img_height_aligned; // The height of the output image after aligned to 16.
    JpegCalBackFree cbFree; // Release the callback function of the output buf.
    enum     jpegd_color_space out_format; // Output yuv data format.
    jpegd_yuv_data_info(): yuv_data(nullptr), yuv_data_size(0), img_width(0), \
        img_height(0), img_width_aligned(0), img_height_aligned(0), cbFree(), \
        out_format( DVPP_JPEG_DECODE_OUT_UNKNOWN )
    { }
};


// venc param
const int venc_default_width = 128;
const int venc_default_high = 128;
const int venc_default_coding_type = 0;
const int venc_default_YUV_store_type = 0;

// common struct
struct dvppapi_ctl_msg {
    int   in_size; // Input Parameter Size
    int   out_size; // Output Parameter Size
    void *in; // Input Parameter
    void *out; // Output Parameter
    dvppapi_ctl_msg()
    {
        in_size = -1;
        out_size = -1;
        in = nullptr;
        out = nullptr;
    }
};

typedef struct dvppTask {
    dvppapi_ctl_msg * msg;
    int thread_id;
    int state;
    int task_id;
    int stop_flag;
    std::vector<dvppTask> sub_task;
    bool has_parent;
    int stop_count;
    int parent_task_id;
    dvppTask * parent_task;
    dvppTask()
    {
        msg       = nullptr;
        thread_id = 0;
        state     = 0;
        task_id   = 0;
        stop_flag = -1;
        has_parent =false;
        stop_count = 0;
        parent_task_id = 0;
    }
} DvppTask;
//common enmu


//common class
class AutoBuffer {
public:
    AutoBuffer();
    ~AutoBuffer();

    void Reset();

    char* allocBuffer(int size);

    char* getBuffer() const { return dest_buffer; }
    // 128-aligned width, 16-aligned height. For example, image is 224*220, getBufferSize=256*224.
    int getBufferSize() { return real_bufferSize; }

    char* dest_buffer;

private:
    char* src_buffer;
    int real_bufferSize; // 128-aligned width, 16-aligned height.
    AutoBuffer(const AutoBuffer&); // no copyable to avoid double free.
    const AutoBuffer& operator= (const AutoBuffer&); // no copyable.
};

//vpc param
struct RDMACHANNEL {
    long luma_head_addr; // the header address of the input Y component.
    long chroma_head_addr; // the header address of the input UV component.
    unsigned int luma_head_stride; // the header stride of the input Y component.
    unsigned int chroma_head_stride; // the header stride of the input UV component.
    long luma_payload_addr; // the header address of the input Y component.
    long chroma_payload_addr; // the header address of the input UV component.
    unsigned int luma_payload_stride; // the header stride of the input Y component data.
    unsigned int chroma_payload_stride; // the header stride of the input UV component data.
    RDMACHANNEL()
    {
        luma_head_addr = 0;
        chroma_head_addr = 0;
        luma_head_stride = vpc_default_stride;
        chroma_head_stride = vpc_default_stride;
        luma_payload_addr = 0;
        chroma_payload_addr = 0;
        luma_payload_stride = vpc_default_stride;
        chroma_payload_stride = vpc_default_stride;
    }
};

struct VpcTurningReverse {
    unsigned int reverse1; // Reserved interface 1.
    unsigned int reverse2; // Reserved interface 2.
    unsigned int reverse3; // Reserved interface 3.
    unsigned int reverse4; // Reserved interface 4.
    VpcTurningReverse()
    {
        reverse1 = 0;
        reverse2 = 0;
        reverse3 = 0;
        reverse4 = 0;
    }
};

// Image overlay parameter structure
struct VpcOverLayReverse {
    int reverse1;
    int reverse2;
    int reverse3;
    int reverse4;
    int reverse5;
    int reverse6;
    int reverse7;
    int reverse8;
    int reverse9;
    int reverse10;
    int reverse11;
    char *reverse12;
    char *reverse13;
    shared_ptr<AutoBuffer> reverse14;
    VpcOverLayReverse()
    {
        reverse1 = 0;
        reverse2 = 0;
        reverse3 = 0;
        reverse4 = 0;
        reverse5 = 0;
        reverse6 = 0;
        reverse7 = 0;
        reverse8 = 0;
        reverse9 = 0;
        reverse10 = 0;
        reverse11 = 0;
        reverse12 = nullptr;
        reverse13 = nullptr;
        reverse14 = nullptr;
    }
};

// Parameters of the image stitching structure
struct VpcCollageReverse {
    int reverse1;
    int reverse2;
    int reverse3;
    int reverse4;
    int reverse5;
    int reverse6;
    int reverse7;
    int reverse8;
    char* reverse9[collage_num];
    shared_ptr<AutoBuffer> reverse10;
    VpcCollageReverse()
    {
        reverse1 = 0;
        reverse2 = 0;
        reverse3 = 0;
        reverse4 = 0;
        reverse5 = 0;
        reverse6 = 0;
        reverse7 = 0;
        reverse8 = 0;
        for(int i=0; i<collage_num; i++) {
            reverse9[i] = nullptr;
        }
        reverse10 = nullptr;
    }
};


struct vpc_in_msg
{
    int format; // image data format.
    int rank; // Image arrangement.
    int bitwidth; // Bit depth.
    int cvdr_or_rdma; // Whether the input image is transmitted through the CVDR channel or RDMA, the default is RDMA.
    int width; // original input image width.
    int high; // original input image height.
    int out_width; // Output image width.
    int out_high; // Output image height.
    int out_jpeg_width;
    int out_jpeg_high;
    int stride;
    int h_stride;
    double hinc; // Horizontal magnification, aicore output channel configuration.
    double vinc; // Vertical magnification, aicore output channel configuration.
    double jpeg_hinc; // Horizontal magnification, jpege output channel configuration.
    double jpeg_vinc; // Vertical magnification, jpege output channel configuration.
    int is_scale_up_only;
    int pixel_format;
    int hmax; // Maximum offset from the origin in the horizontal direction.
    int hmin; // Minimum offset from the origin in the horizontal direction.
    int vmax; // Maximum offset from the origin in the vertical direction.
    int vmin; // Minimum offset from the origin in the vertical direction.
    shared_ptr<AutoBuffer> auto_out_buffer_1; // Use the VPC basic function need to configure this buffer.
    shared_ptr<AutoBuffer> auto_out_buffer_2; // reserved interface.
    char * in_buffer;
    char * out_buffer;
    int in_buffer_size;
    int out_buffer_1_size; // This value is used to perform output buffer check.
    int out_buffer_2_size; // reserved interface.
    int use_flag; // basic functions of the VPC, 0:vpc,1:vpcrawdata8k,2:vpcRawData overlay,3:vpcRawData combination.
    RDMACHANNEL rdma; // The rdma parameter is a HFBC data-specific configuration structure.
    VpcTurningReverse turningReverse1; // Vpc optimized reservation interface.
    bool isVpcUseTurning1; // Whether the flag uses Vpc to optimize the reserved interface.
    VpcTurningReverse turningReverse2; // Vpc optimized reservation interface.
    bool isVpcUseTurning2; // Whether the flag uses Vpc to optimize the reserved interface.

    // If the VPC is used for image overlay, scaling, and stitching, three member variables need to be configured.
    // use_flag value is 2.overlay,collage
    VpcOverLayReverse overlay;
    VpcCollageReverse collage;
    long out_y_buffer;
    long out_uv_buffer;
    int yuvscaler_out_width;
    int yuvscaler_out_high;
    long ihinc;
    long ivinc;
    long ihleft;
    long ihright;
    long ivtop;
    long ivbottom;
    string* yuvscaler_paraset; // Default parameter set path + name
    unsigned int yuvscaler_paraset_size;
    unsigned int index;

    vpc_in_msg() {
        format = 0;
        rank = 0;
        bitwidth = vpc_default_bit_width;
        cvdr_or_rdma = vpc_cvdr_channel;
        width = vpc_default_width;
        high = vpc_default_high;
        out_width = vpc_default_width;
        out_high = vpc_default_high;
        out_jpeg_width = vpc_default_width;
        out_jpeg_high = vpc_default_high;
        stride = vpc_default_stride;
        h_stride =vpc_default_high_stride;
        hinc = 1.0;
        vinc = 1.0;
        jpeg_hinc = 1.0;
        jpeg_vinc = 1.0;
        is_scale_up_only = 0;
        pixel_format = 0;
        hmax = vpc_default_H_max; // Maximum offset from the origin in the horizontal direction
        hmin = vpc_default_H_min; // Minimum offset from the origin in the horizontal direction
        vmax = vpc_default_V_max; // Maximum offset from the origin in the vertical direction
        vmin = vpc_default_V_min; // Minimum offset from the origin in the vertical direction
        in_buffer = nullptr;
        out_buffer = nullptr;
        in_buffer_size = 0;
        out_buffer_1_size = 0;
        out_buffer_2_size = 0;
        use_flag = 0;
        auto_out_buffer_1 = nullptr;
        auto_out_buffer_2 = nullptr;
        isVpcUseTurning1 = false;
        isVpcUseTurning2 = false;
        out_y_buffer = 0;
        out_uv_buffer = 0;
        yuvscaler_out_width = -1;
        yuvscaler_out_high = -1;
        ihinc = -1;
        ivinc = -1;
        ihleft = -1;
        ihright = -1;
        ivtop = -1;
        ivbottom = -1;
        yuvscaler_paraset = nullptr; // Default parameter set path + name
        yuvscaler_paraset_size = 1;
        index = 0; // Parameter set index
    }
};

// cmdlist struct start:
union CMDLIST_IN_BUFFER {
    char* bare_buffer;
    RDMACHANNEL* hfbc_buffer;
};

struct CROP_CONFIG {
    int enable; // if need crop.set enable=1,else enable=0;
    unsigned int hmin; // horizontal min shift
    unsigned int hmax; // horizontal max shift
    unsigned int vmin; // vertical min shift
    unsigned int vmax; // vertical max shift
};

// this out channel can export yuv sum.
struct YUV_SUM_OUT_CONFIG {
    int enable; // if need export this channel.set enable=1,else enable=0;
    unsigned int out_width; // output image width
    unsigned int out_height; // output image height
    char* out_buffer; // Output image memory address pointer.
    unsigned int yuv_sum; // Output image sum of all yuv values.
};

// this out channel only export image.image_size=align(width,128)*height*1.5
struct NORMAL_OUT_CONFIG {
    int enable; // if need export this channel.set enable=1,else enable=0;
    unsigned int out_width; // output image width
    unsigned int out_height; // output image height
    char* out_buffer; // Output image memory address pointer.
};

struct ROI_CONFIG {
    CROP_CONFIG crop_config; // Crop parameter configuration structure.
    YUV_SUM_OUT_CONFIG sum_out; // first output parameter configuration structure.
    NORMAL_OUT_CONFIG normal_out; // second output parameter configuration structure.(Reserved, not supported)
    ROI_CONFIG* next; // next ROI_CONFIG pointer.
};

struct IMAGE_CONFIG {
    unsigned int format; // image format,for examplt: yuv420,yuv422,rgb...
    unsigned int rank; // image rank format, for example:yuv420 has nv12 and nv21 ...
    unsigned int bitwidth;
    int is_hfbc_image; // 0:bare image,1:hfbc image
    unsigned int in_width; // input image width
    unsigned int in_height; // input image height
    unsigned int width_step; // width stride value
    CMDLIST_IN_BUFFER in_buffer; // input buffer bare buffer or hfbc buffer
    ROI_CONFIG roi_config;
    IMAGE_CONFIG* next;
    unsigned int need_debug; // 0:not need export debug info;1: need to export debug info 
};
// cmdlist struct end.

// vdec input struct
struct vdec_in_msg {
    char video_format[10]; // Input video format,only support "h264" or "h265" default format is "h264".
    char image_format[10]; // Output frame format, the format is "nv21" or "nv12", the default is "nv12".
    void (*call_back)(FRAME* frame, void* hiai_data); // Caller callback function,FRAME is the output structure.
    char* in_buffer; // Input video stream memory, this stream is h264 or h265 bare stream.
    int in_buffer_size; // Input video stream memory size.
    // After decoding, the parameter pointer of the result frame callback function is output,
    // and the pointer points to the specific structure defined by the caller.
    void * hiai_data;
    // The use of the hiai_data_sp smart pointer will mask hiai_data, hiai_data_sp and hiai_data can only be selected.
    std::shared_ptr<HIAI_DATA_SP> hiai_data_sp;
    int channelId; // The ID of the decoding channel corresponding to the input code stream.
    void (*err_report)(VDECERR* vdecErr); // Error reporting callback function for notifying the user of an exception.
    bool isEOS; // Code stream end flag.
    // Supports one-in-one-out mode, currently only supports H264 format stream,
    // 1: use one-in-one-out, 0: do not use one-in-one-out, the default value is 0.
    int isOneInOneOutMode;

    vdec_in_msg() {
        video_format[0] = 'h';
        video_format[1] = '2';
        video_format[2] = '6';
        video_format[3] = '4';
        video_format[4] = '\0';

        image_format[0] = 'n';
        image_format[1] = 'v';
        image_format[2] = '1';
        image_format[3] = '2';
        image_format[4] = '\0';

        in_buffer = nullptr;
        in_buffer_size = 0;
        call_back = nullptr;
        hiai_data = nullptr;
        hiai_data_sp = nullptr;
        channelId = 0;
        err_report = nullptr;
        isEOS = false;
        isOneInOneOutMode = 0;
    }
};

// The input param in old venc interface.
struct venc_in_msg {
    int width;
    int height;
    int coding_type; // Video coding protocol.
    int YUV_store_type; // YUV image storage format.
    char* input_data; // Input image data address.
    int input_data_size;
    shared_ptr<AutoBuffer> output_data_queue; // Output code stream data address.

    venc_in_msg()
    {
        width = venc_default_width;
        height = venc_default_high;
        coding_type = venc_default_coding_type;
        YUV_store_type = venc_default_YUV_store_type;
        input_data = nullptr;
        input_data_size = 0;
        output_data_queue = nullptr;
    }
};

typedef enum {
    VDEC_SUB_PROC_SCD  = 0,
    VDEC_SUB_PROC_VDM
} VDEC_SUB_PROC_TYPE;


struct dvpp_capability_ioctl_msg {
    signed int     chan_num;
    signed int     in_size;
    signed int     out_size;
    void         *in;
    void         *out;
};

// Modified for 64-bit platform
struct compat_ioctl_msg {
    signed int     chan_num;
    signed int     in_size;
    signed int     out_size;
};


struct dvpp_request_msg_stru {
    unsigned int     opt;
    void         *in;
    void         *out;
};

struct  power_control_req_stru {
    unsigned int     module_id;
    unsigned int     enigne_type;
    unsigned int      enigne_id;
    unsigned int      ops;
    unsigned int     state;
};

struct  power_control_res_stru {
    unsigned int     state;
};

struct  device_query_req_stru {
    unsigned int     module_id; // thr module id.
    unsigned int   engine_type; // dvpp engine type.
};

struct  module_feature_stru {
    unsigned int     module_id;
    unsigned int      max_input_chan;
    unsigned int     support_type;
    unsigned char      engine_count[16];
};


struct  device_query_res_stru {
    unsigned int     module_count;
    struct  module_feature_stru module_info[16];
};

// capability capability info
/*
SP:  Semi-Planar
P: Packed
8/10BIT: bit depth
LIN : linear
*/
enum dvpp_color_format {
    // YUV444 in different ordering of YUV Semi-Planar/Packed,8 bit,Linear.
    DVPP_COLOR_YUV444_YUV_P_8BIT_LIN,
    DVPP_COLOR_YUV444_YVU_P_8BIT_LIN,
    DVPP_COLOR_YUV444_UYV_P_8BIT_LIN,
    DVPP_COLOR_YUV444_UVY_P_8BIT_LIN,
    DVPP_COLOR_YUV444_VYU_P_8BIT_LIN,
    DVPP_COLOR_YUV444_VUY_P_8BIT_LIN,

    DVPP_COLOR_YUV444_UV_SP_8BIT_LIN,
    DVPP_COLOR_YUV444_VU_SP_8BIT_LIN,

    /*422*/
    DVPP_COLOR_YUYV422_YUYV_P_8BIT_LIN,
    DVPP_COLOR_YUYV422_YVYU_P_8BIT_LIN,
    DVPP_COLOR_YUYV422_UYVY_P_8BIT_LIN,
    DVPP_COLOR_YUYV422_VYUY_P_8BIT_LIN,


    DVPP_COLOR_YUV422_UV_SP_8BIT_LIN,
    DVPP_COLOR_YUV422_VU_SP_8BIT_LIN,


    DVPP_COLOR_YUV420_SP_8BIT_LIN,
    DVPP_COLOR_YVU420_SP_8BIT_LIN,

    DVPP_COLOR_YUV420_SP_8BIT_HFBC,
    DVPP_COLOR_YVU420_SP_8BIT_HFBC,
    DVPP_COLOR_YUV420_SP_10BIT_HFBC,
    DVPP_COLOR_YVU420_SP_10BIT_HFBC,

    DVPP_COLOR_YUV420_P_8BIT_LIN,

    DVPP_COLOR_YVU400_SP_8BIT,

    DVPP_COLOR_RGB888_RGB_P_8BIT_LIN,
    DVPP_COLOR_RGB888_RBG_P_8BIT_LIN,
    DVPP_COLOR_RGB888_GBR_P_8BIT_LIN,
    DVPP_COLOR_RGB888_GRB_P_8BIT_LIN,
    DVPP_COLOR_RGB888_BRG_P_8BIT_LIN,
    DVPP_COLOR_RGB888_BGR_P_8BIT_LIN,

    DVPP_COLOR_ARGB8888_ARGB_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_ARBG_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_AGBR_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_AGRB_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_ABRG_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_ABGR_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_RAGB_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_RABG_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_RGBA_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_RGAB_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_RBAG_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_RBGA_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_BRGA_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_BRAG_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_BGAR_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_BGRA_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_BARG_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_BAGR_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_GRAG_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_GRBA_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_GABR_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_GARB_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_GBRA_P_8BIT_LIN,
    DVPP_COLOR_ARGB8888_GBAR_P_8BIT_LIN,
    PIC_JPEG,
    PIC_PNG,
    VIO_H265,
    VIO_H264
};

enum dvpp_proto_type {
    dvpp_proto_unsupport = -1,
    dvpp_itu_t81,
    iso_iec_15948_2003,
    h265_main_profile_level_5_1_hightier,
    h265_main_10_profile_level_5_1_hightier,
    h264_main_profile_level_5_1,
    h264_baseline_profile_level_5_1,
    h264_high_profile_level_5_1,
    h264_high_profile_level_4_1,
    h264_main_profile_level_4_1,
    h264_baseline_profile_level_4_1,
    h265_main_profile_level_4_1
};


enum dvpp_compress_type {
    arithmetic_code = 0,
    huffman_code
};

// Maximum number of standard protocols supported
#define DVPP_PROTOCOL_TYPE_MAX   6

// Maximum number of supported input / output formats
#define DVPP_VADIO_FORMAT_MAX   64

#define DVPP_PERFOMANCE_MODE_MAX  4

#define DVPP_PRE_CONTRATION_TYPE_MAX 3   // Indicates the number of pre-reduced fixed proportions.

#define DVPP_DATA_INPUT_SPEC_TYPE_MAX   6

enum dvpp_high_align_type {
    pix_random = 0,
    two_pix_alignment = 2,
    four_pix_alignment = 4,
    eight_pix_alignment = 8,
    sixteen_pix_alignment = 16
};

enum dvpp_align_type {
    align_random = 0,
    two_alignment = 2,
    four_alignment = 4,
    eight_alignment = 8,
    sixteen_alignment = 16,
    thirty_two_alignment = 32,
    sixty_four_alignment = 64
};

/*Indicates whether to support this function. The options are as follows:*/
enum dvpp_support_type {
    no_support = 0, // no support
    do_support // support
};

enum dvpp_spec_input_type {
    input_type_rdma = 0,
    input_type_cvdr = 1
};

/*Definition of the pre-reduction information structure*/
struct dvpp_pre_contraction_stru {
    // ability Available for engine, just for vpc now
    enum dvpp_support_type is_support; // vpc:support, others: no support

    // contraction types
    unsigned int contraction_types; // vpc: 3, others: 0

    // contraction size support
    unsigned int contraction_size[DVPP_PRE_CONTRATION_TYPE_MAX]; // vpc:2/4/8, others: 0

    // horizontal support
    enum dvpp_support_type is_horizontal_support; // vpc:support, others: no support

    // vertical support
    enum dvpp_support_type is_vertical_support; // vpc:support, others: no support
};

/*Definition of the postscaling information structure*/
struct dvpp_pos_scale_stru {
    // ability Available for engine, just for vpc now
    enum dvpp_support_type is_support; // vpc:support, others: no support

    // min scale
    unsigned int min_scale; // vpc:1, others: 1

    // max scale
    unsigned int max_scale; // vpc:4, others: 1

    // horizontal support
    enum dvpp_support_type is_horizontal_support; // vpc:support, others: no support

    // vertical support
    enum dvpp_support_type is_vertical_support; // vpc:support, others: no support
};

// Picture format type unit definition structure
struct dvpp_format_unit_stru {
    // Picture color format YUV420/YUV/RGB
    enum dvpp_color_format color_format;

    // Data compression type HFBC, the default is 0, not compressed
    unsigned int compress_type;

    // Alignment size, in bytes
    unsigned int stride_size;
      
    enum dvpp_high_align_type high_alignment; // high alignment type.

    enum dvpp_high_align_type width_alignment; // width alignment type.

    unsigned int out_mem_alignment; // Output memory alignment parameters.
};

// Picture Performance Unit Definition Structure
struct dvpp_perfomance_unit_stru {
    // High picture
    unsigned int resolution_high;

    // width picture
    unsigned int resolution_width;

    // How many ways to support this format of processing
    unsigned int stream_num;

    // According to the above high-width stream processing frames per second
    unsigned long fps;
};

// Resolution structure
struct dvpp_resolution_stru {
    // High picture
    unsigned int resolution_high;

    // width picture
    unsigned int resolution_width;
};

struct dvpp_vpc_data_spec_stru {
    // type
    unsigned int input_type;

    struct dvpp_resolution_stru min_resolution;

    struct dvpp_resolution_stru max_resolution;

    enum dvpp_align_type high_alignment;

    enum dvpp_align_type width_alignment;
};

// DVPP Engine capability information query structure
struct dvpp_engine_capability_stru {
    int engine_type;
    // The maximum resolution supported by the engine
    struct dvpp_resolution_stru max_resolution;

    // The minimum resolution supported by the engine
    struct dvpp_resolution_stru min_resolution;

    // The number of standard protocols supported by the engine
    unsigned int protocol_num;

    // Engine supports the standard protocol type table, 0xFF is invalid value,
    // does not involve filling as invalid value
    unsigned int protocol_type[DVPP_PROTOCOL_TYPE_MAX];

    // The number of input picture / video formats supported by the engine
    unsigned int input_format_num;

    // Engine support input picture / video format table
    struct dvpp_format_unit_stru engine_input_format_table[DVPP_VADIO_FORMAT_MAX];

    // The number of output picture / video formats supported by the engine
    unsigned int output_format_num;

    // Engine Support Output Picture / Video Format Table
    struct dvpp_format_unit_stru engine_output_format_table[DVPP_VADIO_FORMAT_MAX];

    // How many common features does the engine support
    unsigned int performance_mode_num;

    // Supported performance tables: eg: Need support(16 way 1080P 30FPS/2 way 3840*2160 60FPS/1*1 995,328,000Pixel/Sec)
    struct dvpp_perfomance_unit_stru performance_mode_table[DVPP_PERFOMANCE_MODE_MAX];

    // pre contraction information(for VPC)
    struct dvpp_pre_contraction_stru pre_contraction;

    // pos scale information(for VPC)
    struct dvpp_pos_scale_stru pos_scale;
    // The number of special input description
    unsigned int spec_input_num;
    // special input description
    struct dvpp_vpc_data_spec_stru spec_input[DVPP_DATA_INPUT_SPEC_TYPE_MAX];
};

#endif // DVPP_DVPP_CONFIG_H

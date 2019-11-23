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
//default param
#define VPC_IN_WIDTH_STRIDE (128)
#define VPC_IN_HIGH_STRIDE (16)
#define VPC_OUT_WIDTH_STRIDE (128)
#define VPC_OUT_HIGH_STRIDE (16)
#define SMOKE_TEST_RES_BASE_DIR  "/root/device/dvpp/"
//vpc param
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

struct resize_param_in_msg
{
    int src_width;//原图的宽度（128对齐后的宽度，1080，128对齐后1152，将1152作为width）
    int src_high;//原图的高度（16对齐后的宽度，1080，128对齐后1088，将1088作为width）
    int hmax;//奇数
    int hmin;//偶数
    int vmax;//奇数
    int vmin;//偶数
    int dest_width;//2对齐
    int dest_high;//2对齐
};

struct resize_param_out_msg
{
    int dest_width_stride;
    int dest_high_stride;
    double hinc;//水平缩放系数
    double vinc;//垂直缩放系数
    int hmax;//奇数
    int hmin;//偶数
    int vmax;//奇数
    int vmin;//偶数
};
//pngd param
//input
struct PngInputInfoAPI {
    void* inputData; // 输入数据
    unsigned long inputSize; // 输入大小
    void* address; // 内存地址
    unsigned long size; // 内存大小
    int transformFlag; // 转换标志
};

struct PngOutputInfoAPI {
    void* outputData; // 输出数据
    unsigned long outputSize; // 输出大小
    void* address; // 内存地址
    unsigned long size; // 内存大小
    int format; // 输出格式
    unsigned int width; // 输出宽度
    unsigned int high; // 输出高度
    unsigned int widthAlign; // 宽度对齐
    unsigned int highAlign; // 高度对齐
};

//vdec param
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


//jpege param
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

// 硬解和软解输出内存的申请和释放不一致，封装了 function 对象，
// 来屏蔽输出内存的释放细节，且需要具备 bind 的作用，将二元函
// 数和一元函数处理成无参函数，方便作为回调函数提供给调用方调用；
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

// name                     : sJpegIn
// description              : jpeg encoder input data
// member                   : 
//     unsigned char* buf   : alloced with mmap with huge memory table, and aligned to 128, TODO tool_fun should be provided
//     unsigned int bufsize : ALIGN_UP( stride*height, 4096) for yuv422, ALIGN_UP( stride*height*3/2, 4096) for yuv420
//     unsigned int width   :
//     unsigned int height  : 
//     unsigned int stride  : ALIGN_UP( width*2, 16) for yuv422, ALIGN_UP( width, 16) for yuv420, TODO tool_fun should be provided
//     unsigned int level   : the qulitity of the jpg, 0~100, the lager the clearer img
//
typedef struct _sJpegeIn
{
    eEncodeFormat format;
    unsigned char* buf;
    unsigned int bufSize;
    unsigned int width;
    unsigned int height;
    unsigned int stride;
    unsigned int heightAligned;
    unsigned int level;
    _sJpegeIn()
    : format(JPGENC_FORMAT_UYVY), buf( nullptr ), bufSize(0), 
    width(0), height(0), stride(0), heightAligned(0), level(100) {}
} sJpegeIn;

// name                       : sJpegOut
// description                : jpeg encoder output data
// member                     : 
//     unsigned char* jpgData : the jpg img data, the memory alloced inside the API
//     unsigned int jpgSize   : the jpg img len
//     JpegCalBackFree cbFree : callback function to free the output memory, when encode fail, no need to free
//      
typedef struct _sJpegeOut
{
    unsigned char* jpgData;
    unsigned int   jpgSize;
    JpegCalBackFree cbFree;
    _sJpegeOut() : jpgData(nullptr), jpgSize(0), cbFree() {}
} sJpegeOut;


//jpegd param
enum jpegd_raw_format{
    DVPP_JPEG_DECODE_RAW_YUV_UNSUPPORT = -1,
    DVPP_JPEG_DECODE_RAW_YUV444 = 0,
    DVPP_JPEG_DECODE_RAW_YUV422_H2V1 = 1, /* YUV422 */
    DVPP_JPEG_DECODE_RAW_YUV422_H1V2 = 2, /* YUV440 */
    DVPP_JPEG_DECODE_RAW_YUV420 = 3,
    DVPP_JPEG_DECODE_RAW_YUV400 = 4,
    DVPP_JPEG_DECODE_RAW_MAX
};

enum jpegd_color_space{
    DVPP_JPEG_DECODE_OUT_UNKNOWN = -1,
    DVPP_JPEG_DECODE_OUT_YUV444 = 0, /* Y/Cb/Cr (also known as YUV) */
    DVPP_JPEG_DECODE_OUT_YUV422_H2V1 = 1, /* YUV422 */
    DVPP_JPEG_DECODE_OUT_YUV422_H1V2 = 2, /* YUV440 */
    DVPP_JPEG_DECODE_OUT_YUV420 = 3,
    DVPP_JPEG_DECODE_OUT_YUV400 = 4,
    DVPP_JPEG_DECODE_OUT_FORMAT_MAX
};

const uint32_t JPEGD_IN_BUFFER_SUFFIX = 8;

// jpegd in struct
struct JpegdIn {
    unsigned char* jpegData;
    // 8 byte larger than jpeg file length, start address aligned to 128
    uint32_t jpegDataSize; 
    // isYUV420Need == true, output yuv420 data, otherwize, raw format.
    bool isYUV420Need;
    // currently, only support V before U, reserved
    bool isVBeforeU;
    JpegdIn() : 
        jpegData(nullptr), 
        jpegDataSize(0),
        isYUV420Need(true), 
        isVBeforeU(true) {}
};

// jpegd out struct
struct JpegdOut {
    unsigned char* yuvData;
    uint32_t yuvDataSize;
    uint32_t imgWidth;
    uint32_t imgHeight;
    uint32_t imgWidthAligned;   // align imgWidth up to 128
    uint32_t imgHeightAligned;  // align imgHeight up to 16
    JpegCalBackFree cbFree;
    enum     jpegd_color_space outFormat;
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

// jpegd in struct
struct jpegd_raw_data_info{
    unsigned char* jpeg_data;
    // 8 byte larger than jpeg file length, start address aligned to 128
    uint32_t jpeg_data_size; 
    // the caller don't need to fill this segment
    enum jpegd_raw_format in_format; 
    bool IsYUV420Need;
    // currently, only support V before U, reserved
    bool isVBeforeU;  
    jpegd_raw_data_info(): jpeg_data(nullptr), jpeg_data_size(0), \
        in_format( DVPP_JPEG_DECODE_RAW_YUV_UNSUPPORT ), IsYUV420Need(true), isVBeforeU(true)
    {}
};

// jpegd out struct
struct jpegd_yuv_data_info{
    unsigned char* yuv_data;
    uint32_t yuv_data_size;
    uint32_t img_width;
    uint32_t img_height;
    uint32_t img_width_aligned;   // align img_width up to 128
    uint32_t img_height_aligned;  // align img_height up to 16
    JpegCalBackFree cbFree;
    enum     jpegd_color_space out_format;
    jpegd_yuv_data_info(): yuv_data(nullptr), yuv_data_size(0), img_width(0), \
        img_height(0), img_width_aligned(0), img_height_aligned(0), cbFree(), \
        out_format( DVPP_JPEG_DECODE_OUT_UNKNOWN )
    { }
};


//venc param
const int venc_default_width = 128;
const int venc_default_high = 128;
const int venc_default_coding_type = 0;
const int venc_default_YUV_store_type = 0;

//common struct
struct dvppapi_ctl_msg
{
    int   in_size;      //入参大小
    int   out_size;     //出参大小
    void *in;           //入参
    void *out;          //出参
    dvppapi_ctl_msg()
    {
        in_size = -1;
        out_size = -1;
        in = nullptr;
        out = nullptr;
    }
};
typedef struct dvppTask
{
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
    int getBufferSize() { return real_bufferSize; }

    char* dest_buffer;

private:
    char* src_buffer;
    int real_bufferSize;
    AutoBuffer(const AutoBuffer&); //no copyable to avoid double free.
    const AutoBuffer& operator= (const AutoBuffer&); //no copyable
};

//vpc param
struct RDMACHANNEL{
    long luma_head_addr;    /*输入Y分量的头地址*/
    long chroma_head_addr;  /*输入UV分量的头地址*/
    unsigned int luma_head_stride;
    unsigned int chroma_head_stride;
    long luma_payload_addr;  /*输入Y分量的地址*/
    long chroma_payload_addr;/*输入UV分量的地址*/
    unsigned int luma_payload_stride;
    unsigned int chroma_payload_stride;
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
struct VpcTurningReverse{
    unsigned int reverse1;//预留接口1
    unsigned int reverse2;//预留接口2
    unsigned int reverse3;//预留接口3
    unsigned int reverse4;//预留接口4
    VpcTurningReverse()
    {
        reverse1 = 0;
        reverse2 = 0;
        reverse3 = 0;
        reverse4 = 0;
    }
};
//图像叠加参数结构体
struct rawdata_overlay_config
{
    int image_type; // 图像类型
    int image_rank_type; // 图像格式
    int bit_width; // 位宽
    int in_width_image; // 输入图片宽度，2对齐，128~4096
    int in_high_image; // 输入图片高度，2对齐，128~4096
    int in_width_text; // 输入文字图片宽度，2对齐
    int in_high_text; // 输入文字图片高度，2对齐
    int image_width_stride;
    int image_high_stride;
    int text_width_stride;
    int text_high_stride;
    char *in_buffer_image; //输入图片的buffer
    char *in_buffer_text; //输入文字图片的buffer
    shared_ptr<AutoBuffer> auto_overlay_out_buffer; //叠加后图片的输出地址
    rawdata_overlay_config()
    {
        image_type = 0; //yuv420_semi_plannar
        image_rank_type = 0; //NV12
        bit_width = vpc_default_bit_width;
        in_width_image = vpc_default_width;
        in_high_image = vpc_default_high;
        in_width_text = vpc_default_width;
        in_high_text = vpc_default_high;
        image_width_stride = vpc_default_stride;
        image_high_stride = vpc_default_high_stride;
        text_width_stride = vpc_default_stride;
        text_high_stride = vpc_default_high_stride;
        in_buffer_image = nullptr;
        in_buffer_text = nullptr;
        auto_overlay_out_buffer = nullptr;
    }
};
//图像拼接结构体参数
struct rawdata_collage_config
{
    int image_type; // 图像类型
    int image_rank_type; // 图像格式
    int bit_width; // 位宽
    int in_width; // 输入图片宽度，来源于VPC，2对齐，16~4096
    int in_high; // 输入图片高度，来源于VPC，2对齐，16~4096
    int width_stride;
    int high_stride;
    int collage_type;//拼接图片格式，1x4，4x1，2x2
    char* in_buffer[collage_num]; // 拼接图片的输入地址
    shared_ptr<AutoBuffer> auto_out_buffer; // 拼接图片的输出地址
    rawdata_collage_config()
    {
        image_type = 0; //yuv420_semi_plannar
        image_rank_type = 0; //NV12
        bit_width = vpc_default_bit_width;
        in_width = vpc_default_width;
        in_high = vpc_default_high;
        width_stride = vpc_default_stride;
        high_stride = vpc_default_high_stride;
        collage_type = 0; //COLLAGE_2x2 两行两列
        for(int i=0; i<collage_num; i++) {
            in_buffer[i] = nullptr;
        }
        auto_out_buffer = nullptr;
    }
};

struct vpc_in_msg
{
    int format;
    int rank;
    int bitwidth;
    int cvdr_or_rdma;
    int width;
    int high;
    int out_width;
    int out_high;
    int out_jpeg_width;
    int out_jpeg_high;
    int stride;
    int h_stride;
    double hinc;
    double vinc;
    double jpeg_hinc;
    double jpeg_vinc;
    int is_scale_up_only;
    int pixel_format;
    int hmax; //与原点在水平方向的最大偏移
    int hmin; //与原点在水平方向的最小偏移
    int vmax; //与原点在垂直方向的最大偏移
    int vmin; //与原点在垂直方向的最小偏移
    shared_ptr<AutoBuffer> auto_out_buffer_1;
    shared_ptr<AutoBuffer> auto_out_buffer_2;
    char * in_buffer;
    char * out_buffer;
    int in_buffer_size;
    int out_buffer_1_size;
    int out_buffer_2_size;
    int use_flag; // 默认是使用vpc基础功能,0:vpc,1:vpcrawdata8k,2:vpcRawData叠加,3:vpcRawData拼接
    RDMACHANNEL rdma;
    VpcTurningReverse turningReverse1;
    bool isVpcUseTurning1;
    VpcTurningReverse turningReverse2;
    bool isVpcUseTurning2;

    // 如果使用VPC进行图片叠加、缩放、拼接功能时，一共需要配置3个成员变量
    // use_flag值为2、overlay、collage
    rawdata_overlay_config overlay;
    rawdata_collage_config collage;
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
    string* yuvscaler_paraset; // 参数集路径+名称
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
        hmax = vpc_default_H_max; // 与原点在水平方向的最大偏移
        hmin = vpc_default_H_min; // 与原点在水平方向的最小偏移
        vmax = vpc_default_V_max; // 与原点在垂直方向的最大偏移
        vmin = vpc_default_V_min; // 与原点在垂直方向的最小偏移
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
        yuvscaler_paraset = nullptr; // 默认参数集路径+名称
        yuvscaler_paraset_size = 1;
        index = 0; // 参数集索引
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
    char* out_buffer;
    unsigned int yuv_sum;
};

// this out channel only export image.image_size=align(width,128)*height*1.5
struct NORMAL_OUT_CONFIG {
    int enable; // if need export this channel.set enable=1,else enable=0;
    unsigned int out_width; // output image width
    unsigned int out_height; // output image height
    char* out_buffer;
};

struct ROI_CONFIG {
    CROP_CONFIG crop_config;
    YUV_SUM_OUT_CONFIG sum_out;
    NORMAL_OUT_CONFIG normal_out;
    ROI_CONFIG* next;
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

struct vdec_in_msg {
    char video_format[10];
    char image_format[10];
    void (*call_back)(FRAME* frame, void* hiai_data);
    char* in_buffer;
    int in_buffer_size;
    void * hiai_data;
    std::shared_ptr<HIAI_DATA_SP> hiai_data_sp;
    int channelId;
    void (*err_report)(VDECERR* vdecErr);
    bool isEOS;

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
    }
};

struct venc_in_msg
{
    int width;
    int height;
    int coding_type;
    int YUV_store_type;
    char* input_data;
    int input_data_size;
    shared_ptr<AutoBuffer> output_data_queue;

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
//capability interface type
#define  DVPP_TYPE_DEF_VDEC             (0)
#define  DVPP_TYPE_DEF_JPEGD            (1)
#define  DVPP_TYPE_DEF_PNGD             (2)
#define  DVPP_TYPE_DEF_JPEGE            (3)
#define  DVPP_TYPE_DEF_VPC              (4)
#define  DVPP_TYPE_DEF_VENC             (5)
typedef enum {
    VDEC_SUB_PROC_SCD  = 0,
    VDEC_SUB_PROC_VDM
}VDEC_SUB_PROC_TYPE;


struct dvpp_capability_ioctl_msg {
    signed int     chan_num;
    signed int     in_size;
    signed int     out_size;
    void         *in;
    void         *out;
};

//Modified for 64-bit platform
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

#define DVPP_POWER_CTL_GET_STATE    (0)
#define DVPP_POWER_CTL_POWERON        (1)
#define DVPP_POWER_CTL_POWERDOWN    (2)



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
    unsigned int     module_id;
    unsigned int   engine_type;
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
//capability capability info
/*
SP:  Semi-Planar
P: Packed
8/10BIT: bit depth
LIN : linear
*/
enum dvpp_color_format {
      //YUV444 in different ordering of YUV Semi-Planar/Packed，8 bit，Linear。

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

//Maximum number of standard protocols supported
#define DVPP_PROTOCOL_TYPE_MAX   6

//Maximum number of supported input / output formats
#define DVPP_VADIO_FORMAT_MAX   64

#define DVPP_PERFOMANCE_MODE_MAX  4

#define DVPP_PRE_CONTRATION_TYPE_MAX 3   //预缩小固定比例的个数

#define VDEC_OUT_MEM_ALIGNMENT    1024

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

/*是否支持该功能，类型定义*/
enum dvpp_support_type {
       no_support = 0,        //no support
      do_support            //support
};

enum dvpp_spec_input_type {
    input_type_rdma = 0,
    input_type_cvdr = 1
};

/*预缩小信息结构体定义*/
struct dvpp_pre_contraction_stru {
    //ability Available for engine, just for vpc now
    enum dvpp_support_type is_support;    //vpc:support, others: no support

    //contraction types
    unsigned int contraction_types;    //vpc: 3, others: 0

    //contraction size support
    unsigned int contraction_size[DVPP_PRE_CONTRATION_TYPE_MAX];    //vpc:2/4/8, others: 0

    //horizontal support
    enum dvpp_support_type is_horizontal_support;    //vpc:support, others: no support

    //vertical support
    enum dvpp_support_type is_vertical_support;    //vpc:support, others: no support
};

/*后缩放信息结构体定义*/
struct dvpp_pos_scale_stru {
    //ability Available for engine, just for vpc now
    enum dvpp_support_type is_support;     //vpc:support, others: no support

    //min scale
    unsigned int min_scale;    //vpc:1, others: 1

    //max scale
    unsigned int max_scale;    //vpc:4, others: 1

    //horizontal support
    enum dvpp_support_type is_horizontal_support;    //vpc:support, others: no support

    //vertical support
    enum dvpp_support_type is_vertical_support;    //vpc:support, others: no support 
};

//Picture format type unit definition structure
struct dvpp_format_unit_stru
{
      //Picture color format YUV420/YUV/RGB
      enum dvpp_color_format color_format;

      //Data compression type HFBC, the default is 0, not compressed
      unsigned int compress_type;

      //Alignment size, in bytes
      unsigned int stride_size;
      
      enum dvpp_high_align_type high_alignment;

      enum dvpp_high_align_type width_alignment;

      unsigned int out_mem_alignment;

};

//Picture Performance Unit Definition Structure
struct dvpp_perfomance_unit_stru
{
    //High picture
    unsigned int resolution_high;

    //width picture
    unsigned int resolution_width;

    //How many ways to support this format of processing
    unsigned int stream_num;

    //According to the above high-width stream processing frames per second
    unsigned long fps;
};

//Resolution structure
struct dvpp_resolution_stru
{
    //High picture
    unsigned int resolution_high;

    //width picture
    unsigned int resolution_width;
};

struct dvpp_vpc_data_spec_stru
{
    //type
    unsigned int input_type;

    struct dvpp_resolution_stru min_resolution;

    struct dvpp_resolution_stru max_resolution;

    enum dvpp_align_type high_alignment;

    enum dvpp_align_type width_alignment;


};
//DVPP Engine capability information query structure
struct dvpp_engine_capability_stru
{
    int engine_type;
    //The maximum resolution supported by the engine
    struct dvpp_resolution_stru max_resolution;

    //The minimum resolution supported by the engine
    struct dvpp_resolution_stru min_resolution;

     //The number of standard protocols supported by the engine
    unsigned int protocol_num;

    //Engine supports the standard protocol type table, 0xFF is invalid value, does not involve filling as invalid value
    unsigned int protocol_type[DVPP_PROTOCOL_TYPE_MAX];

    //The number of input picture / video formats supported by the engine
    unsigned int input_format_num;

    //Engine support input picture / video format table
    struct dvpp_format_unit_stru engine_input_format_table[DVPP_VADIO_FORMAT_MAX];

    //The number of output picture / video formats supported by the engine
    unsigned int output_format_num;

    //Engine Support Output Picture / Video Format Table
    struct dvpp_format_unit_stru engine_output_format_table[DVPP_VADIO_FORMAT_MAX];

    //How many common features does the engine support
    unsigned int performance_mode_num;

    //Supported performance tables: eg: Need support(16路1080P 30FPS/2路3840*2160 60FPS/1*1 995,328,000Pixel/Sec)
    struct dvpp_perfomance_unit_stru performance_mode_table[DVPP_PERFOMANCE_MODE_MAX];

    //pre contraction information(for VPC) 
    struct dvpp_pre_contraction_stru pre_contraction;

    //pos scale information(for VPC) 
    struct dvpp_pos_scale_stru pos_scale;
     //The number of special input description
    unsigned int spec_input_num;
    //special input description
    struct dvpp_vpc_data_spec_stru spec_input[DVPP_DATA_INPUT_SPEC_TYPE_MAX];
};

#endif // DVPP_DVPP_CONFIG_H

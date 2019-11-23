/* Copyright (C) 2018. Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the Apache License Version 2.0.You may not use this file except in compliance with the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * Apache License for more details at
 * http://www.apache.org/licenses/LICENSE-2.0
 */
#ifndef DOMI_OMG_COMMON_TYPES_H_
#define DOMI_OMG_COMMON_TYPES_H_
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits.h>
#include <linux/limits.h>
#include "common/fmk_error_codes.h"
#include "common/fmk_types.h"
#include "common/op_types.h"

#define CAFFE_BATCH_NORM_ESP_DEFAULT_FLOAT             0.00001
#define CAFFE_BATCH_NORM_ESP_DEFAULT_DIFF_FLOAT        0.000000001
#define CAFFE_BATCH_NORM_MOVING_AVERAGE_FRACTION_FLOAT 0.999
#define YOLO_EPSINON 0.00001
#if !defined( __ANDROID__) && !defined(ANDROID)
#define DOMI_DYNAMIC_CAST dynamic_cast
#define DOMI_DYNAMIC_POINTER_CAST dynamic_pointer_cast
#else
#define DOMI_DYNAMIC_CAST static_cast
#define DOMI_DYNAMIC_POINTER_CAST static_pointer_cast
#endif

namespace domi
{
    /**
     * @brief 支持的公共的Properties名称
     */
    const std::string PROP_OME_START_TIME = "ome_start_time";  /**< 启动时间 */
    const std::string PROP_OME_DUMP_PATH = "ome_dump_path";  /**< dump路径 */
    const std::string PROP_OME_LOG_PATH = "ome_log_path";  /**< 日志路径 */

    // Profile相关常量
    const uint32_t CCE_PROFILE_ON  = 0;
    const uint32_t CCE_PROFILE_OFF = 1;
    const std::string OME_PROFILE = "OME_PROFILE";
    const std::string CCE_PROFILE = "CCE_PROFILE";
    const std::string RTS_PROFILE = "RTS_PROFILE";
    const std::string PROFILER_JOBCTX = "profiler_jobctx";
    const std::string PROFILER_TARGET_PATH = "profiler_target_path";
    const std::string RTS_PROFILE_PATH = "RTS_PATH";
    const std::string PROFILE_STOP_KEY = "stop";
    const std::string PROFILE_STOP_VALUE = "enable";
    static std::map<std::string, std::string> PROFILE_COMPONENT_MAP
    {
        {"ome", OME_PROFILE},
        {"cce", CCE_PROFILE},
        {"runtime", RTS_PROFILE},
    };

    /**
     * @brief task下沉相关数据结构定义
    */
    // Build模式
    enum BuildMode {
        BUILD_NORMAL = 0,             // 正常模式
        GEN_TASK_WITH_FUSION = 3,         // 携带task数据（融合开启）
        GEN_TASK_WITHOUT_FUSION = 4,      // 携带task数据（融合关闭）
        GEN_TASK_WITH_FUSION_L2 = 5       // 携带task数据（融合开启 L2开启）
    };

    const std::string MODEL_ATTR_TASKS = "tasks";
    const std::string MODEL_ATTR_TASK_GEN_BASE_ADDR = "task_gen_base_addr";
    const std::string MODEL_ATTR_TASK_GEN_WEIGHT_ADDR = "task_gen_weight_addr";

    const int MODEL_MAX_SIZE = INT32_MAX; // Max size of 2 GB minus 1 byte.
    const uint64_t FILE_HEADER_MAX_SIZE = 3221225472; // Max size of 3 GB.

#if !defined( __ANDROID__) && !defined(ANDROID)
    const uint64_t ALLOC_MEMORY_MAX_SIZE = 8589934592; // Max size of 8 GB.
#else
    const uint64_t ALLOC_MEMORY_MAX_SIZE = 536870912; // Max size of 512M.
#endif

    template<typename K, typename V>
    static std::pair<V, K> flip_pair(const std::pair<K, V> &p)
    {
        return std::pair<V, K>(p.second, p.first);
    }

    template<typename K, typename V>
    static std::map<V, K> flip_map(std::map<K, V> src)
    {
        std::map<V, K> dst;
        std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
            flip_pair<K, V>);
        return dst;
    }

    REGISTER_OPTYPE(DATA, "Data");
    REGISTER_OPTYPE(CONVOLUTION, "Convolution");
    REGISTER_OPTYPE(CORRELATION, "Correlation");
    REGISTER_OPTYPE(CORRELATIONV2, "Correlation_V2");
    REGISTER_OPTYPE(DECONVOLUTION, "Deconvolution");
    REGISTER_OPTYPE(POOLING, "Pooling");
    REGISTER_OPTYPE(ELTWISE, "Eltwise");
    REGISTER_OPTYPE(RELU, "ReLU");
    REGISTER_OPTYPE(RELU6, "ReLU6");
    REGISTER_OPTYPE(SIGMOID, "Sigmoid");
    REGISTER_OPTYPE(ABSVAL, "AbsVal");
    REGISTER_OPTYPE(TANH, "TanH");
    REGISTER_OPTYPE(PRELU, "PReLU");
    REGISTER_OPTYPE(BATCHNORM, "BatchNorm");
    REGISTER_OPTYPE(FUSIONBATCHNORM, "FusionBatchNorm");
    REGISTER_OPTYPE(SCALE, "Scale");
    REGISTER_OPTYPE(FULL_CONNECTION, "FullConnection");
    REGISTER_OPTYPE(SOFTMAX, "Softmax");
    REGISTER_OPTYPE(PLUS, "Plus");
    REGISTER_OPTYPE(ACTIVATION, "Activation");
    REGISTER_OPTYPE(FLATTEN, "Flatten");
    REGISTER_OPTYPE(ADD, "Add");
    REGISTER_OPTYPE(SUB, "Sub");
    REGISTER_OPTYPE(MUL, "Mul");
    REGISTER_OPTYPE(MATMUL, "MatMul");
    REGISTER_OPTYPE(RSQRT, "Rsqrt");
    REGISTER_OPTYPE(BIASADD, "BiasAdd");
    REGISTER_OPTYPE(RESHAPE, "Reshape");
    REGISTER_OPTYPE(DEPCONVOLUTION, "ConvolutionDepthwise");
    REGISTER_OPTYPE(DROPOUT, "Dropout");
    REGISTER_OPTYPE(CONCAT, "Concat");
    REGISTER_OPTYPE(ROIPOOLING, "ROIPooling");
    REGISTER_OPTYPE(PROPOSAL, "Proposal");
    REGISTER_OPTYPE(FSRDETECTIONOUTPUT, "FSRDetectionOutput");
    REGISTER_OPTYPE(DETECTIONPOSTPROCESS, "Detectpostprocess");
    REGISTER_OPTYPE(LRN, "LRN");
    REGISTER_OPTYPE(TRANSDATA, "TransData");
    REGISTER_OPTYPE(PERMUTE, "Permute");
    REGISTER_OPTYPE(SSDNORMALIZE, "SSDNormalize");
    REGISTER_OPTYPE(SSDPRIORBOX, "SSDPriorBox");
    REGISTER_OPTYPE(NETOUTPUT, "NetOutput");
    REGISTER_OPTYPE(SSDDETECTIONOUTPUT, "SSDDetectionOutput");
    REGISTER_OPTYPE(CHANNELAXPY, "ChannelAxpy");
    REGISTER_OPTYPE(PSROIPOOLING, "PSROIPooling");
    REGISTER_OPTYPE(POWER, "Power");
    REGISTER_OPTYPE(ROIALIGN, "ROIAlign");
    REGISTER_OPTYPE(PYTHON, "Python");
    REGISTER_OPTYPE(FREESPACEEXTRACT, "FreespaceExtract");
    REGISTER_OPTYPE(SPATIALTF, "SpatialTransform");
    REGISTER_OPTYPE(SHAPE, "Shape");
    REGISTER_OPTYPE(ARGMAX, "ArgMax");
    REGISTER_OPTYPE(GATHERND, "GatherNd");
    REGISTER_OPTYPE(GATHER, "Gather");
    REGISTER_OPTYPE(REALDIV, "RealDiv");
    REGISTER_OPTYPE(PACK, "Pack");
    REGISTER_OPTYPE(SLICE, "Slice");
    REGISTER_OPTYPE(FLOORDIV, "FloorDiv");
    REGISTER_OPTYPE(SQUEEZE, "Squeeze");
    REGISTER_OPTYPE(STRIDEDSLICE, "StridedSlice");
    REGISTER_OPTYPE(RANGE, "Range");
    REGISTER_OPTYPE(RPNPROPOSALS, "GenerateRpnProposals");
    REGISTER_OPTYPE(DECODEBBOX, "DecodeBBox");
    REGISTER_OPTYPE(PAD, "Pad");
    REGISTER_OPTYPE(TILE, "Tile");
    REGISTER_OPTYPE(SIZE, "Size");
    REGISTER_OPTYPE(CLIPBOXES, "Clipboxes");
    REGISTER_OPTYPE(FASTRCNNPREDICTIONS, "FastrcnnPredictions");
    REGISTER_OPTYPE(SPLIT, "Split");
    REGISTER_OPTYPE(EXPANDDIMS, "ExpandDims");
    REGISTER_OPTYPE(MEAN, "Mean");
    REGISTER_OPTYPE(GREATER, "Greater");
    REGISTER_OPTYPE(SWITCH, "Switch");
    REGISTER_OPTYPE(MERGE, "Merge");
    REGISTER_OPTYPE(TRANSPOSE, "Transpose");
    REGISTER_OPTYPE(CAST, "Cast");
    REGISTER_OPTYPE(REGION, "Region")
    REGISTER_OPTYPE(YOLO, "Yolo")
    REGISTER_OPTYPE(YOLODETECTIONOUTPUT, "YoloDetectionOutput")
    REGISTER_OPTYPE(FILL, "Fill");
    REGISTER_OPTYPE(REVERSE, "Reverse");
    REGISTER_OPTYPE(UNPACK, "Unpack");
    REGISTER_OPTYPE(YOLO2REORG, "Yolo2Reorg");
    REGISTER_OPTYPE(REDUCESUM, "ReduceSum");
    REGISTER_OPTYPE(CONSTANT, "Const");
    REGISTER_OPTYPE(RESIZEBILINEAR, "ResizeBilinear");
    REGISTER_OPTYPE(MAXIMUM, "Maximum");
    REGISTER_OPTYPE(FRAMEWORKOP, "FrameworkOp");
    REGISTER_OPTYPE(ARG, "_Arg");
    REGISTER_OPTYPE(FUSEDBATCHNORMGRAD, "FusedBatchNormGrad");
    REGISTER_OPTYPE(LSTM, "LSTM");
    REGISTER_OPTYPE(HIGHWAY, "HighWay");
    REGISTER_OPTYPE(RNN, "RNN");
    REGISTER_OPTYPE(ATTENTIONDECODER, "AttentionDecoder");
    REGISTER_OPTYPE(LOGICAL_NOT, "LogicalNot");
    REGISTER_OPTYPE(LOGICAL_AND, "LogicalAnd");
    REGISTER_OPTYPE(EQUAL, "Equal");
    REGISTER_OPTYPE(INTERP, "Interp");
    REGISTER_OPTYPE(SHUFFLECHANNEL, "ShuffleChannel");



    /***************ANN专用算子*************************/
    REGISTER_OPTYPE(ANN_MEAN, "AnnMean");
    REGISTER_OPTYPE(ANN_CONVOLUTION, "AnnConvolution");
    REGISTER_OPTYPE(ANN_DEPCONVOLUTION, "AnnDepthConv");
    REGISTER_OPTYPE(DIV, "Div");
    REGISTER_OPTYPE(ANN_FULLCONNECTION, "AnnFullConnection");
    REGISTER_OPTYPE(ANN_NETOUTPUT, "AnnNetOutput");
    REGISTER_OPTYPE(ANN_DATA,"AnnData");
    /***************************************************/

    //算子重名时，公用的类型
    REGISTER_OPSAMETYPE(DETECTIONOUTPUT, "DetectionOutput");

    enum class ImplyType : unsigned int
    {
        BUILDIN = 0,// 内置算子，由OME正常执行
        TVM,        // 编译成tvm bin文件执行
        CUSTOM,     // 由用户自定义计算逻辑，通过CPU执行
        AI_CPU,      // AICPU 自定义算子类型
        INVALID = 0xFFFFFFFF,
    };

    enum InputMode {
        INPUT = 0,
        CONST
    };

    /**
     * @brief Process模块处理状态enun定义
     */
    enum  ModelProcessState
    {
        INIT_STATE = 0,     //初始状态
        WAIT_EVENT_STATE,   //等待EVENT状态
        IND_RSLT_STATE,     //正在向高层输出Model执行结果状态
        STOPPED_STATE,      //Model执行结束状态，在Model Manager::Stop后进入此状态
        RESERVED_STATE,     //保留
    };

    /**
     * @brief Process模块Model 执行模式enun定义
     */
    enum SysMode
    {
        INFERENCE = 0,      //正常，即Inference 模式
        DEBUG,              //Debug模式
        TIME,               //计算模型执行时间模式，包括各OP执行时间
        STOP,               //STOP模式
        RESET,              //RESET模式
        PERFORMANCE,        //性能模型，当前开启后的影响：1、认为模型输入数据已就绪，无需做格式转换
        ANDROID_DEBUG, //用于Android_平台计算数据导出
        RESERVED,           //保留
    };

    /**
    * @brief 模型文件加密类型
    */
    enum ModelEncryptType
    {
        UNENCRYPTED, // 不加密
        ENCRYPTED    // 加密
    };

    /**
    * @brief 校验签名
    */
    enum ModelCheckType
    {
        CHECK,    // 校验签名
        UNCHECK   // 不校验
    };

    /**
    * @brief 模型文件魔数
    */
    const uint32_t MODEL_FILE_MAGIC_NUM = 0x444F4D49;  /**< 魔数DOMI */

    /**
    * @brief 模型头长度
    */
    const uint32_t MODEL_FILE_HEAD_LEN = 256;

    /**
    * @brief 模型名字长度
    */
    const uint32_t MODEL_NAME_LENGTH = 32;

    /**
    * @brief 用户自定义信息长度
    */
    const uint32_t USER_DEFINE_INFO_LENGTH = 32;

    /**
    * @brief 模型文件签名长度
    */
    const uint32_t MODEL_FILE_CHECKSUM_LENGTH = 64;

    /**
    * @brief 模型文件头保留字段长度
    */
    const uint32_t MODEL_FILE_RESERVED_LENGTH = 104;

    /**
     * @ingroup domi_omg
     * @brief INPUT节点类型
     */
    static const std::string INPUT_TYPE = "Input";

    /**
     * @ingroup domi_omg
     * @brief DATA节点类型
     */
    static const std::string DATA_TYPE = "Data";

    /**
     * @ingroup domi_omg
     * @brief 框架算子类型
     */
    static const std::string FRAMEWORK_OP_TYPE = "FrameworkOp";

      /**
     * @ingroup domi_omg
     * @brief DATA节点类型
     */
    static const std::string ANN_DATA_TYPE = "AnnData";
    static const std::string ANN_NETOUTPUT_TYPE = "AnnNetOutput";
    /**
    * @ingroup domi_omg
    * @brief 卷积节点类型
    */
    static const std::string NODE_NAME_NET_OUTPUT = "Node_Output";

    /**
    * @ingroup domi_omg
    * @brief 卷积节点类型
    */
    static const std::string OP_TYPE_CONVOLUTION = "Convolution";
    /**
    * @ingroup domi_omg
    * @brief 为硬AIPP增加卷积节点名字
    */
    static const std::string AIPP_CONV_OP_NAME = "aipp_conv_op";
    /**
    * @ingroup domi_omg
    * @brief 算子配置项分隔符
    */
    static const std::string OP_CONF_DELIMITER = ":";

    /**
     * @ingroup domi_omg
     * @brief alpha默认值
     */
    static const float ALPHA_DEFAULT_VALUE = 1.0;

    /**
     * @ingroup domi_omg
     * @brief beta默认值
     */
    static const float BETA_DEFAULT_VALUE = 0.0;

    /**
    * @ingroup domi_omg
    * @brief coef默认值
    */
    static const float COEF_DEFAULT_VALUE = 0.0;

    /**
    * @ingroup domi_omg
    * @brief Relu6的coef值
    */
    static const float RELU6_COEF = 6.0;

    /**
     * @ingroup domi_omg
     * @brief stride默认值
     */
    static const uint32_t STRIDE_DEFAULT_VALUE = 1;

    /**
     * @ingroup domi_omg
     * @brief pad默认值
     */
    static const uint32_t PAD_DEFAULT_VALUE = 0;

    /**
     * @ingroup domi_omg
     * @brief dilation默认值
     */
    static const uint32_t DILATION_DEFAULT_VALUE = 1;

    /**
     * @ingroup domi_omg
     * @brief kernel默认值
     */
    static const uint32_t KERNEL_DEFAULT_VALUE = 0;

    /**
     * @ingroup domi_omg
     * @brief 默认卷积Group Size
     */
    static const uint32_t DEFAULT_CONV_GROUP = 1;

    /**
     * @ingroup domi_omg
     * @brief 默认的反卷积adj
     */
    static const uint32_t DEFAULT_DECONV_ADJ = 0;

    /**
     * @ingroup domi_omg
     * @brief 表示数值1
     */
    static const uint32_t NUM_ONE = 1;

    /**
     * @ingroup domi_omg
     * @brief dim size默认值
     */
    static const int32_t DIM_DEFAULT_SIZE = 4;

    /**
     * @ingroup domi_omg
     * @brief   spatial dim size默认值
     */
    static const int32_t SPATIAL_DIM_DEFAULT_SIZE = 2;

    /**
    * @ingroup domi_omg
    * @brief dim 扩展默认值
    */
    static const int32_t DIM_DEFAULT_VALUE = 1;

    /**
    * @ingroup domi_omg
    * @brief opdef中weight list的第一个为filter
    */
    static const int32_t WEIGHT_FILTER_INDEX = 0;

    /**
    * @ingroup domi_omg
    * @brief opdef中weight list的第2个为bias
    */
    static const int32_t WEIGHT_BIAS_INDEX = 1;

    static const int32_t TENSOR_ND_SUPPORT_SIZE = 8;

    /**
    * @ingroup domi_omg
    * @brief NCHW索引默认值
    */
    static const int32_t NCHW_DIM_N = 0;
    static const int32_t NCHW_DIM_C = 1;
    static const int32_t NCHW_DIM_H = 2;
    static const int32_t NCHW_DIM_W = 3;

    /**
    * @ingroup domi_omg
    * @brief KCHW索引默认值
    */
    static const int32_t KCHW_DIM_K = 0;
    static const int32_t KCHW_DIM_C = 1;
    static const int32_t KCHW_DIM_H = 2;
    static const int32_t KCHW_DIM_W = 3;

    /**
    * @ingroup domi_omg
    * @brief HWCK索引默认值
    */
    static const int32_t HWCK_DIM_H = 0;
    static const int32_t HWCK_DIM_W = 1;
    static const int32_t HWCK_DIM_C = 2;
    static const int32_t HWCK_DIM_K = 3;

    /**
    * @ingroup domi_omg
    * @brief NHWC索引默认值
    */
    static const int32_t NHWC_DIM_N = 0;
    static const int32_t NHWC_DIM_H = 1;
    static const int32_t NHWC_DIM_W = 2;
    static const int32_t NHWC_DIM_C = 3;

    /**
    * @ingroup domi_omg
    * @brief CHW索引默认值
    */
    static const int32_t CHW_DIM_C = 0;
    static const int32_t CHW_DIM_H = 1;
    static const int32_t CHW_DIM_W = 2;

    /**
    * @ingroup domi_omg
    * @brief HWC索引默认值
    */
    static const int32_t HWC_DIM_H = 0;
    static const int32_t HWC_DIM_W = 1;
    static const int32_t HWC_DIM_C = 2;
    /**
    * @ingroup domi_omg
    * @brief Pad索引默认值
    */
    static const int32_t PAD_H_HEAD = 0;
    static const int32_t PAD_H_TAIL = 1;
    static const int32_t PAD_W_HEAD = 2;
    static const int32_t PAD_W_TAIL = 3;


    /**
    * @ingroup domi_omg
    * @brief window索引默认值
    */
    static const int32_t WINDOW_H = 0;
    static const int32_t WINDOW_W = 1;

    /**
    * @ingroup domi_omg
    * @brief stride索引默认值
    */
    static const int32_t STRIDE_H = 0;
    static const int32_t STRIDE_W = 1;

    /**
    * @ingroup domi_omg
    * @brief dilation索引默认值
    */
    static const int32_t DILATION_H = 0;
    static const int32_t DILATION_W = 1;

    /**
     * @ingroup domi_omg
     * @brief the num of XRBG channel
     */
    static const int32_t XRGB_CHN_NUM = 4;

    /**
    * @ingroup domi_omg
    * @brief 默认的tensor format
    */
    extern const int DEFAULT_FORMAT;

    /**
     * @ingroup domi_omg
     * @brief global pooling默认值
     */
    static const bool DEFAULT_GLOBAL_POOLING = false;

    const uint32_t MODEL_VERSION = 0x10000000; /**< 模型版本 1.0 */

    //Eltwise算子的输入数量
    static const int ELTWISE_MIN_INPUT_SIZE = 2;

    /**
    * @brief 模型文件文件头定义
    */
    struct ModelFileHeader
    {
        uint32_t magic = MODEL_FILE_MAGIC_NUM;   /**< 魔数DOMI */
        uint32_t headsize = MODEL_FILE_HEAD_LEN; /**< 模型头长度，固定256 */
        uint32_t version = MODEL_VERSION; /**< 版本1.0 */
        uint8_t checksum[MODEL_FILE_CHECKSUM_LENGTH] = { 0 };  /**< 签名 */
        uint32_t length = 0;      /**< 密文长度, 非加密模型为明文长度 */
        uint8_t is_encrypt = ModelEncryptType::UNENCRYPTED;  /**< 是否加密 0:不加密，1:加密 */
        uint8_t is_checksum = ModelCheckType::CHECK;  /**< 是否检查checksum */
        uint8_t modeltype = 0; /**< 0：IR模型 1：OM标准模型 2: OM Tiny模型 */
        uint8_t genmode = 0;   /**< 0：离线生成 1：在线生成 */
        uint8_t name[MODEL_NAME_LENGTH] = { 0 };  /**< 模型名字32长度*/
        uint32_t ops = 0; /**< 算力，单位Kops */
        uint8_t userdefineinfo[USER_DEFINE_INFO_LENGTH] = { 0 }; /**< 用户自定义信息 32长度 */
        uint8_t reserved[MODEL_FILE_RESERVED_LENGTH] = { 0 };   /**< 保留字段 104长度 */
    };
    #define PTHREAD_CREAT_SUCCESS       (0) //pthread_creat success

    //CCE定义的常量
    /**
    * @ingroup domi
    * @brief original tensor type
    */
    typedef enum tagDomiTensorFormat
    {
        DOMI_TENSOR_NCHW = 0,         /**< NCHW */
        DOMI_TENSOR_NHWC,             /**< NHWC */
        DOMI_TENSOR_ND,               /**< Nd Tensor */
        DOMI_TENSOR_NC1HWC0,          /**< NC1HWC0 */
        DOMI_TENSOR_FRACTAL_Z,        /**< FRACTAL_Z */
        DOMI_TENSOR_NC1C0HWPAD,
        DOMI_TENSOR_NHWC1C0,
        DOMI_TENSOR_FSR_NCHW,
        DOMI_TENSOR_FRACTAL_DECONV,
        DOMI_TENSOR_BN_WEIGHT,
        DOMI_TENSOR_CHWN,            /*Android NN Depth CONV*/
        DOMI_TENSOR_RESERVED
    } domiTensorFormat_t;

    /**
    * @ingroup domi
    * @brief Filter format
    */
    typedef enum tagDomiFilterFormat
    {
        DOMI_FILTER_KCHW,             /**< KCHW */
        DOMI_FILTER_HWCK,             /**< HWCK */
        DOMI_FILTER_RESERVED
    } domiFilterFormat_t;

    /**
    * @ingroup domi
    * @brief Const data trans type
    */
    typedef enum tagDomiConstDataTransType
    {
        DOMI_CONST_DATA_NOT_CHANGE = 0,             /**< 不需要处理 */
        DOMI_CONST_DATA_TRANS_MATMUL,                      /**< TensorFlow的const输入给MatMul，需要转置 */
        DOMI_CONST_DATA_RESERVED
    } domiConstDataTransType_t;

    /**
    * @ingroup domi
    * @brief mode of activation
    */
    typedef enum tagDomiActivationMode
    {
        DOMI_ACTIVATION_SIGMOID = 0,      /**< sigmoid */
        DOMI_ACTIVATION_RELU,             /**< ReLU */
        DOMI_ACTIVATION_TANH,             /**< tanh */
        DOMI_ACTIVATION_CLIPPED_RELU,     /**< clipped ReLU */
        DOMI_ACTIVATION_ELU,              /**< ELU */
        DOMI_ACTIVATION_LEAKY_RELU,
        DOMI_ACTIVATION_ABS,              /**< Abs */
        DOMI_ACTIVATION_RESERVED
    } domiActivationMode_t;

    /**
    * @ingroup domi
    * @brief mode of batchnorm
    */
    typedef enum tagDomiBatchNormMode
    {
        DOMI_BATCHNORM_PER_ACTIVATION = 0,    /**< bnScale, bnBias tensor dims are 1xCxHxW */
        DOMI_BATCHNORM_SPATIAL,               /**< bnScale, bnBias tensor dims are 1xCx1x1 */
        DOMI_BATCHNORM_RESERVED
    } domiBatchNormMode_t;

    /**
    * @ingroup domi
    * @brief eltwise mode
    */
    typedef enum tagDomiEltwiseMode {
        DOMI_ELTWISE_PROD = 0,    /**< prod */
        DOMI_ELTWISE_SUM,         /**< sum */
        DOMI_ELTWISE_MAX,         /**< max */
        DOMI_ELTWISE_RESERVED
    } domiEltwiseMode_t;

    /**
    * @ingroup domi
    * @brief mode of padding
    */
    typedef enum tagDomiPaddingMode
    {
        DOMI_PADDING_CEIL = 0,             /**< Default padding mode, same with caffe, same with MxNet full mode */
        DOMI_PADDING_DIRECTASSIGN,         /**< Default padding mode: NOTSET */
        DOMI_PADDING_VALID,                /**< VALID padding mode , same with tensorflow VALID mode */
        DOMI_PADDING_SAME,                 /**< Padding values of 0 are always used */
        DOMI_PADDING_CEIL_NEW,                 /**< Padding values of 0 are always used */
        DOMI_PADDING_VALID_NEW,                 /**< Padding values of 0 are always used */
        DOMI_PADDING_SAME_NEW,                 /**< Padding values of 0 are always used */
        DOMI_PADDING_RESERVED
    } domiPaddingMode_t;

    /**
    * @ingroup domi
    * @brief algorithm of convolution forward
    */
    typedef enum tagDomiConvolutionFwdAlgo
    {
        DOMI_CONVOLUTION_FWD_ALGO_GEMM = 0,   /**< matrix gemm algo */
        DOMI_CONVOLUTION_FWD_ALGO_WINOGRAD,   /**< Winograd Transform algo */
        DOMI_CONVOLUTION_FWD_ALGO_GEMM_ACCU_FLOAT32,
        DOMI_CONVOLUTION_FWD_ALGO_RESERVED
    } domiConvolutionFwdAlgo_t;

    typedef enum tagDomiFullConnectFwdAlgo
    {
        DOMI_FULLCONNECT_FWD_ALGO_HALF = 0,  //accumulate in L0c with FP16
        DOMI_FULLCONNECT_FWD_ALGO_FLOAT32    //accumulate in L0c with FP32
    }domiFullConnectFwdAlgo_t;

    typedef enum tagDomiPooingFwdAlgo
    {
        DOMI_POOLING_FWD_ALGO_HALF = 0, //accumulate in L0c with FP16
        DOMI_POOLING_FWD_ALGO_FLOAT32  //accumulate in L0c with FP32
    }domiPooingFwdAlgo_t;

    /**
    * @ingroup domi
    * @brief mode of convolution
    */
    typedef enum tagDomiConvolutionMode
    {
        DOMI_CONV_CONVOLUTION = 0,            /**< math convolution */
        DOMI_CONV_CROSS_CORRELATION,          /**< cross-correlation convolution */
        DOMI_CONV_DECONVOLUTION,              /**< deconvolution, also named transposed convolution*/
        DOMI_CONV_MODE_RESERVED
    } domiConvolutionMode_t;

    /**
    * @ingroup domi
    * @brief softmax mode
    */
    typedef enum tagDomiSoftmaxMode
    {
        DOMI_SOFTMAX_MODE_INSTANCE = 0,   /**< compute the softmax over all C, H, W for each N */
        DOMI_SOFTMAX_MODE_CHANNEL,        /**< compute the softmax over all C for each H, W, N */
        DOMI_SOFTMAX_MODE_HEIGHT,         /**< compute the softmax over all H for each N, C, W */
        DOMI_SOFTMAX_MODE_WIDTH,          /**< compute the softmax over all W for each N, C, H */
        DOMI_SOFTMAX_MODE_RESERVED
    } domiSoftmaxMode_t;

    /**
    * @ingroup domi
    * @brief softmax algorithm
    */
    typedef enum tagDomiSoftmaxAlgo
    {
        DOMI_SOFTMAX_FAST = 0,    /**< straightforward implementation */
        DOMI_SOFTMAX_ACCURATE,    /**< subtract max from every point to avoid overflow */
        DOMI_SOFTMAX_LOG,         /**< perform the Log softmax operation to avoid overflow */
        DOMI_SOFTMAX_RESERVED
    } domiSoftmaxAlgo_t;

    /**
    * @ingroup domi
    * @brief original data type
    */
    typedef enum tagDomiDataType
    {
        DOMI_DATA_FLOAT = 0,         /**< float type */
        DOMI_DATA_HALF,              /**< fp16 type */
        DOMI_DATA_INT8,              /**< int8 type */
        DOMI_DATA_INT32,             /**< int32 type */
        DOMI_DATA_UINT8,             /**< uint8 type */
        DOMI_DATA_HALF_UINT16_PROPOSAL,  /**<mixed type for proposal*/
        DOMI_DATA_INT16,             /**< int16 type */
        DOMI_DATA_UINT16,            /**< uint16 type */
        DOMI_DATA_UINT32,             /**< uint32 type */
        DOMI_DATA_INT64,              /**< int64 type */
        DOMI_DATA_UINT64,            /**< uint64 type */
        DOMI_DATA_DOUBLE,            /**< double type */
        DOMI_DATA_BOOL,              /**< bool type */
        DOMI_DATA_RESERVED
    } domiDataType_t;

    /**
    * @ingroup domi
    * @brief mode of pooling
    */
    typedef enum tagDomiPoolingMode
    {
        DOMI_POOLING_MAX = 0,             /**< max pooling */
        DOMI_POOLING_AVG,                 /**< average pooling */
        DOMI_POOLING_RESERVED
    } domiPoolingMode_t;

    /**
    * @ingroup domi
    * @brief propagate Nan
    */
    typedef enum tagDomiNanPropagation
    {
        DOMI_NAN_NOT_PROPAGATE = 0,  /**< Nan numbers are not propagated */
        DOMI_NAN_PROPAGATE,           /**< Nan numbers are propagated */
        DOMI_NAN_PROPAGATE_RESERVED
    } domiNanPropagation_t;

    /**
    * @ingroup domi
    * @brief yolo version
    */
    typedef enum tagDomiYoloVersion
    {
        DOMI_YOLO_V2 =1,
        DOMI_YOLO_V3,
        DOMI_YOLO_TRSERVED
    }domiYoloVersion_t;

    /**
    * @ingroup domi
    * @brief RNNDataLayout
    */
    typedef enum tagDomiRNNDataLayout
    {
        DOMI_RNN_ND_TBX = 0, //数据[max_time,batch_size,Xt]
        DOMI_RNN_ND_BTX,   //数据[batch_size,max_time,Xt]
        DOMI_RNN_5D_TX1BX, //数据[max_time,Xt,1,batch_size,Xt]
        DOMI_RNN_5D_BX1TX, //数据[batch_size,Xt,1,max_time,Xt]
        DOMI_RNN_4DTBX1, //caffe专用
        DOMI_ENN_DL_RESERVED
    } domiRNNDataLayout_t;

     /**
    * @ingroup domi
    * @brief RNNInputMode
    */
    typedef enum tagDomiRNNInputMode
    {
        DOMI_RNN_LINEAR_INPUT = 0,
        DOMI_RNN_SKIP_INPUT
    } domiRNNInputMode_t;

     /**
    * @ingroup domi
    * @brief RNNDirectionMode
    */
    typedef enum tagDomiRNNDirectionMode
    {
        DOMI_RNN_UNIDIRECTIONAL = 0,
        DOMI_RNN_BIDIRECTIONAL
    } domiDirectionMode_t;

     /**
    * @ingroup domi
    * @brief RNNMode
    */
    typedef enum tagDomiRNNMode
    {
        DOMI_RNN_RELU = 0,
        DOMI_RNN_TANH,
        DOMI_LSTM,
        DOMI_GRU,
        DOMI_RNN_MODE_RESERVED
    } domiRNNMode_t;

#pragma pack(1)                             /** 单字节对齐   **/
    /**
    * @ingroup domi_ome
    * @brief DUMP file struct
    */
    struct FileHeader {
        int32_t Version;                    /** 版本号  **/
        int32_t Output_Offset;              /** output偏移地址 ***/
        char Reserved[24] = {0};            /**  保留  ***/
    };

    struct BasicInfo {
        struct FileHeader header;           /** 文件头  **/
        int32_t stream_id;                 /** stread id **/
        uint64_t start_time;                /** 开始时间 **/
        uint64_t end_time;                  /** 结束时间 **/
        uint32_t input_size;                /** 输入内存大小 **/
        uint32_t output_size;               /** 输出内存大小 **/
        uint32_t weight_size;               /** 权值内存大小 **/
        uint32_t workspace_size;            /** workspace    **/
        uint32_t total_size;                /** 内存总大小   **/
    };
#pragma pack()                             /** 取消单字节对齐   **/
};

#endif //DOMI_OMG_COMMON_TYPES_H_

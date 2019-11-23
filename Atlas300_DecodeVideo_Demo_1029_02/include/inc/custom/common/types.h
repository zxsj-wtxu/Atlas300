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
#include <memory>
#include <algorithm>
#include <limits.h>
#include <linux/limits.h>
#include "framework/common/fmk_error_codes.h"
#include "framework/common/fmk_types.h"
#include "framework/common/op_types.h"

#define CAFFE_BATCH_NORM_ESP_DEFAULT_FLOAT             0.00001
#define CAFFE_BATCH_NORM_ESP_DEFAULT_DIFF_FLOAT        0.000000001
#define CAFFE_BATCH_NORM_MOVING_AVERAGE_FRACTION_FLOAT 0.999
#define YOLO_EPSINON 0.00001
#if !defined( __ANDROID__) && !defined(ANDROID)
#define DOMI_DYNAMIC_CAST static_cast
#define DOMI_DYNAMIC_POINTER_CAST std::static_pointer_cast
#else
#define DOMI_DYNAMIC_CAST static_cast
#define DOMI_DYNAMIC_POINTER_CAST std::static_pointer_cast
#endif

namespace domi
{
    /**
     * @brief 支持的公共的Properties名称
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string PROP_OME_START_TIME ;  /**< 启动时间 */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string PROP_OME_DUMP_PATH ;  /**< dump路径 */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string PROP_OME_LOG_PATH ;  /**< 日志路径 */

    // Profile相关常量
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t CCE_PROFILE_ON  ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t CCE_PROFILE_OFF ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string OME_PROFILE ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string CCE_PROFILE ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string RTS_PROFILE ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string PROFILER_JOBCTX ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string PROFILER_TARGET_PATH ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string RTS_PROFILE_PATH ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string PROFILE_STOP_KEY ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string PROFILE_STOP_VALUE ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::map<std::string, std::string> PROFILE_COMPONENT_MAP;

    /**
     * @brief task下沉相关数据结构定义
    */
    // Build模式
    enum BuildMode {
        GEN_TASK_WITHOUT_L2FUSION = 3,      // 携带task数据（融合功能关闭L2）
        GEN_TASK_WITHOUT_FUSION = 4,        // 携带task数据（融合功能全部关闭）
        GEN_TASK_WITH_FUSION = 5            // 携带task数据（融合功能全部开启UB/L1/L2）
    };

    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string MODEL_ATTR_TASKS ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string MODEL_ATTR_TASK_GEN_BASE_ADDR ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string MODEL_ATTR_TASK_GEN_WEIGHT_ADDR ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string MODEL_ATTR_FUSION_MODEL_DEF ;

    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int MODEL_MAX_SIZE ; // Max size of 2 GB minus 1 byte.
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint64_t FILE_HEADER_MAX_SIZE ; // Max size of 3 GB.

#if !defined( __ANDROID__) && !defined(ANDROID)
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint64_t ALLOC_MEMORY_MAX_SIZE ; // Max size of 8 GB.
#else
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint64_t ALLOC_MEMORY_MAX_SIZE ; // Max size of 512M.
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

    REGISTER_OPTYPE_DECLARE(DATA, "Data");
    REGISTER_OPTYPE_DECLARE(CONVOLUTION, "Convolution");
    REGISTER_OPTYPE_DECLARE(CORRELATION, "Correlation");
    REGISTER_OPTYPE_DECLARE(CORRELATIONV2, "Correlation_V2");
    REGISTER_OPTYPE_DECLARE(DECONVOLUTION, "Deconvolution");
    REGISTER_OPTYPE_DECLARE(POOLING, "Pooling");
    REGISTER_OPTYPE_DECLARE(ELTWISE, "Eltwise");
    REGISTER_OPTYPE_DECLARE(RELU, "ReLU");
    REGISTER_OPTYPE_DECLARE(RELU6, "ReLU6");
    REGISTER_OPTYPE_DECLARE(SIGMOID, "Sigmoid");
    REGISTER_OPTYPE_DECLARE(ABSVAL, "AbsVal");
    REGISTER_OPTYPE_DECLARE(TANH, "TanH");
    REGISTER_OPTYPE_DECLARE(PRELU, "PReLU");
    REGISTER_OPTYPE_DECLARE(BATCHNORM, "BatchNorm");
    REGISTER_OPTYPE_DECLARE(FUSIONBATCHNORM, "FusionBatchNorm");
    REGISTER_OPTYPE_DECLARE(SCALE, "Scale");
    REGISTER_OPTYPE_DECLARE(FULL_CONNECTION, "FullConnection");
    REGISTER_OPTYPE_DECLARE(SOFTMAX, "Softmax");
    REGISTER_OPTYPE_DECLARE(PLUS, "Plus");
    REGISTER_OPTYPE_DECLARE(ACTIVATION, "Activation");
    REGISTER_OPTYPE_DECLARE(FLATTEN, "Flatten");
    REGISTER_OPTYPE_DECLARE(ADD, "Add");
    REGISTER_OPTYPE_DECLARE(SUB, "Sub");
    REGISTER_OPTYPE_DECLARE(MUL, "Mul");
    REGISTER_OPTYPE_DECLARE(MATMUL, "MatMul");
    REGISTER_OPTYPE_DECLARE(RSQRT, "Rsqrt");
    REGISTER_OPTYPE_DECLARE(BIASADD, "BiasAdd");
    REGISTER_OPTYPE_DECLARE(RESHAPE, "Reshape");
    REGISTER_OPTYPE_DECLARE(DEPCONVOLUTION, "ConvolutionDepthwise");
    REGISTER_OPTYPE_DECLARE(DROPOUT, "Dropout");
    REGISTER_OPTYPE_DECLARE(CONCAT, "Concat");
    REGISTER_OPTYPE_DECLARE(ROIPOOLING, "ROIPooling");
    REGISTER_OPTYPE_DECLARE(PROPOSAL, "Proposal");
    REGISTER_OPTYPE_DECLARE(FSRDETECTIONOUTPUT, "FSRDetectionOutput");
    REGISTER_OPTYPE_DECLARE(DETECTIONPOSTPROCESS, "Detectpostprocess");
    REGISTER_OPTYPE_DECLARE(LRN, "LRN");
    REGISTER_OPTYPE_DECLARE(TRANSDATA, "TransData");
    REGISTER_OPTYPE_DECLARE(PERMUTE, "Permute");
    REGISTER_OPTYPE_DECLARE(SSDNORMALIZE, "SSDNormalize");
    REGISTER_OPTYPE_DECLARE(SSDPRIORBOX, "SSDPriorBox");
    REGISTER_OPTYPE_DECLARE(NETOUTPUT, "NetOutput");
    REGISTER_OPTYPE_DECLARE(SSDDETECTIONOUTPUT, "SSDDetectionOutput");
    REGISTER_OPTYPE_DECLARE(CHANNELAXPY, "ChannelAxpy");
    REGISTER_OPTYPE_DECLARE(PSROIPOOLING, "PSROIPooling");
    REGISTER_OPTYPE_DECLARE(POWER, "Power");
    REGISTER_OPTYPE_DECLARE(POW, "Pow");
    REGISTER_OPTYPE_DECLARE(ROIALIGN, "ROIAlign");
    REGISTER_OPTYPE_DECLARE(PYTHON, "Python");
    REGISTER_OPTYPE_DECLARE(FREESPACEEXTRACT, "FreespaceExtract");
    REGISTER_OPTYPE_DECLARE(SPATIALTF, "SpatialTransform");
    REGISTER_OPTYPE_DECLARE(SHAPE, "Shape");
    REGISTER_OPTYPE_DECLARE(ARGMAX, "ArgMax");
    REGISTER_OPTYPE_DECLARE(GATHERND, "GatherNd");
    REGISTER_OPTYPE_DECLARE(GATHER, "Gather");
    REGISTER_OPTYPE_DECLARE(REALDIV, "RealDiv");
    REGISTER_OPTYPE_DECLARE(PACK, "Pack");
    REGISTER_OPTYPE_DECLARE(SLICE, "Slice");
    REGISTER_OPTYPE_DECLARE(FLOORDIV, "FloorDiv");
    REGISTER_OPTYPE_DECLARE(SQUEEZE, "Squeeze");
    REGISTER_OPTYPE_DECLARE(STRIDEDSLICE, "StridedSlice");
    REGISTER_OPTYPE_DECLARE(RANGE, "Range");
    REGISTER_OPTYPE_DECLARE(RPNPROPOSALS, "GenerateRpnProposals");
    REGISTER_OPTYPE_DECLARE(DECODEBBOX, "DecodeBBox");
    REGISTER_OPTYPE_DECLARE(PAD, "Pad");
    REGISTER_OPTYPE_DECLARE(PADV2, "PadV2");
    REGISTER_OPTYPE_DECLARE(MIRRORPAD, "MirrorPad");
    REGISTER_OPTYPE_DECLARE(TILE, "Tile");
    REGISTER_OPTYPE_DECLARE(SIZE, "Size");
    REGISTER_OPTYPE_DECLARE(CLIPBOXES, "Clipboxes");
    REGISTER_OPTYPE_DECLARE(FASTRCNNPREDICTIONS, "FastrcnnPredictions");
    REGISTER_OPTYPE_DECLARE(SPLIT, "Split");
    REGISTER_OPTYPE_DECLARE(EXPANDDIMS, "ExpandDims");
    REGISTER_OPTYPE_DECLARE(MEAN, "Mean");
    REGISTER_OPTYPE_DECLARE(GREATER, "Greater");
    REGISTER_OPTYPE_DECLARE(SWITCH, "Switch");
    REGISTER_OPTYPE_DECLARE(MERGE, "Merge");
    REGISTER_OPTYPE_DECLARE(TRANSPOSE, "Transpose");
    REGISTER_OPTYPE_DECLARE(CAST, "Cast");
    REGISTER_OPTYPE_DECLARE(REGION, "Region")
    REGISTER_OPTYPE_DECLARE(YOLO, "Yolo")
    REGISTER_OPTYPE_DECLARE(YOLODETECTIONOUTPUT, "YoloDetectionOutput")
    REGISTER_OPTYPE_DECLARE(FILL, "Fill");
    REGISTER_OPTYPE_DECLARE(REVERSE, "Reverse");
    REGISTER_OPTYPE_DECLARE(UNPACK, "Unpack");
    REGISTER_OPTYPE_DECLARE(YOLO2REORG, "Yolo2Reorg");
    REGISTER_OPTYPE_DECLARE(REDUCESUM, "ReduceSum");
    REGISTER_OPTYPE_DECLARE(CONSTANT, "Const");
    REGISTER_OPTYPE_DECLARE(RESIZEBILINEAR, "ResizeBilinear");
    REGISTER_OPTYPE_DECLARE(MAXIMUM, "Maximum");
    REGISTER_OPTYPE_DECLARE(FRAMEWORKOP, "FrameworkOp");
    REGISTER_OPTYPE_DECLARE(ARG, "_Arg");
    REGISTER_OPTYPE_DECLARE(FUSEDBATCHNORMGRAD, "FusedBatchNormGrad");
    REGISTER_OPTYPE_DECLARE(LSTM, "LSTM");
    REGISTER_OPTYPE_DECLARE(HIGHWAY, "HighWay");
    REGISTER_OPTYPE_DECLARE(RNN, "RNN");
    REGISTER_OPTYPE_DECLARE(ATTENTIONDECODER, "AttentionDecoder");
    REGISTER_OPTYPE_DECLARE(LOGICAL_NOT, "LogicalNot");
    REGISTER_OPTYPE_DECLARE(LOGICAL_AND, "LogicalAnd");
    REGISTER_OPTYPE_DECLARE(LOGICAL_OR, "LogicalOr");
    REGISTER_OPTYPE_DECLARE(EQUAL, "Equal");
    REGISTER_OPTYPE_DECLARE(INTERP, "Interp");
    REGISTER_OPTYPE_DECLARE(SHUFFLECHANNEL, "ShuffleChannel");
    REGISTER_OPTYPE_DECLARE(AIPP, "Aipp");
    REGISTER_OPTYPE_DECLARE(MULTISHAPE, "MultiShape");
    REGISTER_OPTYPE_DECLARE(RECIPROCAL, "Reciprocal");
    REGISTER_OPTYPE_DECLARE(SELU, "Selu");
    REGISTER_OPTYPE_DECLARE(ELU, "Elu");
    REGISTER_OPTYPE_DECLARE(ACOSH, "Acosh");
    REGISTER_OPTYPE_DECLARE(ASINH, "Asinh");
    REGISTER_OPTYPE_DECLARE(MINIMUM, "Minimum");
    REGISTER_OPTYPE_DECLARE(CLIP, "Clip");
    REGISTER_OPTYPE_DECLARE(L2NORMALIZE, "L2Normalize");
    REGISTER_OPTYPE_DECLARE(CROPANDRESIZE, "CropAndResize");
    REGISTER_OPTYPE_DECLARE(UNUSEDCONST, "UnusedConst");
    REGISTER_OPTYPE_DECLARE(SPARSETODENSE, "SparseToDense");
    REGISTER_OPTYPE_DECLARE(NONMAXSUPPRESSION, "NonMaxSuppression");
    REGISTER_OPTYPE_DECLARE(TOPKV2,"TopKV2");
    REGISTER_OPTYPE_DECLARE(INVERTPERMUTATION, "InvertPermutation");
    REGISTER_OPTYPE_DECLARE(MULTINOMIAL, "Multinomial");
    REGISTER_OPTYPE_DECLARE(REVERSESEQUENCE,"ReverseSequence");
    REGISTER_OPTYPE_DECLARE(REDUCEPROD,"ReduceProd");
    REGISTER_OPTYPE_DECLARE(REDUCEMAX,"ReduceMax");
    REGISTER_OPTYPE_DECLARE(REDUCEMIN,"ReduceMin");
    REGISTER_OPTYPE_DECLARE(EXTRACTIMAGEPATCHES,"ExtractImagePatches");
    REGISTER_OPTYPE_DECLARE(SQRT,"Sqrt");
    REGISTER_OPTYPE_DECLARE(REDUCEALL,"ReduceAll");
    REGISTER_OPTYPE_DECLARE(RESIZENEARESTNEIGHBOR,"ResizeNearestNeighbor");
    REGISTER_OPTYPE_DECLARE(SPACETOBATCHND,"SpaceToBatchND");
    REGISTER_OPTYPE_DECLARE(BATCHTOSPACEND,"BatchToSpaceND");
    REGISTER_OPTYPE_DECLARE(ASSERT,"Assert");
    REGISTER_OPTYPE_DECLARE(GREATEREQUAL, "GreaterEqual");
    REGISTER_OPTYPE_DECLARE(FLOOR,"Floor");
    REGISTER_OPTYPE_DECLARE(SPACETODEPTH,"SpaceToDepth");
    REGISTER_OPTYPE_DECLARE(DEPTHTOSPACE,"DepthToSpace");
    REGISTER_OPTYPE_DECLARE(RINT,"Rint");
    REGISTER_OPTYPE_DECLARE(ATAN, "Atan");
    REGISTER_OPTYPE_DECLARE(ATANH, "Atanh");
    REGISTER_OPTYPE_DECLARE(ACOS, "Acos");
    REGISTER_OPTYPE_DECLARE(ASIN, "Asin");
    REGISTER_OPTYPE_DECLARE(NEG, "Neg");
    REGISTER_OPTYPE_DECLARE(LOG, "Log");
    REGISTER_OPTYPE_DECLARE(TAN, "Tan");
    REGISTER_OPTYPE_DECLARE(ROUND, "Round");
    REGISTER_OPTYPE_DECLARE(UPSAMPLE, "Upsample");
    REGISTER_OPTYPE_DECLARE(FLOORMOD, "FloorMod");
    REGISTER_OPTYPE_DECLARE(LESS, "Less");
    REGISTER_OPTYPE_DECLARE(ZEROSLIKE,"ZerosLike");
    REGISTER_OPTYPE_DECLARE(EXP,"Exp");
    REGISTER_OPTYPE_DECLARE(WHERE,"Where");
    REGISTER_OPTYPE_DECLARE(FAKEQUANTWITHMINMAXVARS,"FakeQuantWithMinMaxVars");
    REGISTER_OPTYPE_DECLARE(SOFTPLUS, "Softplus");
    REGISTER_OPTYPE_DECLARE(SOFTSIGN, "Softsign");
    REGISTER_OPTYPE_DECLARE(COSH, "Cosh");
    REGISTER_OPTYPE_DECLARE(SINH, "Sinh");
    REGISTER_OPTYPE_DECLARE(RETINAMULTIANCHORS, "RetinaMultiAnchor");
    REGISTER_OPTYPE_DECLARE(SQUAREDDIFFERENCE, "SquaredDifference");
    REGISTER_OPTYPE_DECLARE(REQUIREDSPACETOBATCHPADDINGS, "RequiredSpaceToBatchPaddings"); // for retinanet scope fusion
    REGISTER_OPTYPE_DECLARE(SSDPOSTPROCESSOR, "SSDPostProcessor");
    REGISTER_OPTYPE_DECLARE(SSDANCHORGENERATOR, "SSDAnchorGenerator");
    REGISTER_OPTYPE_DECLARE(RETINANETBOXES, "RetinanetBoxes");
    REGISTER_OPTYPE_DECLARE(RETINANETCLIPPEDBOXES, "RetinanetClippedBoxes");
    REGISTER_OPTYPE_DECLARE(RETINANETFILTEREDDETECTIONS, "RetinanetFilteredDetections");
    REGISTER_OPTYPE_DECLARE(RETINANETPOSTPROCESSOR, "RetinanetPostProcessor");
    REGISTER_OPTYPE_DECLARE(RETINANETANCHORS, "RetinanetAnchors");
    REGISTER_OPTYPE_DECLARE(FASTERRCNNMAP, "FasterRCNNMap");
    REGISTER_OPTYPE_DECLARE(FASTERRCNNMAP1, "FasterRCNNMap1");
    REGISTER_OPTYPE_DECLARE(FASTERRCNNSECONDSTAGEPOSTPROCESSOR, "FasterRCNNSecondStagePostprocessor");
    REGISTER_OPTYPE_DECLARE(FASTERRCNNROIINTERPOOLING, "FasterRCNNROIInterPooling");
    REGISTER_OPTYPE_DECLARE(FASTERRCNNFIRSTSTAGEPOSTPROCESSOR, "FasterRCNNFirstStagePostprocessor");
    REGISTER_OPTYPE_DECLARE(FASTERRCNNGRIDANCHORGENERATOR, "FasterRCNNGridAnchorGenerator");
    REGISTER_OPTYPE_DECLARE(ROIINTERPOOLING, "ROIInterPooling");
    REGISTER_OPTYPE_DECLARE(FASTERRCNNCLIPTOWINDOW, "FasterRCNNClipToWindow");
    REGISTER_OPTYPE_DECLARE(EMBEDLOOKUP, "EmbedLookup");
    REGISTER_OPTYPE_DECLARE(HASHLOOKUP, "HashLookup");
    REGISTER_OPTYPE_DECLARE(LSH_PROJ,"LshProject");
    REGISTER_OPTYPE_DECLARE(SVDF, "SVDF");
    REGISTER_OPTYPE_DECLARE(IDENTITY, "Identity");
    /***************ANN专用算子*************************/

    REGISTER_OPTYPE_DECLARE(ANN_MEAN, "AnnMean");
    REGISTER_OPTYPE_DECLARE(ANN_CONVOLUTION, "AnnConvolution");
    REGISTER_OPTYPE_DECLARE(ANN_DEPCONVOLUTION, "AnnDepthConv");
    REGISTER_OPTYPE_DECLARE(ANN_FULLCONNECTION, "AnnFullConnection");
    REGISTER_OPTYPE_DECLARE(ANN_NETOUTPUT, "AnnNetOutput");
    REGISTER_OPTYPE_DECLARE(ANN_DATA,"AnnData");
    REGISTER_OPTYPE_DECLARE(ANN_RESHAPE,"AnnReshape");
    REGISTER_OPTYPE_DECLARE(ANN_ADD,"AnnAdd");
    REGISTER_OPTYPE_DECLARE(ANN_MUL,"AnnMul");
    REGISTER_OPTYPE_DECLARE(ANN_SUB,"AnnSub");
    REGISTER_OPTYPE_DECLARE(ANN_DIV,"AnnDiv");
    REGISTER_OPTYPE_DECLARE(ANN_DEQUANTIZE,"AnnDequant");
    REGISTER_OPTYPE_DECLARE(ANN_QUANTIZE,"AnnQuant");
    REGISTER_OPTYPE_DECLARE(ANN_PAD,"AnnPad");
    REGISTER_OPTYPE_DECLARE(ANN_RESIZE_BILINEAR,"AnnResizeBilinear");

    /***************************************************/
    /******************训练算子*************************/
    REGISTER_OPTYPE_DECLARE(CONVGRADFILTER, "Conv2DBackpropFilter");
    REGISTER_OPTYPE_DECLARE(CONV2D, "Conv2D");
    REGISTER_OPTYPE_DECLARE(CONV2DBACKPROPINPUT, "Conv2DBackpropInput");
    REGISTER_OPTYPE_DECLARE(FUSEDBATCHNORM, "FusedBatchNorm");
    REGISTER_OPTYPE_DECLARE(BIASADDGRAD, "BiasAddGrad");
    REGISTER_OPTYPE_DECLARE(ACTIVATIONGRAD,"ReluGrad");
    REGISTER_OPTYPE_DECLARE(MAXPOOLWITHARGMAX,"MaxPoolWithArgmax");
    REGISTER_OPTYPE_DECLARE(MAXPOOLGRADWITHARGMAX,"MaxPoolGradWithArgmax");
    REGISTER_OPTYPE_DECLARE(SPARSESOFTMAXCROSSENTROPYWITHLOGITS,"SparseSoftmaxCrossEntropyWithLogits");
    REGISTER_OPTYPE_DECLARE(SNAPSHOT,"Snapshot");    
    
    REGISTER_OPTYPE_DECLARE(MEANGRAD, "MeanGrad");
    REGISTER_OPTYPE_DECLARE(TRANSLATE, "Translate");
    REGISTER_OPTYPE_DECLARE(ADDN,"AddN");
    REGISTER_OPTYPE_DECLARE(L2LOSS,"L2Loss");
    REGISTER_OPTYPE_DECLARE(MULTIPLY,"Multiply");

    /*变量下沉相关*/
    REGISTER_OPTYPE_DECLARE(VARIABLEV2, "VariableV2");
    REGISTER_OPTYPE_DECLARE(VARHANDLEOP, "VarHandleOp");
    REGISTER_OPTYPE_DECLARE(VARIABLE, "Variable");
    
    REGISTER_OPTYPE_DECLARE(ASSIGN, "Assign");
    REGISTER_OPTYPE_DECLARE(ASSIGNVARIABLEOP, "AssignVariableOp");

    REGISTER_OPTYPE_DECLARE(ASSIGNADD,"AssignAdd");
    REGISTER_OPTYPE_DECLARE(ASSIGNADDVARIABLEOP,"AssignAddVariableOp");

    REGISTER_OPTYPE_DECLARE(ASSIGNSUB,"AssignSub");
    REGISTER_OPTYPE_DECLARE(ASSIGNSUBVARIABLEOP,"AssignSubVariableOp");

    REGISTER_OPTYPE_DECLARE(APPLYMOMENTUM,"ApplyMomentum");
    REGISTER_OPTYPE_DECLARE(RESOURCEAPPLYMOMENTUM,"ResourceApplyMomentum");
    REGISTER_OPTYPE_DECLARE(NOOP, "NoOp");
    
    /***************************************************/
    REGISTER_OPTYPE_DECLARE(SQUARE, "Square");
    REGISTER_OPTYPE_DECLARE(HCOMBROADCAST,"HCOMBroadcast");
    REGISTER_OPTYPE_DECLARE(HCOMALLGATHER,"HCOMAllgather");
    REGISTER_OPTYPE_DECLARE(HCOMALLREDUCE,"HCOMAllreduce");
    REGISTER_OPTYPE_DECLARE(VARASSIGN,"VarAssign");
    REGISTER_OPTYPE_DECLARE(VARISINITIALIZEDOP,"VarIsInitializedOp");
    REGISTER_OPTYPE_DECLARE(VAR,"Var");

    //算子重名时，公用的类型
    REGISTER_OPTYPE_DECLARE(DETECTIONOUTPUT, "DetectionOutput");

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
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t MODEL_FILE_MAGIC_NUM; /**< 魔数DOMI */

    /**
    * @brief 模型头长度
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t MODEL_FILE_HEAD_LEN ;

    /**
    * @brief 模型名字长度
    */
    #define MODEL_NAME_LENGTH  (32)

    /**
    * @brief 用户自定义信息长度
    */
    #define USER_DEFINE_INFO_LENGTH  (32)

    /**
    * @brief 模型文件签名长度
    */
    #define MODEL_FILE_CHECKSUM_LENGTH  (64)

    /**
    * @brief 模型文件头保留字段长度
    */
    #define MODEL_FILE_RESERVED_LENGTH (79)

    /**
     * @ingroup domi_omg
     * @brief INPUT节点类型
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string INPUT_TYPE ;

    /**
     * @ingroup domi_omg
     * @brief AIPP 标记, 标记aipp conv算子
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string AIPP_CONV_FLAG ;

    /**
    * @ingroup domi_omg
    * @brief AIPP 标记, 标记aipp data算子
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string AIPP_DATA_FLAG ;

    /**
     * @ingroup domi_omg
     * @brief Data算子的标记，标记这个输入将会输入给动态AIPP算子
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string INPUT_TO_DYNAMIC_AIPP ;

    /**
     * @ingroup domi_omg
     * @brief 记录动态AIPP对应的模型输入的W维度
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string AIPP_RELATED_DATA_DIM_W;

    /**
     * @ingroup domi_omg
     * @brief记录动态AIPP对应的模型输入的H维度
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string AIPP_RELATED_DATA_DIM_H;

    /**
     * @ingroup domi_omg
     * @brief DATA节点类型
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string DATA_TYPE ;

    /**
     * @ingroup domi_omg
     * @brief 框架算子类型
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string FRAMEWORK_OP_TYPE ;

      /**
     * @ingroup domi_omg
     * @brief DATA节点类型
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string ANN_DATA_TYPE ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string ANN_NETOUTPUT_TYPE ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string ANN_DEPTHCONV_TYPE ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string ANN_CONV_TYPE ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string ANN_FC_TYPE ;
    /**
    * @ingroup domi_omg
    * @brief 卷积节点类型
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string NODE_NAME_NET_OUTPUT ;

    /**
    * @ingroup domi_omg
    * @brief 卷积节点类型
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string OP_TYPE_CONVOLUTION ;
    /**
    * @ingroup domi_omg
    * @brief 为硬AIPP增加卷积节点名字
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string AIPP_CONV_OP_NAME ;
    /**
    * @ingroup domi_omg
    * @brief 算子配置项分隔符
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const std::string OP_CONF_DELIMITER ;

    /**
     * @ingroup domi_omg
     * @brief alpha默认值
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const float ALPHA_DEFAULT_VALUE ;

    /**
     * @ingroup domi_omg
     * @brief beta默认值
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const float BETA_DEFAULT_VALUE ;

    /**
    * @ingroup domi_omg
    * @brief coef默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const float COEF_DEFAULT_VALUE ;

    /**
    * @ingroup domi_omg
    * @brief Relu6的coef值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const float RELU6_COEF ;

    /**
     * @ingroup domi_omg
     * @brief stride默认值
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t STRIDE_DEFAULT_VALUE ;

    /**
     * @ingroup domi_omg
     * @brief pad默认值
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t PAD_DEFAULT_VALUE ;

    /**
     * @ingroup domi_omg
     * @brief dilation默认值
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int DILATION_DEFAULT_VALUE ;

    /**
     * @ingroup domi_omg
     * @brief kernel默认值
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t KERNEL_DEFAULT_VALUE ;

    /**
     * @ingroup domi_omg
     * @brief 默认卷积Group Size
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t DEFAULT_CONV_GROUP ;

    /**
     * @ingroup domi_omg
     * @brief 默认的反卷积adj
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t DEFAULT_DECONV_ADJ ;

    /**
     * @ingroup domi_omg
     * @brief 表示数值1
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t NUM_ONE ;

    /**
     * @ingroup domi_omg
     * @brief dim size默认值
     */
    static const int32_t DIM_DEFAULT_SIZE = 4;

    /**
     * @ingroup domi_omg
     * @brief   spatial dim size默认值
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t SPATIAL_DIM_DEFAULT_SIZE ;

    /**
    * @ingroup domi_omg
    * @brief dim 扩展默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t DIM_DEFAULT_VALUE ;

    /**
    * @ingroup domi_omg
    * @brief opdef中weight list的第一个为filter
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t WEIGHT_FILTER_INDEX ;

    /**
    * @ingroup domi_omg
    * @brief opdef中weight list的第2个为bias
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t WEIGHT_BIAS_INDEX ;

    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t TENSOR_ND_SUPPORT_SIZE ;

    /**
    * @ingroup domi_omg
    * @brief NCHW索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t NCHW_DIM_N ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t NCHW_DIM_C ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t NCHW_DIM_H ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t NCHW_DIM_W ;

    /**
    * @ingroup domi_omg
    * @brief KCHW索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t KCHW_DIM_K ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t KCHW_DIM_C ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t KCHW_DIM_H ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t KCHW_DIM_W ;

    /**
    * @ingroup domi_omg
    * @brief HWCK索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t HWCK_DIM_H ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t HWCK_DIM_W ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t HWCK_DIM_C ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t HWCK_DIM_K ;

    /**
    * @ingroup domi_omg
    * @brief NHWC索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t NHWC_DIM_N ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t NHWC_DIM_H ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t NHWC_DIM_W ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t NHWC_DIM_C ;

        /**
    * @ingroup domi_omg
    * @brief CHWN索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t CHWN_DIM_N ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t CHWN_DIM_C ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t CHWN_DIM_H ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t CHWN_DIM_W ;

    /**
    * @ingroup domi_omg
    * @brief CHW索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t CHW_DIM_C ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t CHW_DIM_H ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t CHW_DIM_W ;

    /**
    * @ingroup domi_omg
    * @brief HWC索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t HWC_DIM_H ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t HWC_DIM_W ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t HWC_DIM_C ;
    /**
    * @ingroup domi_omg
    * @brief Pad索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t PAD_H_HEAD ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t PAD_H_TAIL ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t PAD_W_HEAD ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t PAD_W_TAIL ;


    /**
    * @ingroup domi_omg
    * @brief window索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t WINDOW_H ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t WINDOW_W ;

    /**
    * @ingroup domi_omg
    * @brief stride索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t STRIDE_H ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t STRIDE_W ;

    /**
    * @ingroup domi_omg
    * @brief dilation索引默认值
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t DILATION_H ;
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t DILATION_W ;

    /**
     * @ingroup domi_omg
     * @brief the num of XRBG channel
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int32_t XRGB_CHN_NUM ;

    /**
    * @ingroup domi_omg
    * @brief 默认的tensor format
    */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int DEFAULT_FORMAT;

    /**
     * @ingroup domi_omg
     * @brief global pooling默认值
     */
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const bool DEFAULT_GLOBAL_POOLING ;

    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const uint32_t MODEL_VERSION ; /**< 模型版本 1.0 */

    //Eltwise算子的输入数量
    FMK_FUNC_HOST_VISIBILITY FMK_FUNC_DEV_VISIBILITY extern const int ELTWISE_MIN_INPUT_SIZE ;

    static const int PLATFORM_VERSION_LEN = 20;

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
        uint32_t om_ir_version = 0;
        uint8_t platform_version[PLATFORM_VERSION_LEN] = { 0 };
        uint8_t platform_type = { 0 };
        uint8_t reserved[MODEL_FILE_RESERVED_LENGTH] = { 0 };   /**< 保留字段 79 长度 */
    };

    #define TARGET_TYPE_LTTE_8BIT 0
    #define TARGET_TYPE_MINI_8BIT 1
    #define TARGET_TYPE_TINY_8BIT 2

    #define PARTITION_TYPE_MODEL_DEF 0
    #define PARTITION_TYPE_WEIGHTS 1
    #define PARTITION_TYPE_TASK_INFO 2

    // 当前模型partition数目
    #define PARTITION_SIZE 3

    #define SIZE_OF_MODEL_PARTITION_TABLE(table) (sizeof(ModelPartitionTable) + sizeof(ModelPartitionMemInfo) * (table).num)

    enum ModelPartitionType
    {
        MODEL_DEF = 0,
        WEIGHTS_DATA,
        TASK_INFO
    };

    struct ModelPartitionMemInfo
    {
        ModelPartitionType type;
        uint32_t mem_offset;
        uint32_t mem_size;
    };

    struct ModelPartitionTable
    {
        uint32_t num;
        ModelPartitionMemInfo partition[0];
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
        DOMI_TENSOR_FILTER_HWCK,     /* filter input tensor format */
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
        DOMI_ACTIVATION_RELU1,            /**< relu1 */
        DOMI_ACTIVATION_SOFTSIGN,         /**< softsign */
        DOMI_ACTIVATION_SOFTPLUS,         /**< softplus */
        DOMI_ACTIVATION_HARDSIGMOID,      /**< hardsigmoid*/
        DOMI_ACTIVATION_THRESHOLD_RELU,   /**< threshold */
        DOMI_ACTIVATION_SELU,             /**< selu */
        DOMI_ACTIVATION_LINEAR,           /**< linear */
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
        DOMI_CONVOLUTION_FWD_ALGO_GEMM_ACCU_FLOAT32, //accumulate in L0c with FP32
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
        DOMI_CONV_MODE_DEPTHWISE,             /**< depthwise convolution*/
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
        DOMI_SOFTMAX_ACCURATE_FP32,
        DOMI_SOFTMAX_RESERVED
    } domiSoftmaxAlgo_t;

    /**
    * @ingroup domi
    * @brief algorithm of convolution backward
    */
   typedef enum tagDomiConvolutionBwdAlgo
    {
        DOMI_CONVOLUTION_BWD_ALGO_GEMM = 0,   /**< matrix gemm algo */
        DOMI_CONVOLUTION_BWD_ALGO_WINOGRAD,   /**< Winograd Transform algo */
        DOMI_CONVOLUTION_BWD_ALGO_RESERVED
    } domiConvolutionBwdAlgo_t;

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
        DOMI_DATA_STRING,            /**< string type */
        DOMI_DATA_RESOURCE,            /**< resource type */
        DOMI_DATA_INT64_REF = 101,   /**< int64 ref */
        DOMI_DATA_INT32_REF = 103,   /**< int64 ref */
        DOMI_DATA_FLOAT_REF = 109,   /**< float ref */
        DOMI_DATA_VARIANT,           /**< variant type */
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
        DOMI_POOLING_L2,                 /**< L2 pooling */
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
        * @brief mode of cropandresize
        */
        typedef enum tagDomiCropAndResizeMode
        {
                DOMI_RESIZE_METHOD_BILINEAR = 0,             /**< resize bilinear */
                DOMI_RESIZE_METHOD_NEAREST,                 /**< resize nearest */
                DOMI_RESIZE_RESERVED
        } domiCropAndResizeMode_t;


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

	typedef enum tagDomiPoolingCeilMode
    {
	DOMI_POOLING_FLOOR = 0,
        DOMI_POOLING_CEIL
    } domiPoolingCeilMode_t;

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

    typedef enum tagDomiResizeBilinearMode
    {
        DOMI_RESIZE_OUTPUT_DIM_BY_ZOOM_FACTOR = 0,             /**< Output dimension specified by zoom factor*/
        DOMI_RESIZE_OUTPUT_DIM_BY_SHRINK_FACTOR,               /**< specified by shrink factor */
        DOMI_RESIZE_OUTPUT_DIM_EXPLICIT,                        /**< specified explicitly */
        DOMI_RESIZE_OUTPUT_DIM_RESERVED
    } domiResizeOutputDimMode_t;

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

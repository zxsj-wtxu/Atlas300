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
#ifndef DOMI_OMG_COMMON_ATTR_DEFINE_H_
#define DOMI_OMG_COMMON_ATTR_DEFINE_H_

#include <string>

namespace domi
{
    /**
    * 公共属性
    */
    static const std::string ATTR_NAME_ALPHA = "alpha";

    static const std::string ATTR_NAME_BETA = "beta";

    static const std::string ATTR_NAME_PADMODE = "padmode";

    static const std::string ATTR_NAME_MODE = "mode";

    static const std::string ATTR_NAME_FILTER = "filter";

    static const std::string ATTR_NAME_BIAS = "bias";

    static const std::string ATTR_NAME_BIAS_TERM = "bias_term";

    static const std::string ATTR_NAME_PAD = "pad";

    static const std::string ATTR_NAME_ALGO = "algo";

    static const std::string ATTR_NAME_FORMAT = "format";

    static const std::string ATTR_NAME_FILTER_FORMAT = "filter_format";

    static const std::string ATTR_NAME_LRN_K = "lrn_k";

    static const std::string ATTR_NAME_LRN_NORM_REGION = "lrn_normregion";

    static const std::string ATTR_NAME_LRN_LOCAL_SIZE = "lrn_localsize";

    static const std::string ATTR_NAME_LRN_ALPHA = "lrn_alpha";

    static const std::string ATTR_NAME_LRN_BETA = "lrn_beta";

    static const std::string ATTR_NAME_AXIS = "axis";
    static const std::string ATTR_NAME_BROADCAST = "broadcast";

    static const std::string ATTR_NAME_OUTPUT_NUM = "output_num";
    static const std::string ATTR_NAME_TIDX = "t_idx";

    static const std::string ATTR_NAME_TPADDINGS = "t_paddings";
    static const std::string ATTR_IMG_H = "img_h";
    static const std::string ATTR_IMG_W = "img_w";
    static const std::string ATTR_NET_H = "net_h";
    static const std::string ATTR_NET_W = "net_w";

    static const std::string ATTR_NAME_TMULTIPLES = "t_multiples";

    static const std::string ATTR_NAME_MULTIPLES = "multiples";

    static const std::string ATTR_NAME_T = "T";

    static const std::string ATTR_NAME_TSHAPE = "Tshape";
    static const std::string ATTR_NAME_NAN_OPT = "nan_opt";

    static const std::string ATTR_NAME_AIPP = "aipp";

    static const std::string ATTR_NAME_INPUT_FORMAT = "input_format";
    static const std::string ATTR_NAME_OUTPUT_FORMAT = "output_format";

    static const std::string ATTR_NAME_FRAMEWORK_NODE_DEF = "node_def";
    static const std::string ATTR_NAME_FRAMEWORK_OP_DEF = "op_def";
    static const std::string ATTR_NAME_FRAMEWORK_FWK_TYPE = "framework_type";
    static const std::string ATTR_NAME_FRAMEWORK_FUNC_DEF = "func_def";


    static const std::string ATTR_NAME_INPUT_TENSOR_DESC = "input_tensor_desc";
    static const std::string ATTR_NAME_OUTPUT_TENSOR_DESC = "output_tensor_desc";

    static const std::string ATTR_NAME_INFERRED_FORMAT = "inferred_format";
    static const std::string ATTR_NAME_PRED_PERMUTE_DELETED = "pred_permute_deleted";
    static const std::string ATTR_NAME_IGNORE_PRED_FORMAT = "ignore_pred_format";

    /* to be deleted*/
    static const std::string ATTR_TO_BE_DELETED = "to_be_deleted";
    static const std::string PERMUTE_RESHAPE_FUSION = "permute_reshape_fusion";
    static const std::string PERMUTE_RESHAPE_FUSION_CONV_PROPOSAL = "fusion_conv_proposal";
    static const std::string PERMUTE_RESHAPE_FUSION_CONV_DECODEBBOX = "fusion_conv_decodebbox";
    static const std::string PERMUTE_RESHAPE_FUSION_BOX_TYPE_NUM = "box_type_num";
    static const std::string SSD_MBOX_LOC_FUSION = "permute_flatten_fusion";
    static const std::string SSD_MBOX_CONF_FUSION = "permute_flatten_reshape_flatten_fusion";
    static const std::string SSD_MBOX_FUSION_BOX_TYPE_NUM = "ssd_mbox_fusion_box_type_num";
    static const std::string SSD_RESHAPE_SLICE_CONCAT_FUSION = "reshape_slice_concat_fusion";

    static const std::string SSD_PRIORBOX_CONCAT = "ssd_mbox_conf_priorbox_concat_flag";


    /* _Arg */
    static const std::string ATTR_NAME_INDEX = "index";
    /* _RetVal */
    static const std::string RETVAL_ATTR_NAME_INDEX = "retval_index";
	/*Data*/
	static const std::string DATA_ATTR_NAME_DATA_TYPE = "data_type";
    /**
    * convolution
    */
    static const std::string ATTR_NAME_STRIDE = "stride";

    static const std::string ATTR_NAME_DILATION = "dilation";

    static const std::string ATTR_NAME_DILATIONS = "dilations";

    static const std::string CONV_ATTR_NAME_GROUP = "group";

    /* Concat */
    static const std::string CONCAT_ATTR_NAME_AXIS = "axis";

    /* Const */
    static const std::string CONST_ATTR_NAME_OUTPUT_FORMAT = "output_format";
    static const std::string CONST_ATTR_NAME_OUTPUT_TYPE = "output_type";

     /* roipooling */
    static const std::string ROIPOOLING_ATTR_NAME_POOLED_H = "pooled_h";
    static const std::string ROIPOOLING_ATTR_NAME_POOLED_W = "pooled_w";
    static const std::string ROIPOOLING_ATTR_NAME_SPATIAL_SCALE = "spatial_scale";
    static const std::string ROIPOOLING_ATTR_NAME_RIO_POOLING_MODE = "rio_pooling_mode";
    static const std::string ROIPOOLING_ATTR_NAME_POOLING_MODE = "pooling_mode";
    static const std::string ROIPOOLING_ATTR_NAME_SAMPLING_RATIO = "sampling_ratio";

    /* DetectionOutput */
    static const std::string DETECTIONOUTPUT_ATTR_NUM_CLASSES = "num_classes";
    static const std::string DETECTIONOUTPUT_ATTR_NMS_THRESHOLD = "nms_threshold";
    static const std::string DETECTIONOUTPUT_ATTR_TOP_K = "top_k";
    static const std::string DETECTIONOUTPUT_ATTR_CONFIDENCE_THRESHOLD = "confidence_threshold";
    static const std::string DETECTIONOUTPUT_ATTR_IMG_H = "img_h";
    static const std::string DETECTIONOUTPUT_ATTR_IMG_W = "img_w";
    static const std::string DETECTIONOUTPUT_ATTR_BATCH_SIZE = "batch_size";
    /* Ssd DetectionOutput */
    static const std::string DETECTIONOUTPUT_ATTR_ETA = "eta";
    static const std::string DETECTIONOUTPUT_ATTR_SHARED_LOCATION = "shared_location";
    static const std::string DETECTIONOUTPUT_ATTR_BACKGROUND_LABEL_ID = "background_label_id";
    static const std::string DETECTIONOUTPUT_ATTR_CODE_TYPE = "code_type";
    static const std::string DETECTIONOUTPUT_ATTR_VARIANCE_ENCODED_IN_TARGET = "variance_encoded_in_target";
    static const std::string DETECTIONOUTPUT_ATTR_KEEP_TOP_K = "keep_top_k";
    /* yolo DetectionOutput */
    static const std::string DETECTIONOUTPUT_ATTR_ClASSES = "classes";
    static const std::string DETECTIONOUTPUT_ATTR_BIASES = "biases";
    static const std::string DETECTIONOUTPUT_ATTR_RELATIVE = "relative";
    static const std::string DETECTIONOUTPUT_ATTR_OBJECTNESS_THRESHOLD = "objectness_threshold";
    static const std::string DETECTIONOUTPUT_ATTR_CLASS_THRESHOLD = "class_threshold";
    static const std::string DETECTIONOUTPUT_ATTR_POST_TOP_K = "post_top_k";
    static const std::string DETECTIONOUTPUT_ATTR_IOU_THRESHOLD_DECAY = "iou_threshold_decay";
    static const std::string DETECTIONOUTPUT_ATTR_COOR_SCALE_FACTOR = "coor_scale_factor";
    static const std::string DETECTIONOUTPUT_ATTR_YOLO_VERSION = "yolo_version";

    /* DetectionPostprocess */
    static const std::string POSTPROCESS_ATTR_NAME_CLS_NUM = "cls_num";
    static const std::string POSTPROCESS_ATTR_NAME_CONF_THRESH = "conf_thresh";
    static const std::string POSTPROCESS_ATTR_NAME_NMS_THRESH = "nms_thresh";
    static const std::string POSTPROCESS_ATTR_POST_NMS_TOPN = "post_nms_topn";
    static const std::string POSTPROCESS_ATTR_NAME_BBOX_REG_WEIGHT = "bbox_reg_weights";

    /* Spatialtransfrom */
    static const std::string SPTIALTF_ATTR_NAME_OUTPUT_H = "output_h";
    static const std::string SPTIALTF_ATTR_NAME_OUTPUT_W = "output_w";
    static const std::string SPTIALTF_ATTR_NAME_BORDER_VALUE = "border_value";
    static const std::string SPTIALTF_ATTR_NAME_AFFINE_TRANSFORM = "affine_transform";

    /* Proposal */
    static const std::string PROPOSAL_ATTR_NAME_FEAT_STRIDE = "feat_stride";
    static const std::string PROPOSAL_ATTR_NAME_BASE_SIZE = "base_size";
    static const std::string PROPOSAL_ATTR_NAME_MIN_SIZE = "min_size";
    static const std::string PROPOSAL_ATTR_NAME_RATIO = "ratio";
    static const std::string PROPOSAL_ATTR_NAME_SCALE = "scale";
    static const std::string PROPOSAL_ATTR_NAME_PRE_NMS_TOPN = "pre_nms_topn";
    static const std::string PROPOSAL_ATTR_NAME_POST_NMS_TOPN = "post_nms_topn";
    static const std::string PROPOSAL_ATTR_NAME_NMS_THRESH = "nms_thresh";
    static const std::string PROPOSAL_ATTR_NAME_TOP_SIZE = "top_size";
    static const std::string PROPOSAL_ATTR_IMG_H = "img_h";
    static const std::string PROPOSAL_ATTR_IMG_W = "img_w";
    /* Softmax */
    static const std::string SOFTMAX_ATTR_AXIS = "axis";

    /* Permute */
    static const std::string PERMUTE_ATTR_ORDER = "order";

    /*SSD Normalize*/
    static const std::string SSDNORMALIZE_ATTR_ACCROSS_SPATIAL = "across_spatial";
    static const std::string SSDNORMALIZE_ATTR_CHANNEL_SHARED = "channel_shared";
    static const std::string SSDNORMALIZE_ATTR_EPS = "eps";

    /* Flatten */
    static const std::string FLATTEN_ATTR_AXIS = "axis";
    static const std::string FLATTEN_ATTR_END_AXIS = "end_axis";

    /* SsdPRIORBOX */
    static const std::string SSD_PRIOR_BOX_ATTR_FLIP = "flip";
    static const std::string SSD_PRIOR_BOX_ATTR_CLIP = "clip";
    static const std::string SSD_PRIOR_BOX_ATTR_IMG_H = "img_h";
    static const std::string SSD_PRIOR_BOX_ATTR_IMG_W = "img_w";
    static const std::string SSD_PRIOR_BOX_ATTR_STEP_H = "step_h";
    static const std::string SSD_PRIOR_BOX_ATTR_STEP_W = "step_w";
    static const std::string SSD_PRIOR_BOX_ATTR_OFFSET = "offset";
    static const std::string SSD_PRIOR_BOX_ATTR_MIN_SIZE = "min_size";
    static const std::string SSD_PRIOR_BOX_ATTR_MAX_SIZE = "max_size";
    static const std::string SSD_PRIOR_BOX_ATTR_MIN_SIZE_NUM = "min_size_num";
    static const std::string SSD_PRIOR_BOX_ATTR_MAX_SIZE_NUM = "max_size_num";
    static const std::string SSD_PRIOR_BOX_ATTR_ASPECT_RATIO = "aspect_ratio";
    static const std::string SSD_PRIOR_BOX_ATTR_ASPECT_RATIO_NUM = "aspect_ratio_num";
    static const std::string SSD_PRIOR_BOX_ATTR_VARIANCE = "variance";
    static const std::string SSD_PRIOR_BOX_ATTR_VARIANCE_NUM = "variance_num";

    /* PRelu */
    static const std::string PRELU_ATTR_CHANNEL_SHARED = "channel_shared";

    /*psroi pooling*/
    static const std::string PSROIPOOLING_ATTR_SPATIAL_SCALE = "spatial_scale";
    static const std::string PSROIPOOLING_ATTR_OUTPUT_DIM = "output_dim";
    static const std::string PSROIPOOLING_ATTR_GROUP_SIZE = "group_size";

    /* power */
    static const std::string POWER_ATTR_NAME_POWER = "power";
    static const std::string POWER_ATTR_NAME_SCALE = "scale";
    static const std::string POWER_ATTR_NAME_SHIFT = "shift";

    /*pack*/
    static const std::string PACK_ATTR_NAME_NUM = "N";

    /*unpack*/
    static const std::string UNPACK_ATTR_NAME_NUM = "num";
    /*gathernd*/
    static const std::string GATHERND_ATTR_NAME_TINDICES = "Tindices";
    static const std::string GATHERND_ATTR_NAME_TPARAMS = "Tparams";

     /*argmax*/
    static const std::string ARGMAX_ATTR_NAME_TOPK = "topk";
    static const std::string ARGMAX_ATTR_NAME_OUTMAX = "outmaxval";

    /* relu */
    static const std::string ATTR_NAME_NEGATIVE_SLOPE = "negative_slope";

    /* FreeSpaceExtract */
    static const std::string FREESPACEEXTRACT_ATTR_NAME_ORG_HEIGHT = "org_height";

    /* split */
    static const std::string SPLIT_ATTR_NAME_SLICE_POINT = "slice_point";
    static const std::string SPLIT_ATTR_NAME_SIZE_SPLIT = "size_split";
    static const std::string SPLIT_ATTR_NAME_NUM_SPLIT = "num_split";

    /* Tvm */
    static const std::string TVM_ATTR_NAME_MAGIC = "tvm_magic";
    static const std::string TVM_ATTR_NAME_BLOCKDIM = "tvm_blockdim";
    static const std::string TVM_ATTR_NAME_METADATA = "tvm_metadata";

    /*squeeze*/
    static const std::string SQUEEZE_ATTR_AXIS = "axis";
    static const std::string SQUEEZE_ATTR_DIMS = "squeeze_dims";
    static const std::string SQUEEZE_OP_NAME = "Squeeze";

    /*stride slice*/
    static const std::string STRIDE_SLICE_ATTR_BEGIN_MASK = "begin_mask";
    static const std::string STRIDE_SLICE_ATTR_END_MASK = "end_mask";
    static const std::string STRIDE_SLICE_ATTR_ELLIPSIS_MASK = "ellipsis_mask";
    static const std::string STRIDE_SLICE_ATTR_NEW_AXIS_MASK = "new_axis_mask";
    static const std::string STRIDE_SLICE_ATTR_SHRINK_AXIS_MASK = "shrink_axis_mask";

    /*slice*/
    static const std::string SLICE_ATTR_NAME_BEGINS = "begins";
    static const std::string SLICE_ATTR_NAME_SIZES = "sizes";

    /*batchnorm*/
    static const std::string BATCHNORM_ATTR_EPSILON = "epsilon";

    /*roialign*/
    static const std::string ROIALIGN_ATTR_SPATIAL_SCALE = "spatial_scale";
    static const std::string ROIALIGN_ATTR_SAMPLING_RATIO = "sampling_ratio";
    static const std::string ROIALIGN_ATTR_NAME_POOLED_H = "pooled_h";
    static const std::string ROIALIGN_ATTR_NAME_POOLED_W = "pooled_w";

    /*generate_rpn_proposal*/
    static const std::string GENERATE_RPN_PROPOSAL_ATTR_PRE_NMS_TOPK = "pre_nms_topk";
    static const std::string GENERATE_RPN_PROPOSAL_ATTR_POST_NMS_TOPK = "post_nms_topk";
    static const std::string GENERATE_RPN_PROPOSAL_ATTR_RPN_MINI_SIZE = "rpn_mini_size";
    static const std::string GENERATE_RPN_PROPOSAL_ATTR_RPN_PROPOSAL_NMS_THRESH= "rpn_proposal_nms_thresh";

    /*decode_bbox*/
    static const std::string DECODE_BBOX_ATTR_DECODECLIP = "decodeClip";

    /* Cast */
    static const std::string CAST_ATTR_DSTT = "DstT";
    static const std::string CAST_ATTR_SRCT = "SrcT";

    /* fastrcnnn predications*/
    static const std::string FASTRCNN_PREDICTIONS_ATTR_TOPK = "fsr_topk";
    static const std::string FASTRCNN_PREDICTIONS_ATTR_SCORE_THRESHOLD = "fsr_score_thres";
    static const std::string FASTRCNN_PREDICTIONS_ATTR_NMS_THRESHOLD = "fsr_nms_thres";
    static const std::string FASTRCNN_PREDICTIONS_ATTR_NUM_CLASSES = "fsr_num_classes";

    /* REORG*/
    static const std::string REORG_ATTR_STRIDE = "stride";
    static const std::string REORG_ATTR_REVERSE = "reverse";

    /* MERGE*/
    static const std::string MERGE_DEAD_INDEX = "merge_dead_index";
    static const std::string MERGE_PRENODE_FLAG = "merge_prenode_flag";
    static const std::string TO_BE_OUTPUT = "to_be_output";

    /*Concatv2*/
    static const std::string CONCAT_V2_ATTR_TIDX = "Tidx";
    static const std::string CONCAT_V2_ATTR_N = "N";
      /* SUM*/
    static const std::string SUM_ATTR_TIDX = "Tidx";
    static const std::string SUM_ATTR_AXIS = "axis";
    static const std::string SUM_ATTR_KEEP_DIMS = "keep_dims";

    /*ResizeBilinear*/
    static const std::string RESIZE_BILINEAR_ATTR_ALIGN_CORNERS = "align_corners";
    static const std::string RESIZE_BILINEAR_ATTR_HEIGHT = "height";
    static const std::string RESIZE_BILINEAR_ATTR_WIDTH = "width";

    /*MatMul*/
    static const std::string MATMUL_TRANSPOSE_X = "transposeX";
    static const std::string MATMUL_TRANSPOSE_W = "transposeW";

    /*Flatten*/
    static const std::string FLATTEN_START_AXIS = "start_axis";
    static const std::string FLATTEN_END_AXIS = "end_axis";

    /*reshape*/
    static const std::string RESHAPE_ATTR_AXIS = "axis";
    static const std::string RESHAPE_ATTR_NUM_AXES = "num_axes";
    static const std::string RESHAPE_ATTR_FORMAT = "format";
    static const std::string RESHAPE_ATTR_SHAPE = "shape";

    /*frameoworkop*/
    static const std::string TENSORFLOW_IN_DATATYPE = "t_in_datatype";
    static const std::string TENSORFLOW_OUT_DATATYPE = "t_out_datatype";
    static const std::string ATTR_NAME_OUT_N = "out_n";
    static const std::string ATTR_NAME_OUT_C = "out_c";
    static const std::string ATTR_NAME_OUT_H = "out_h";
    static const std::string ATTR_NAME_OUT_W = "out_w";
    static const std::string ATTR_PAD_DEPTH_CONV = "pad_depth_conv";
    static const std::string ATTR_PAD_CONV = "pad_conv";

    static const std::string ATTR_NAME_BEFORE_PAD = "before_pad";
    static const std::string ANN_MEAN_KEEPDIMS = "AnnMeanKeepDims";
    static const std::string PAD_ATTR_PADDINGDS = "paddings";
    static const std::string PAD_ATTR_CONSTANT_VALUE = "padvalue";

    /*Rnn*/
    static const std::string RNN_TENSORFLOW = "rnn_tensorflow";
    static const std::string RNN_MODE_STATIC = "rnn_static";
    static const std::string MUTI_RNN = "multi_rnn";
    static const std::string CELL_MODE = "mode";
    static const std::string LSTM_CELL = "lstm_cell";
    static const std::string GRU_CELL = "gru_cell";
    static const std::string RNN_HT = "ht";
    static const std::string RNN_XT_HT = "xt_ht";
    static const std::string RNN_BATCH_SIZE = "batch_size";

    /*Upsample*/
    static const std::string UPSAMPLE_ATTR_NAME_SCALE = "scale";

    /* filler */
    static const std::string FILLER_TYPE = "filler_type";
    static const std::string FILLER_VALUE = "filler_value";
   
    /*shufflechannel*/
    static const std::string SHUFFLE_CHANNEL_GROUP = "group";
}

#endif

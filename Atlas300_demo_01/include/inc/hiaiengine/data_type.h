/**
* @file data_type.h
*
* Copyright (C) <2018>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef INC_HIAIENGINE_DATA_TYPE_H_
#define INC_HIAIENGINE_DATA_TYPE_H_

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "status.h"
#include "hiaiengine/log.h"
#include "cereal/cereal.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/map.hpp"
#define HIAI_SHARED_PTR_RESET(memory, valuePointer, logText)                        \
                    try {                                                           \
                        (memory).reset((valuePointer));                             \
                    } catch (...) {                                                 \
                        HIAI_ENGINE_LOG(HIAI_MEMORY_EXHAUSTED, logText);            \
                        memory = nullptr;                                           \
                        return;                                                     \
                    }

namespace hiai {
    // message name  <<<<<<=================>>>>>>> message type
    // name:"BatchInfo"                             type:BatchInfo
    // name:"FrameInfo"                             type:FrameInfo
    // name:"IMAGEFORMAT"                           type:IMAGEFORMAT
    // name:"Angle"                                 type:Angle
    // name:"Point2D"                               type:Point2D
    // name:"Point3D"                               type:Point3D
    // name:"ROICube"                               type:ROICube
    // name:"RLECode"                               type:RLECode
    // name:"IDTreeNode"                            type:IDTreeNode
    // name:"IDTreePara"                            type:IDTreePara
    // name:"BatchIDTreePara"                       type:BatchIDTreePara
    // name:"RetrievalResult"                       type:RetrievalResult
    // name:"RetrievalResultTopN"                   type:RetrievalResultTopN
    // name:"RetrievalResultPara"                   type:RetrievalResultPara
    // name:"RawDataBuffer"                         type:RawDataBuffer
    // name:"BatchRawDataBuffer"                    type:BatchRawDataBuffer
    // name:"string"                                type:string

    // name:"vector_uint8_t"                        type:vector<uint8_t>
    // name:"vector_float"                          type:vector<float>

    // name:"ImageData_uint8_t"                     type:ImageData<uint8_t>
    // name:"ImageData_float"                       type:ImageData<float>

    // name:"ImagePara_uint8_t"                     type:ImagePara<uint8_t>
    // name:"ImagePara_float"                       type:ImagePara<float>

    // name:"BatchImagePara_uint8_t"                type:BatchImagePara<uint8_t>
    // name:"BatchImagePara_float"                  type:BatchImagePara<float>

    // name:"Line_uint8_t"                          type:Line<uint8_t>
    // name:"Line_float"                            type:Line<float>

    // name:"Rectangle_uint8_t"                     type:Rectangle<uint8_t>
    // name:"Rectangle_float"                       type:Rectangle<float>

    // name:"Polygon_uint8_t"                       type:Polygon<uint8_t>
    // name:"Polygon_float"                         type:Polygon<float>

    // name:"MaskMatrix_uint8_t"                    type:MaskMatrix<uint8_t>
    // name:"MaskMatrix_float"                      type:MaskMatrix<float>

    // name:"ObjectLocation_uint8_t_uint8_t"        type:ObjectLocation<uint8_t, uint8_t>
    // name:"ObjectLocation_float_float"            type:ObjectLocation<float, uint8_t>

    // name:"DetectedObjectPara_uint8_t_uint8_t"    type:DetectedObjectPara<uint8_t, uint8_t>
    // name:"DetectedObjectPara_float_float"        type:DetectedObjectPara<float, float>

    // name:"BatchDetectedObjectPara_uint8_t_uint8_t"               type:BatchDetectedObjectPara<uint8_t,uint8_t>
    // name:"BatchDetectedObjectPara_float_float"                   type:BatchDetectedObjectPara<float,float>
    // name:"BatchDetectedObjectPara_Rectangle_Point2D_int32_t"     type:BatchDetectedObjectPara<Rectangle<Point2D>, int32_t>
    // name:"BatchDetectedObjectPara_Rectangle_Point2D_float"       type:BatchDetectedObjectPara<Rectangle<Point2D>, float>

    // name:"RegionImage_uint8_t"                   type:RegionImage<uint8_t>
    // name:"RegionImage_float"                     type:RegionImage<float>

    // name:"RegionImagePara_uint8_t"               type:RegionImagePara<uint8_t>
    // name:"RegionImagePara_float"                 type:RegionImagePara<float>

    // name:"BatchRegionImagePara_uint8_t"          type:BatchRegionImagePara<uint8_t>
    // name:"BatchRegionImagePara_float"            type:BatchRegionImagePara<float>

    // name:"Attribute_uint8_t"                     type:Attribute<uint8_t>
    // name:"Attribute_float"                       type:Attribute<float>

    // name:"AttributeTopN_uint8_t"                 type:AttributeTopN<uint8_t>
    // name:"AttributeTopN_float"                   type:AttributeTopN<float>

    // name:"AttributeVec_uint8_t"                  type:AttributeVec<uint8_t>
    // name:"AttributeVec_float"                    type:AttributeVec<float>

    // name:"AttributeResultPara_uint8_t"           type: AttributeVec<uint8_t>
    // name:"AttributeResultPara_float"             type:AttributeVec<float>

    // name:"BatchAttributeResultPara_uint8_t"      type:AttributeVec<uint8_t>
    // name:"BatchAttributeResultPara_float"        type:AttributeVec<float>

    // name:"Feature_uint8_t"                       type:AttributeVec<uint8_t>
    // name:"Feature_float"                         type:AttributeVec<float>

    // name:"FeatureList_uint8_t"                   type:FeatureList<uint8_t>
    // name:"FeatureList_float"                     type:FeatureList<float>

    // name:"FeatureVec_uint8_t"                    type:FeatureVec<uint8_t>
    // name:"FeatureVec_float"                      type:FeatureVec<float>

    // name:"FeatureResultPara_uint8_t"             type:FeatureResultPara<uint8_t>
    // name:"FeatureResultPara_float"               type:FeatureResultPara<float>

    // name:"BatchFeatureResultPara_uint8_t"        type:BatchFeatureResultPara<uint8_t>
    // name:"BatchFeatureResultPara_float"          type:BatchFeatureResultPara<float>

    // name:"FeatureRecord_uint8_t"                 type:FeatureRecord<uint8_t>
    // name:"FeatureRecord_float"                   type:FeatureRecord<float>

    // name:"RetrievalSet_uint8_t_uint8_t"          type:RetrievalSet<uint8_t, uint8_t>
    // name:"RetrievalSet_float_float"              type:RetrievalSet<float, float>

    // name:"EvaluationResult_uint8_t"              type:EvaluationResult<uint8_t>
    // name:"EvaluationResult_float"                type:EvaluationResult<float>

    // name:"EvaluationResultVec_uint8_t"           type:EvaluationResultVec<uint8_t>
    // name:"EvaluationResultVec_float"             type:EvaluationResultVec<float>

    // name:"EvaluationResultPara_uint8_t"          type:EvaluationResultPara<uint8_t>
    // name:"EvaluationResultPara_float"            type:EvaluationResultPara<float>

    // name:"BatchEvaluationResultPara_uint8_t"     type:BatchEvaluationResultPara<uint8_t>
    // name:"BatchEvaluationResultPara_float"       type:BatchEvaluationResultPara<float>

    // name:"Classification_uint8_t"                type:Classification<uint8_t>
    // name:"Classification_float"                  type:Classification<float>

    // name:"ClassificationTopN_uint8_t"            type:ClassificationTopN<uint8_t>
    // name:"ClassificationTopN_float"              type:ClassificationTopN<float>

    // name:"ClassificationVec_uint8_t"             type:ClassificationVec<uint8_t>
    // name:"ClassificationVec_float"               type:ClassificationVec<float>

    // name:"ClassificationResultPara_uint8_t"      type:ClassificationResultPara<uint8_t>
    // name:"ClassificationResultPara_float"        type:ClassificationResultPara<float>

    // name:"BatchClassificationResultPara_uint8_t" type:BatchClassificationResultPara<uint8_t>
    // name:"BatchClassificationResultPara_float"   type:BatchClassificationResultPara<float>

    //////////////////////////////////////////////////////
    ///////////////     batch信息     ////////////////////
    //////////////////////////////////////////////////////
    struct BatchInfo {
        bool is_first = false;                    // 是否为第一个batch
        bool is_last = false;                     // 是否为最后一个batch
        uint32_t batch_size = 0;              // 当前batch的实际大小
        uint64_t max_batch_size = 0;          // batch预设大小（最大容量
        uint32_t batch_ID = 0;                // 当前batch ID号
        uint32_t channel_ID = 0;              // 处理当前batch的通道ID号
        uint32_t processor_stream_ID = 0;     // 处理器计算流ID号

        std::vector<uint32_t> frame_ID;        // batch中图像帧ID号
        std::vector<uint32_t> source_ID;       // batch中图像源ID号

        std::vector<uint64_t> timestamp;       // batch中图像的时间戳
    };

    template<class Archive>
    void serialize(Archive& ar, BatchInfo& data) {
        ar(data.is_first, data.is_last, data.batch_size,
            data.max_batch_size, data.batch_ID, data.channel_ID,
            data.processor_stream_ID, data.frame_ID, data.source_ID,
            data.timestamp);
    }

    //////////////////////////////////////////////////////
    ///////////////     frame信息     ////////////////////
    //////////////////////////////////////////////////////
    struct FrameInfo {
        bool is_first = false;                    // 是否为第一个frame
        bool is_last = false;                     // 是否为最后一个frame
        uint32_t channel_ID = 0;              // 处理当前frame的通道ID号
        uint32_t processor_stream_ID = 0;     // 处理器计算流ID号
        uint32_t frame_ID = 0;                // 图像帧ID号
        uint32_t source_ID = 0;               // 图像源ID号
        uint64_t timestamp = 0;               // 图像的时间戳
    };

    template<class Archive>
    void serialize(Archive& ar, FrameInfo& data) {
        ar(data.is_first, data.is_last, data.channel_ID,
            data.processor_stream_ID, data.frame_ID, data.source_ID,
            data.timestamp);
    }

    //////////////////////////////////////////////////////
    //////////////////  1. image tensor ///////////////////
    //////////////////////////////////////////////////////

    // 图像格式
    enum IMAGEFORMAT {
        RGB565,          //  Red 15:11, Green 10:5, Blue 4:0
        BGR565,          //  Blue 15:11, Green 10:5, Red 4:0
        RGB888,          //  Red 24:16, Green 15:8, Blue 7:0
        BGR888,          //  Blue 24:16, Green 15:8, Red 7:0
        BGRA8888,        //  Blue 31:24, Green 23:16, Red 15:8, Alpha 7:0
        ARGB8888,        //  Alpha 31:24, Red 23:16, Green 15:8, Blue 7:0
        RGBX8888,
        XRGB8888,
        YUV420Planar,    //  I420
        YVU420Planar,    //  YV12
        YUV420SP,        //  NV12
        YVU420SP,        //  NV21
        YUV420Packed,    //  YUV420 Interleaved
        YVU420Packed,    //  YVU420 Interleaved
        YUV422Planar,    //  Three arrays Y,U,V.
        YVU422Planar,
        YUYVPacked,      //  422 packed per payload in planar slices
        YVYUPacked,      //  422 packed
        UYVYPacket,      //  422 packed
        VYUYPacket,      //  422 packed
        YUV422SP,        //  422 packed
        YVU422SP,
        YUV444Interleaved,  //  Each pixel contains equal parts YUV
        Y8,
        Y16,
        RAW
    };

    enum OBJECTTYPE {
        OT_VEHICLE,
        OT_HUMAN,
        OT_NON_MOTOR,
        OT_FACE,
        OT_FACE_BODY,
        OT_PLATE
    };

    template<class T>   // T: uint8_t  float
    struct ImageData {
        IMAGEFORMAT format;   // 图像格式

        uint32_t width  = 0;       // 图像宽
        uint32_t height = 0;      // 图像高
        uint32_t channel = 0;     // 图像通道数
        uint32_t depth = 0;       // 位深
        uint32_t height_step = 0;  // 对齐高度
        uint32_t width_step = 0;  // 对齐宽度
        uint32_t size = 0;        // 数据大小（Byte
        std::shared_ptr<T> data;   // 数据指针
    };

    template<class Archive, class T>
    void serialize(Archive& ar, ImageData<T>& data) {
        ar(data.format, data.width, data.height, data.channel,
            data.depth, data.height_step, data.width_step, data.size);
        if (data.size > 0 && data.data.get() == nullptr) {
            HIAI_SHARED_PTR_RESET(data.data, new(std::nothrow) T[data.size],\
                "serialize ImageData reset fail");
        }
        if (data.data.get() != nullptr) {
            ar(cereal::binary_data(data.data.get(), data.size * sizeof(T)));
        }
    }

    // 参数
    template<class T>
    struct ImagePara {
        FrameInfo f_info;          // frame信息
        ImageData<T> img;          // 图像
    };

    template<class Archive, class T>
    void serialize(Archive& ar, ImagePara<T>& data) {
        ar(data.f_info, data.img);
    }

    // 参数
    template<class T>
    struct BatchImagePara {
        BatchInfo b_info;                  // batch信息
        std::vector<ImageData<T> > v_img;   // batch中的图像
    };

    template<class Archive, class T>
    void serialize(Archive& ar, BatchImagePara<T>& data) {
        ar(data.b_info, data.v_img);
    }


    // 角度
    typedef struct {
        float x;
        float y;
        float z;
    }Angle;

    template<class Archive>
    void serialize(Archive& ar, Angle& data) {
        ar(data.x, data.y, data.z);
    }

    // 2D点
    struct Point2D {
        int32_t x;
        int32_t y;
    };

    template<class Archive>
    void serialize(Archive& ar, Point2D& data) {
        ar(data.x, data.y);
    }

    //////////////////////////////////////////////////////
    ///////////////     ROI信息     ////////////////////
    //////////////////////////////////////////////////////

    struct RoiPolygon {
        uint32_t         uiPtNum;       //多边形顶点数
        std::vector<Point2D> astPts;    //多个检索结果（TopN）
    };

    template<class Archive>
    void serialize(Archive &ar, RoiPolygon &data) {
        ar(data.uiPtNum, data.astPts);
    }

    struct ArgsRoiPolygon {
        RoiPolygon stRoiPolygon; // 配置项Roi
        uint32_t iMinFace; // 最小人脸26*26
        uint32_t iMaxFace; // 最大人脸300*300
        uint32_t imgWidth;  // 视频帧图像宽
        uint32_t imgHeight; // 视频帧图像高
    };

    template<class Archive>
    void serialize(Archive &ar, ArgsRoiPolygon &data) {
        ar(data.stRoiPolygon, data.iMinFace, data.iMaxFace, data.imgWidth, data.imgHeight, data.iMinFace);
    }
    // 3D点
    struct Point3D {
        int32_t x;
        int32_t y;
        int32_t z;
    };

    template<class Archive>
    void serialize(Archive& ar, Point3D& data) {
        ar(data.x, data.y, data.z);
    }

    // 2D或3D空间上的线
    template<class T>   // T: Point2D  Point3D
    struct Line {
        T start;
        T end;
    };

    template<class Archive, class T>
    void serialize(Archive& ar, Line<T>& data) {
        ar(data.start, data.end);
    }

    // 2D或3D空间上的矩形平面
    template<class T>  // T: Point2D  Point3D
    struct Rectangle {
        T anchor_lt;
        T anchor_rb;
    };

    template<class Archive, class T>
    void serialize(Archive& ar, Rectangle<T>& data) {
        ar(data.anchor_lt, data.anchor_rb);
    }

    // 2D或3D空间上的多边形平面
    template<class T>  // T: Point2D  Point3D
    struct Polygon {
        std::vector<T> anchors;
    };

    template<class Archive, class T>
    void serialize(Archive& ar, Polygon<T>& data) {
        ar(data.anchors);
    }

    // 立方体
    struct ROICube {
        Point3D anchor;
        uint32_t length;
        uint32_t width;
        uint32_t height;
    };

    template<class Archive>
    void serialize(Archive& ar, ROICube& data) {
        ar(data.anchor, data.length, data.width, data.height);
    }

    // 掩膜矩阵
    template<class T>    // T: int32_t float etc.
    struct MaskMatrix {
        uint32_t cols;
        uint32_t rows;
        std::shared_ptr<T> data;
    };

    template<class Archive, class T>
    void serialize(Archive& ar, MaskMatrix<T>& data) {
        ar(data.cols, data.rows);
        if (data.cols * data.rows > 0 && data.data.get() == nullptr) {
            HIAI_SHARED_PTR_RESET(data.data, new(std::nothrow) T[data.cols * data.rows],\
                "serialize MaskMatrix reset");
        }
        ar(cereal::binary_data(data.data.get(),
            data.cols * data.rows * sizeof(T)));
    }

    // RLE编码
    struct RLECode {
        uint32_t len;
        uint32_t cols;
        uint32_t rows;
        std::shared_ptr<uint32_t> data;
    };

    template<class Archive>
    void serialize(Archive& ar, RLECode& data) {
        ar(data.len, data.cols, data.rows);
        if (data.len > 0 && data.data.get() == nullptr) {
            HIAI_SHARED_PTR_RESET(data.data, new(std::nothrow) uint32_t[data.len],\
                "serialize RLECode reset fail");
        }
        ar(cereal::binary_data(data.data.get(),
            data.len * sizeof(uint32_t)));
    }

    template<class T1, class T2>    // T1: Point2D Rectangle RLECode etc.
    struct ObjectLocation {          // T2: int8_t float etc.
        std::vector<uint32_t> v_obj_id;
        std::vector<T1> range;           // 目标范围描述
        std::vector<Angle> angle;        // 角度信息
        std::vector<T2> confidence;      // 置信度
        std::vector<uint32_t> label;     // 目标类型标签
        std::vector<uint32_t> isInRoi;   // 框是否在ROI区域内
    };

    template<class Archive, class T1, class T2>
    void serialize(Archive& ar, ObjectLocation<T1, T2>& data) {
        ar(data.v_obj_id, data.range, data.angle,
            data.confidence, data.label, data.isInRoi);
    }

    // 参数
    template<class T1, class T2>
    struct DetectedObjectPara {
        FrameInfo f_info;                   // frame信息
        ObjectLocation<T1, T2> location;     // 多个目标位置
    };

    template<class Archive, class T1, class T2>
    void serialize(Archive& ar, DetectedObjectPara<T1, T2>& data) {
        ar(data.f_info, data.location);
    }

    // 参数
    template<class T1, class T2>
    struct BatchDetectedObjectPara {
        //  batch信息
        BatchInfo b_info;
        //  对应每帧图像的多个目标位置
        std::vector<ObjectLocation<T1, T2> > v_location;
    };

    template<class Archive, class T1, class T2>
    void serialize(Archive& ar, BatchDetectedObjectPara<T1, T2>& data) {
        ar(data.b_info, data.v_location);
    }



    ///////////////////////////////////////////////////////
    /////////////// 3. region image tensor ////////////////
    ///////////////////////////////////////////////////////

    template<class T>
    struct RegionImage {
        std::vector<ImageData<T> > region;
        std::vector<uint32_t> v_obj_id;
    };

    template<class Archive, class T>
    void serialize(Archive& ar, RegionImage<T>& data) {
        ar(data.v_obj_id, data.region);
    }

    // 参数
    template<class T>
    struct RegionImagePara {
        FrameInfo f_info;             // frame信息
        RegionImage<T> region;        // 每帧对应的小图
    };

    template<class Archive, class T>
    void serialize(Archive& ar, RegionImagePara<T>& data) {
        ar(data.f_info, data.region);
    }

    // 参数
    template<class T>
    struct BatchRegionImagePara {
        // batch信息
        BatchInfo b_info;
        // 每帧图像若干region image，用一个指向Image数组的指针来引用
        std::vector<RegionImage<T>> v_region;
    };

    template<class Archive, class T>
    void serialize(Archive& ar, BatchRegionImagePara<T>& data) {
        ar(data.b_info, data.v_region);
    }


    // 递归ID树节点
    struct IDTreeNode {
        std::vector<uint32_t>  nodeID;     // 节点上的ID值
        std::vector<bool> is_leaf;         // 是否为叶子节点
        std::vector<IDTreeNode> node;     // 子节点结构指针
    };

    template <class Archive>
    void serialize(Archive& ar, IDTreeNode& data) {
        ar(data.nodeID, data.is_leaf, data.node);
    }

    // 参数
    struct IDTreePara {
        FrameInfo f_info;    // frame信息
        IDTreeNode tree;     // 图像中的ID Tree
    };

    template <class Archive>
    void serialize(Archive& ar, IDTreePara& data) {
        ar(data.f_info, data.tree);
    }

    // 参数
    struct BatchIDTreePara {
        BatchInfo b_info;                   // batch信息
        std::vector<IDTreeNode> v_tree;     // 每帧图像中的ID Tree
    };

    template <class Archive>
    void serialize(Archive& ar, BatchIDTreePara& data) {
        ar(data.b_info, data.v_tree);
    }


    template<class T>
    struct Attribute {
        std::string attr_value;    // ÊôÐÔÈ¡Öµ
        T score;               // ÖÃÐÅ¶È
    };

    template <class Archive, class T>
    void serialize(Archive& ar, Attribute<T>& data) {
        ar(data.attr_value, data.score);
    }

    template<class T>
    struct AttributeTopN {
        uint32_t obj_ID;
        std::map<std::string, std::vector<Attribute<T>> > attr_map;
    };

    template <class Archive, class T>
    void serialize(Archive& ar, AttributeTopN<T>& data) {
        ar(data.obj_ID, data.attr_map);
    }

    template<class T>
    struct AttributeVec {
        std::vector<AttributeTopN<T>> obj_attr;     // ¶à¸öÄ¿±êµÄÊôÐÔ
    };

    template <class Archive, class T>
    void serialize(Archive& ar, AttributeVec<T>& data) {
        ar(data.obj_attr);
    }

    // ²ÎÊý
    template<class T>
    struct AttributeResultPara {
        FrameInfo f_info;         // frameÐÅÏ¢
        AttributeVec<T> attr;     // Í¼ÏñµÄ¶à¸öÄ¿±êµÄÊôÐÔ
    };

    template <class Archive, class T>
    void serialize(Archive& ar, AttributeResultPara<T>& data) {
        ar(data.f_info, data.attr);
    }


    // ²ÎÊý
    template<class T>
    struct BatchAttributeResultPara {
        BatchInfo b_info;                        // batchÐÅÏ¢
        std::vector<AttributeVec<T>> v_attr;
    };

    template <class Archive, class T>
    void serialize(Archive& ar, BatchAttributeResultPara<T>& data) {
        ar(data.b_info, data.v_attr);
    }

    template<class T>
    struct Feature {
        int32_t len;                // 特征向量长度
        std::shared_ptr<T> feature;      // 特征向量指针
    };

    template<class Archive, class T>
    void serialize(Archive& ar, Feature<T>& data) {
        ar(data.len);
        if (data.len > 0 && data.feature.get() == nullptr) {
            HIAI_SHARED_PTR_RESET(data.feature, new(std::nothrow) T[data.len],\
                "serialize Feature reset fail");
        }
        ar(cereal::binary_data(data.feature.get(), data.len * sizeof(T)));
    }


    template<class T>
    struct FeatureList {
        uint32_t obj_ID;                 // 目标ID
        std::vector<Feature<T>> feature_list;    // 目标的多个特征
    };

    template<class Archive, class T>
    void serialize(Archive& ar, FeatureList<T>& data) {
        ar(data.obj_ID, data.feature_list);
    }

    template<class T>
    struct FeatureVec {
        std::vector<FeatureList<T>> obj_feature;   // 多个目标的属性
    };

    template<class Archive, class T>
    void serialize(Archive& ar, FeatureVec<T>& data) {
        ar(data.obj_feature);
    }

    // 参数
    template<class T>
    struct FeatureResultPara {
        FrameInfo f_info;         // frame信息
        FeatureVec<T> feature;     // 图像的多个目标的特征
    };

    template<class Archive, class T>
    void serialize(Archive& ar, FeatureResultPara<T>& data) {
        ar(data.f_info, data.feature);
    }

    // 参数
    template<class T>
    struct BatchFeatureResultPara {
        // batch信息
        BatchInfo b_info;
        // 对应每帧图像的多个目标的特征
        std::vector<FeatureVec<T>> v_feature;
    };

    template<class Archive, class T>
    void serialize(Archive& ar, BatchFeatureResultPara<T>& data) {
        ar(data.b_info, data.v_feature);
    }


    template<class T>
    struct FeatureRecord {
        uint32_t ID;            // 特征图ID
        uint32_t len;           // 特征向量长度
        std::shared_ptr<T> feature;  // 特征向量指针
    };

    template<class Archive, class T>
    void serialize(Archive& ar, FeatureRecord<T>& data) {
        ar(data.ID, data.len);
        if (data.len > 0 && data.feature.get() == nullptr) {
            HIAI_SHARED_PTR_RESET(data.feature, new(std::nothrow) T[data.len],\
                "serialize FeatureRecord reset fail");
        }
        ar(cereal::binary_data(data.feature.get(), data.len * sizeof(T)));
    }

    template<class T1, class T2>
    struct RetrievalSet {
        uint32_t TopN;       // TopN结果设置
        T1 threshold;        // 相似度阈值
        std::vector<FeatureRecord<T2>> record;    // 特征集合中所有的记录
    };

    template<class Archive, class T1, class T2>
    void serialize(Archive& ar, RetrievalSet<T1, T2>& data) {
        ar(data.TopN, data.threshold, data.record);
    }

    // 参数
    template<class T1, class T2>
    struct RetrievalSetPara {
        RetrievalSet<T1, T2> set;   // 参数和特征集合
    };

    template<class Archive, class T1, class T2>
    void serialize(Archive& ar, RetrievalSetPara<T1, T2>& data) {
        ar(data.set);
    }

    struct RetrievalResult {
        uint32_t ID;        // 特征图ID
        float similarity;   // 相似度
    };

    template<class Archive>
    void serialize(Archive& ar, RetrievalResult& data) {
        ar(data.ID, data.similarity);
    }

    struct RetrievalResultTopN {
        std::vector<RetrievalResult> result_list;    // 多个检索结果（TopN）
    };

    template<class Archive>
    void serialize(Archive& ar, RetrievalResultTopN& data) {
        ar(data.result_list);
    }

    // 参数
    struct RetrievalResultPara {
        FrameInfo f_info;               // frame信息
        RetrievalResultTopN result;     // 检索结果
    };

    template<class Archive>
    void serialize(Archive& ar, RetrievalResultPara& data) {
        ar(data.f_info, data.result);
    }

    // 参数
    struct BatchRetrievalResultPara {
        // batch信息
        BatchInfo b_info;
        // 对应每帧图像的检索结果
        std::vector<RetrievalResultTopN> v_result;
    };

    template<class Archive>
    void serialize(Archive& ar, BatchRetrievalResultPara& data) {
        ar(data.b_info, data.v_result);
    }

    template<class T>
    struct EvaluationResult {
        uint32_t obj_ID;           // 目标ID
        std::string description;    // 描述
        T score;                   // 评价分数
    };

    template <class Archive, class T>
    void serialize(Archive& ar, EvaluationResult<T>& data) {
        ar(data.obj_ID, data.description, data.score);
    }

    template<class T>
    struct EvaluationResultVec {
        std::vector<EvaluationResult<T>> result;    // 多个目标的评价结果
    };

    template <class Archive, class T>
    void serialize(Archive& ar, EvaluationResultVec<T>& data) {
        ar(data.result);
    }

    // 参数
    template<class T>
    struct EvaluationResultPara {
        FrameInfo f_info;                 // frame信息
        EvaluationResultVec<T> result;    // 评价结果
    };

    template <class Archive, class T>
    void serialize(Archive& ar, EvaluationResultPara<T>& data) {
        ar(data.f_info, data.result);
    }

    // 参数
    template<class T>
    struct BatchEvaluationResultPara {
        // batch信息
        BatchInfo b_info;
        // 对应每帧图像的评价结果
        std::vector<EvaluationResultVec<T>> v_result;
    };

    template <class Archive, class T>
    void serialize(Archive& ar, BatchEvaluationResultPara<T>& data) {
        ar(data.b_info, data.v_result);
    }


    template<class T>
    struct Classification {
        std::string class_value;    // 类别取值
        T score;                    // 置信度
    };

    template<class Archive, class T>
    void serialize(Archive& ar, Classification<T>& data) {
        ar(data.class_value, data.score);
    }

    template<class T>
    struct ClassificationTopN {
        uint32_t obj_ID;                               // 目标ID
        std::vector<Classification<T>> class_result;   // 分类结果（TopN）
    };

    template<class Archive, class T>
    void serialize(Archive& ar, ClassificationTopN<T>& data) {
        ar(data.obj_ID, data.class_result);
    }

    template<class T>
    struct ClassificationVec {
        std::vector<ClassificationTopN<T>> class_list;   // 多个目标的分类
    };

    template<class Archive, class T>
    void serialize(Archive& ar, ClassificationVec<T>& data) {
        ar(data.class_list);
    }

    // 参数
    template<class T>
    struct ClassificationResultPara {
        FrameInfo f_info;                          // frame信息
        ClassificationVec<T> classification;       // 多个目标的分类
    };

    template<class Archive, class T>
    void serialize(Archive& ar, ClassificationResultPara<T>& data) {
        ar(data.f_info, data.classification);
    }

    // 参数
    template<class T>
    struct BatchClassificationResultPara {
        // batch信息
        BatchInfo b_info;
        // 对应每帧图像的多个目标的分类
        std::vector<ClassificationVec<T>> v_class;
    };


    template<class Archive, class T>
    void serialize(Archive& ar, BatchClassificationResultPara<T>& data) {
        ar(data.b_info, data.v_class);
    }

    struct RawDataBuffer {
        uint32_t len_of_byte;  // size length
        std::shared_ptr<uint8_t> data;  // 一块buffer，可转为用户自定义类型
    };

    template<class Archive>
    void serialize(Archive& ar, RawDataBuffer& data) {
        ar(data.len_of_byte);
        if (data.len_of_byte > 0 && data.data.get() == nullptr) {
            HIAI_SHARED_PTR_RESET(data.data, new(std::nothrow) uint8_t[data.len_of_byte],\
                "serialize RawDataBuffer reset fail");
        }
        ar(cereal::binary_data(data.data.get(), data.len_of_byte *
            sizeof(uint8_t)));
    }

    // common raw databuffer struct
    struct BatchRawDataBuffer {
        BatchInfo b_info;  // batch info
        std::vector<RawDataBuffer> v_info;
    };

    template<class Archive>
    void serialize(Archive& ar, BatchRawDataBuffer& data) {
        ar(data.b_info, data.v_info);
    }


    template<typename T> const char* TypeName(void);

#define REGISTER_TYPE_DEFINITION(type) \
    template<> inline const char* TypeName<type>(void) { return #type; }

    REGISTER_TYPE_DEFINITION(int8_t);
    REGISTER_TYPE_DEFINITION(uint8_t);
    REGISTER_TYPE_DEFINITION(int16_t);
    REGISTER_TYPE_DEFINITION(uint16_t);
    REGISTER_TYPE_DEFINITION(int32_t);
    REGISTER_TYPE_DEFINITION(uint32_t);
    REGISTER_TYPE_DEFINITION(int64_t);
    REGISTER_TYPE_DEFINITION(uint64_t);
    REGISTER_TYPE_DEFINITION(float);
    REGISTER_TYPE_DEFINITION(double);
    REGISTER_TYPE_DEFINITION(Point2D);
    REGISTER_TYPE_DEFINITION(Point3D);
    REGISTER_TYPE_DEFINITION(Line<Point2D>);
    REGISTER_TYPE_DEFINITION(Line<Point3D>);
    REGISTER_TYPE_DEFINITION(Rectangle<Point2D>);
    REGISTER_TYPE_DEFINITION(Rectangle<Point3D>);
    REGISTER_TYPE_DEFINITION(Polygon<Point2D>);
    REGISTER_TYPE_DEFINITION(Polygon<Point3D>);
    REGISTER_TYPE_DEFINITION(ROICube);
    REGISTER_TYPE_DEFINITION(MaskMatrix<int8_t>);
    REGISTER_TYPE_DEFINITION(MaskMatrix<int32_t>);
    REGISTER_TYPE_DEFINITION(MaskMatrix<float>);
    REGISTER_TYPE_DEFINITION(RLECode);
    REGISTER_TYPE_DEFINITION(RoiPolygon);
    REGISTER_TYPE_DEFINITION(ArgsRoiPolygon);
}

#endif  //  INC_HIAIENGINE_DATA_TYPE_H_




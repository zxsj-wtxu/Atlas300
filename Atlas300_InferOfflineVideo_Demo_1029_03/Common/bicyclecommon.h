#ifndef BICYCLECOMMON_H
#define BICYCLECOMMON_H
#include "cereal/types/utility.hpp"
#include "hiaiengine/data_type.h"
#include "hiaiengine/data_type_reg.h"

struct BicycleDetectionResult{
    int type;
    int xmin;
    int ymin;
    int xmax;
    int ymax;
    float confidence;
    BicycleDetectionResult(){
        type = -1;
        xmin = -1;
        ymin = -1;
        xmax = -1;
        ymax = -1;
        confidence = -1;
    };
    BicycleDetectionResult(int _type, int _xmin, int _ymin, int _xmax, int _ymax, float _confidence){
        type = _type;
        xmin = _xmin;
        ymin = _ymin;
        xmax = _xmax;
        ymax = _ymax;
        confidence = _confidence;
    };
};
template <class Archive>
void serialize(Archive& ar, BicycleDetectionResult& data)
{
    ar(data.type, data.xmin, data.ymin, data.xmax, data.ymax, data.confidence);
}

struct SsdResults{
    uint32_t channelid;
    vector<std::shared_ptr<BicycleDetectionResult>> results;
};
template <class Archive>
void serialize(Archive& ar, SsdResults& data)
{
    ar(data.channelid, data.results);
}

HIAI_REGISTER_DATA_TYPE("SsdResults", SsdResults);
HIAI_REGISTER_DATA_TYPE("BicycleDetectionResult", BicycleDetectionResult);

#endif // BICYCLECOMMON_H

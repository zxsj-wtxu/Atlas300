#ifndef POSTSSDRESULTSENGINE_H
#define POSTSSDRESULTSENGINE_H
#include <fstream>
#include "hiaiengine/engine.h"
#include "Common.h"
#include "stream_data.h"
#include "AppCommon.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#define DST_INPUT_SIZE 2
#define DST_OUTPUT_SIZE 1

class PostSsdResultsEngine : public hiai::Engine
{
public:
    HIAI_StatusT Init(const hiai::AIConfig& config, const std::vector<hiai::AIModelDescription>& model_desc);

    HIAI_DEFINE_PROCESS(DST_INPUT_SIZE, DST_OUTPUT_SIZE)
};

#endif // POSTSSDRESULTSENGINE_H

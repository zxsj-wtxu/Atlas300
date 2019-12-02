/**
 * ============================================================================
 *
 * Copyright (C) 2019, Huawei Technologies Co., Ltd. All Rights Reserved.
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

#include "DstEngine.h"
#include <sys/stat.h>

uint32_t OUT_PATH_MAX = 128;
using Stat = struct stat;

const string RESULT_FOLDER = "result_files/";
const string FILE_PRE_FIX = "result_";
const int MAX_CHAR_LENGTH = 256;

// create directory
void MkdirP(const std::string& outdir)
{
    Stat st;
    if (stat(outdir.c_str(), &st) != 0) {
        int dir_err = mkdir(outdir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (dir_err == -1) {
            printf("Error creating directory!\n");
            exit(1);
        }
    }
}

DstEngine::DstEngine():input_que_(DST_INPUT_SIZE, hiai::MultiTypeQueue::MULTI_QUEUE_16_QUE_NUM){

}

HIAI_StatusT DstEngine::Init(const hiai::AIConfig& config, const std::vector<hiai::AIModelDescription>& model_desc) {
    auto aimap = kvmap(config);
    if (aimap.count("labelPath")) {
//        labelPath = aimap["labelPath"];
    }else {
//        labelPath = "./imagenet1000_clsidx_to_labels.txt";
    }
    return HIAI_OK;
}

HIAI_StatusT DstEngine::ProcessResult(int output_port, const std::shared_ptr<DeviceStreamData>& inputArg)
{
    std::shared_ptr<SsdResults> results = std::make_shared<SsdResults>();

    for (const auto& det : inputArg->detectResult) {
        std::shared_ptr<BicycleDetectionResult> outdata = std::make_shared<BicycleDetectionResult>();
        int argmax = det.classifyResult.classIndex;
        float score = det.classifyResult.confidence;
        outdata.get()->type = det.classId;
        outdata.get()->confidence = det.confidence;
        outdata.get()->xmin = det.location.anchor_lt.x;
        outdata.get()->ymin = det.location.anchor_lt.y;
        outdata.get()->xmax = det.location.anchor_rb.x;
        outdata.get()->ymax = det.location.anchor_rb.y;
        results.get()->results.push_back(outdata);
        results.get()->channelid = inputArg.get()->info.channelId;
    }
    SendData(output_port, "SsdResults", static_pointer_cast<void>(results));

    results.get()->results.clear();
    return HIAI_OK;
}

HIAI_StatusT DstEngine::ProcessResult(const std::string& resultFileTxt, const std::shared_ptr<DeviceStreamData>& inputArg)
{
//    uint32_t graph_cur_id = this->GetGraphId();
//    MkdirP(RESULT_FOLDER);
//    string res_folder = RESULT_FOLDER + to_string(graph_cur_id)+"/";
//    MkdirP(res_folder);
//    string resultFile = res_folder + FILE_PRE_FIX + to_string(getCurentTime());
//    string resultFileJpg = resultFile + ".jpg";
//    unsigned char *ptr_uint8 = (unsigned char *)(inputArg->imgOrigin.buf.data.get());
//    vector<unsigned char> buff(ptr_uint8, ptr_uint8 + inputArg->imgOrigin.buf.len_of_byte);
//    cv::Mat image = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
//    int i=0;
    std::shared_ptr<SsdResults> results = std::make_shared<SsdResults>();

    for (const auto& det : inputArg->detectResult) {
        std::shared_ptr<BicycleDetectionResult> outdata = std::make_shared<BicycleDetectionResult>();
        int argmax = det.classifyResult.classIndex;
        float score = det.classifyResult.confidence;
        outdata.get()->type = det.classId;
        outdata.get()->confidence = det.confidence;
        outdata.get()->xmin = det.location.anchor_lt.x;
        outdata.get()->ymin = det.location.anchor_lt.y;
        outdata.get()->xmax = det.location.anchor_rb.x;
        outdata.get()->ymax = det.location.anchor_rb.y;
        results.get()->results.push_back(outdata);
        results.get()->channelid = inputArg.get()->info.channelId;
//        cv::rectangle(image, cv::Point(outdata.get()->xmin, outdata.get()->ymin), cv::Point(outdata.get()->xmax, outdata.get()->ymax), cv::Scalar(0, 0, 255), 2);
    }
//    cv::imwrite(resultFileJpg, image);
    SendData(0, "SsdResults", static_pointer_cast<void>(results));

    results.get()->results.clear();
    return HIAI_OK;
}
HIAI_IMPL_ENGINE_PROCESS("DstEngine", DstEngine, DST_INPUT_SIZE)
{
    DEFINE_MULTI_INPUT_ARGS_PUSH(1);
    DEFINE_MULTI_INPUT_ARGS_POP(1);
    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[DstEngine] recv all data");

    std::vector<std::shared_ptr<DeviceStreamData>> devicestreamdata_v;
    devicestreamdata_v.push_back(_multi_input_0);

    static int imageid = 0;
    std::string imagename = "./out/image" + std::to_string(imageid++) +".jpg";
    int size = devicestreamdata_v.size();
    for(int i=0; i<size; i++){
        std::shared_ptr<DeviceStreamData>& temp = devicestreamdata_v[i];
        uint8_t* data = temp.get()->imgOrigin.buf.data.get();
        if(!data) {
            printf("data is null.\n");
            break;
        }
        FILE* fp = fopen(imagename.c_str(), "wb+");
        fwrite(data, 1, temp.get()->imgOrigin.buf.len_of_byte, fp);
        fflush(fp);
        fclose(fp);
    }

    std::shared_ptr<SsdResults> results = std::make_shared<SsdResults>();
    std::shared_ptr<BicycleDetectionResult> outdata = std::make_shared<BicycleDetectionResult>();
    int argmax = 0;
    float score = 0;
    outdata.get()->type = 0;
    outdata.get()->confidence = 0;
    outdata.get()->xmin = 0;
    outdata.get()->ymin = 0;
    outdata.get()->xmax = 0;
    outdata.get()->ymax = 0;
    results.get()->results.push_back(outdata);
    results.get()->channelid = 0;
    SendData(0, "SsdResults", static_pointer_cast<void>(results));
    //    if(_multi_input_0 && _multi_input_1 && _multi_input_2 && _multi_input_3 &&
//            _multi_input_4 && _multi_input_5 && _multi_input_6 && _multi_input_7 &&
//            _multi_input_8 && _multi_input_9 && _multi_input_11 && _multi_input_12 &&
//            _multi_input_13 && _multi_input_14 && _multi_input_15){

//    }
//    if (arg0){
//        printf("arg0\n");
//        auto inputArg = std::static_pointer_cast<DeviceStreamData>(arg0);
//        if(ProcessResult(0, inputArg) != HIAI_OK){
//            printf("[DstEngine]  process result failed\n");
//            return HIAI_ERROR;
//        }
//    }

    return HIAI_OK;
}

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

HIAI_StatusT DstEngine::Init(const hiai::AIConfig& config, const std::vector<hiai::AIModelDescription>& model_desc) {
    auto aimap = kvmap(config);
    if (aimap.count("labelPath")) {
        labelPath = aimap["labelPath"];
    }else {
        labelPath = "./imagenet1000_clsidx_to_labels.txt";
    }
    return HIAI_OK;
}

HIAI_StatusT DstEngine::SaveJpg(const std::string& resultFileJpg, const std::shared_ptr<DeviceStreamData>& inputArg)
{
    void *ptr = (void *)(inputArg->imgOrigin.buf.data.get());
    unsigned char *ptr_uint8 = (unsigned char *)(inputArg->imgOrigin.buf.data.get());

    vector<unsigned char> buff(ptr_uint8, ptr_uint8 + inputArg->imgOrigin.buf.len_of_byte);
    cv::Mat image = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
    cv::imwrite(resultFileJpg, image);

//    FILE *fp = fopen(resultFileJpg.c_str(), "wb");
//    if (NULL == fp) {
//        HIAI_ENGINE_LOG(HIAI_IDE_INFO, "[SaveFile] Save file engine: open file fail!");
//        return HIAI_ERROR;
//    } else {
//        fwrite(ptr, 1, inputArg->imgOrigin.buf.len_of_byte, fp);
//        fflush(fp);
//        fclose(fp);
//    }
    return HIAI_OK;
}

HIAI_StatusT DstEngine::ProcessResult(const std::string& resultFileTxt, const std::shared_ptr<DeviceStreamData>& inputArg)
{
    string resultFile = RESULT_FOLDER + FILE_PRE_FIX + to_string(imageid++);
    string resultFileJpg = resultFile + ".jpg";
    unsigned char *ptr_uint8 = (unsigned char *)(inputArg->imgOrigin.buf.data.get());
    vector<unsigned char> buff(ptr_uint8, ptr_uint8 + inputArg->imgOrigin.buf.len_of_byte);
    cv::Mat image = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
    int i=0;
    HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "detectResult size:%d", inputArg->detectResult.size());
    for (const auto& det : inputArg->detectResult) {
        int argmax = det.classifyResult.classIndex;
        float score = det.classifyResult.confidence;
        int obj_classId = det.classId;
        float obj_confidence = det.confidence;
        int xmin, ymin, xmax, ymax;
        xmin = det.location.anchor_lt.x;
        ymin = det.location.anchor_lt.y;
        xmax = det.location.anchor_rb.x;
        ymax = det.location.anchor_rb.y;
        cv::rectangle(image, cv::Point(xmin, ymin), cv::Point(xmax, ymax), cv::Scalar(0, 0, 255), 2);
    }
    cv::imwrite(resultFileJpg, image);
    return HIAI_OK;
}
HIAI_IMPL_ENGINE_PROCESS("DstEngine", DstEngine, DST_INPUT_SIZE)
{
//    HIAI_ENGINE_LOG(HIAI_INFO, "[DstEngine] start process!");
    if (arg0 == nullptr){
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[DstEngine]  The input arg0 is nullptr");
        return HIAI_ERROR;
    }
    auto inputArg = std::static_pointer_cast<DeviceStreamData>(arg0);

    // if it is the end of stream, send end signal to main
   if(inputArg->info.isEOS){
        std::shared_ptr<std::string> result_data(new std::string);
        hiai::Engine::SendData(0, "string", std::static_pointer_cast<void>(result_data));
        return HIAI_OK;
   }

   // create directory for saving result info
    MkdirP(RESULT_FOLDER);
    string resultFile = RESULT_FOLDER + FILE_PRE_FIX + to_string(getCurentTime());
    string resultFileTxt = resultFile + ".txt";
    string resultFileJpg = resultFile + ".jpg";

//    printf("[DstEngine] end process, %d\n", count_id++);
    // save the result information in file named resultFileTxt
    if(ProcessResult(resultFileTxt, inputArg) != HIAI_OK){
        printf("[DstEngine]  process result failed\n");
        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[DstEngine]  process result failed");
        return HIAI_ERROR;
    }

    // save the result jpg file named resultFileJpg
//    if(SaveJpg(resultFileJpg,inputArg) != HIAI_OK){
//        printf("[DstEngine]  save jpg file failed\n");
//        HIAI_ENGINE_LOG(HIAI_IDE_ERROR, "[DstEngine]  save jpg file failed");
//        return HIAI_ERROR;
//    }
//    HIAI_ENGINE_LOG(HIAI_INFO, "[DstEngine] end process!");
    return HIAI_OK;
}

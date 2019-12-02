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
#include <atomic>
#include <iostream>
#include <libgen.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <time.h>
#include "DataRecv.h"
#include "hiaiengine/api.h"
#include "DynamicGraph.h"
#include "CommandLine.h"
#include "CommandParser.h"
#include "graphmanager.h"
#include "HttpService.h"
#include "BicycleDetection.h"
#include "hiaiengine/engine.h"
#include "hiaiengine/c_graph.h"
#include "webapi.h"
#include "BoardParams.h"
//#include "BoardParams.h"

static const std::string GRAPH_FILENAME = "./graph.config";
// graph id
static const uint32_t GRAPH_ID = 100;
// destination engine id
static const uint32_t SRC_ENGINE = 101;
// leaf node engine id
static const uint32_t terminators[] = { 106 };
// flag to guard eos signal
static std::atomic<int> g_flag = { 1 };
// sleep time
static const uint32_t USLEEP_TIME = 100000;
// device id
static const uint32_t DEVICE_ID = 0;

dg::DynamicGraph graphs;
std::vector<dg::DynamicGraph> vector_graphs;

void SendEventInfo(MyString& serverip, MyString& serverport, MyString& eventinfo_api, BaseClass& event){
    string send_url = string("http://") + string(serverip.c_str()) + ":" + string(serverport.c_str()) + string(eventinfo_api.c_str());
    string data(event.toJson().c_str());

    HttpService::Instance()->SendPostData(send_url, data);
}

HIAI_StatusT CustomDataRecvInterface::RecvData(const std::shared_ptr<void>& message)
{
    static int history_time[1024] = {0};
    std::shared_ptr<SsdResults> data = std::static_pointer_cast<SsdResults>(message);
    BicycleDetection::run_version_muilt(data, data.get()->channelid);
    unsigned int channelid = data.get()->channelid;

    struct  tm      *ptm;
    struct  timeb   stTimeb;
    ftime(&stTimeb);
    ptm = localtime(&stTimeb.time);
    if((ptm->tm_sec == 59) || 1){
        int current_time = ptm->tm_hour * 60 * 60 + ptm->tm_min * 60 + ptm->tm_sec;
        int diff = current_time - history_time[channelid];
        if(diff >= 1){
            history_time[channelid] = current_time;
            int count1 = BicycleDetection::getCount1Total(channelid);
            int count2 = BicycleDetection::getCount2Total(channelid);
            LOG_DEBUG("Channel id %d, Total count1:%d, Total count2:%d\n",channelid, count1, count2);
        }
    }

    return HIAI_OK;
}

char* log_Time(void)
{
    struct  tm      *ptm;
    struct  timeb   stTimeb;
    static  char    szTime[19];

    ftime(&stTimeb);
    ptm = localtime(&stTimeb.time);
    sprintf(szTime, "%02d-%02d %02d:%02d:%02d.%03d",
            ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm);
    szTime[18] = 0;
    return szTime;
}

void my_handler(int s)
{
    printf("Caught signal %d\n", s);
    if (s == 2) {
        printf("DestroyGraph %u\n", GRAPH_ID);
        DestroyAllGraphs();
//        int graph_size = vector_graphs.size();
//        for(int i=0; i<graph_size; i++){
//            vector_graphs[i].destroyGraph();
//        }
        exit(0);
    }
}

int main(int argc, char* argv[])
{
    CommandParser options;
    options
        .addOption("-h")
        .addOption("-g", "1")     //增加支持的参数选项
        .addOption("-id", "100")      //增加支持的参数选项
        .addOption("-dev", "0");      //增加支持的参数选项

    options.parseArgs(argc, argv);  //将argv参数添加到vector容器中

    bool help = options.cmdOptionExists("-h");  //从vector容器中查找是否输入了-h选项
    int groups = parseStrToInt(options.cmdGetOption("-g"));
    int graphid = parseStrToInt(options.cmdGetOption("-id"));
    int deviceid = parseStrToInt(options.cmdGetOption("-dev"));

    if (groups <= 0) {
        printf("option -g should be greater than or equal to 1.\n");
        showUsage();
        return -1;
    }

    g_flag = groups;
    if (help) {
        showUsage();
        return 0;
    }
    WebServerInit();
    BoardParams::Instance()->initLoadParam("device");

    WebServerStart();
    // cd to directory of main
    char* dirc = strdup(argv[0]);
    if (dirc != NULL) {
        char* dname = ::dirname(dirc);
        int r = chdir(dname);
        if (r != 0) {
            printf("chdir error code %d\n", r);
            return -1;
        }
        free(dirc);
    }

    uint32_t devCount;
    HIAI_GetDeviceNum(&devCount);
    std::cout << "device count:" << devCount <<std::endl;
    // wait for ctrl+c
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    while (g_flag > 0) {
        sleep(5);
    }
    printf("[main] destroy graph-%u done\n", GRAPH_ID);
    DestroyAllGraphs();
    return 0;
}

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
#include "DataRecv.h"
#include "hiaiengine/api.h"
#include "DynamicGraph.h"
#include "CommandLine.h"
#include "CommandParser.h"
#include <atomic>
#include <iostream>
#include <libgen.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <time.h>
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

HIAI_StatusT CustomDataRecvInterface::RecvData(const std::shared_ptr<void>& message)
{
    std::shared_ptr<std::string> data = std::static_pointer_cast<std::string>(message);
    std::cout << "RecvData g_flag " << g_flag << "\n";
    g_flag--;
    return HIAI_OK;
}

// Init and create graph
HIAI_StatusT HIAI_InitAndStartGraph(const std::string& configFile)
{
    // Step1: Global System Initialization before using HIAI Engine
    HIAI_StatusT status = HIAI_Init(0);

    // Step2: Create and Start the Graph
    status = hiai::Graph::CreateGraph(configFile);
    if (status != HIAI_OK) {
        HIAI_ENGINE_LOG(status, "Fail to start graph");
        return status;
    }

    // Step3: Set DataRecv Functor
    std::shared_ptr<hiai::Graph> graph = hiai::Graph::GetInstance(GRAPH_ID);
    if (graph == nullptr) {
        HIAI_ENGINE_LOG("Fail to get the graph-%u", GRAPH_ID);
        return status;
    }

    for (int i = 0; i < sizeof(terminators) / sizeof(uint32_t); i++) {
        hiai::EnginePortID target_port_config;
        target_port_config.graph_id = GRAPH_ID;
        target_port_config.engine_id = terminators[i];
        target_port_config.port_id = 0;
        graph->SetDataRecvFunctor(target_port_config,
            std::make_shared<CustomDataRecvInterface>(""));
    }
    return HIAI_OK;
}

void my_handler(int s)
{
    printf("Caught signal %d\n", s);
    if (s == 2) {
        printf("DestroyGraph %u\n", GRAPH_ID);
        graphs.destroyGraph();
        exit(0);
    }
}
char*   log_Time(void)
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

HIAI_StatusT CreateDynamicGraphs(uint32_t num, int id, dg::DynamicGraph& graphs, std::vector<dg::NodeInfo>& inputNodes,
    std::vector<dg::NodeInfo>& outputNodes)
{
    // init id number
    // create graphs dynamically instead of using graph.config file
    for (int i = 0; i < num; i++) {
        dg::graph g(id++, DEVICE_ID);
        dg::engine e0("StreamPuller", id++, 1, dg::engine::HOST);
        e0.so_name.push_back("./libStreamPuller.so");
        {
            dg::AIConfigItem item;
            item.name = "channel_id";
            item.value= std::to_string(i);
            e0.ai_config.items.push_back(item);
        }
        {
            dg::AIConfigItem item;
            item.name = "stream_name";
            item.value= "/home/HwHiAiUser/test.h264";
            e0.ai_config.items.push_back(item);
        }

        dg::engine e1("VDecEngine", id++, 1, dg::engine::DEVICE);
        e1.so_name.push_back("libVDecEngine.so");

        dg::engine e2("SSDDetection", id++, 1, dg::engine::DEVICE);
        e2.so_name.push_back("libSSDDetection.so");
        {
            dg::AIConfigItem item;
            item.name = "model";
            item.value= "../data/models/vgg_ssd_300x300.om";
            e2.ai_config.items.push_back(item);
        }

        dg::engine e3("JpegEncode", id++, 1, dg::engine::DEVICE);
        e3.so_name.push_back("libJpegEncode.so");
        {
            dg::AIConfigItem item;
            item.name = "init_config";
            item.value= "";
            e3.ai_config.items.push_back(item);
        }
        {
            dg::AIConfigItem item;
            item.name = "passcode";
            item.value= "";
            e3.ai_config.items.push_back(item);
        }

        dg::engine e4("DstEngine", id++, 1, dg::engine::HOST);
        e4.so_name.push_back("libDstEngine.so");
        {
            dg::AIConfigItem item;
            item.name = "labelPath";
            item.value= "./imagenet1000_clsidx_to_labels.txt";
            e4.ai_config.items.push_back(item);
        }

        inputNodes.push_back(std::make_tuple(g, e0, 0));
        outputNodes.push_back(std::make_tuple(g, e4, 0));
        g.addEngine(e0);
        g.addEngine(e1);
        g.addEngine(e2);
        g.addEngine(e3);
        g.addEngine(e4);

        g.addConnection(dg::connection(e0, 0, e1, 0));
        g.addConnection(dg::connection(e1, 0, e2, 0));
        g.addConnection(dg::connection(e2, 0, e3, 0));
        g.addConnection(dg::connection(e3, 0, e4, 0));

        graphs.addGraph(g);
    }
    return HIAI_OK;
}

int main(int argc, char* argv[])
{
    CommandParser options;
    options
        .addOption("-h")
        .addOption("-g", "1")     //增加支持的参数选项
        .addOption("-id", "100");      //增加支持的参数选项

    options.parseArgs(argc, argv);  //将argv参数添加到vector容器中

    bool help = options.cmdOptionExists("-h");  //从vector容器中查找是否输入了-h选项
    int groups = parseStrToInt(options.cmdGetOption("-g"));
    int id = parseStrToInt(options.cmdGetOption("-id"));

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

    std::vector<dg::NodeInfo> inputNodes;
    std::vector<dg::NodeInfo> outputNodes;
    HIAI_StatusT ret;
    ret = CreateDynamicGraphs(groups, id, graphs, inputNodes, outputNodes);
    if (ret != HIAI_OK) {
        printf("CreateDynamicGraphs failed %d\n", ret);
        return -1;
    }
    ret = graphs.createGraph();
    if (ret != HIAI_OK) {
        printf("createGraph failed %d\n", ret);
        return -1;
    }
    for (const auto& node : outputNodes) {
        ret = graphs.setDataRecvFunctor(node, std::make_shared<CustomDataRecvInterface>(""));
        if (ret != HIAI_OK) {
            printf("setDataRecvFunctor failed %d\n", ret);
            return -1;
        }
    }
    // send from main to graph
    for (const auto& node : inputNodes) {
        ret = graphs.sendData(node, "string", std::make_shared<std::string>());
        if (ret != HIAI_OK) {
            printf("sendData failed %d\n", ret);
            return -1;
        }
    }

    // wait for ctrl+c
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    printf("start:[%s]\n", log_Time());
    while (g_flag > 0) {
        usleep(10000);
    }
    printf("end:[%s]\n", log_Time());
    printf("[main] destroy graph-%u done\n", GRAPH_ID);
//    hiai::Graph::DestroyGraph(GRAPH_ID);
    graphs.destroyGraph();

    return 0;
}

#include "graphmanager.h"
#include "DataRecv.h"
#include "hiaiengine/api.h"
#include "DynamicGraph.h"
#include "CommandLine.h"
#include "CommandParser.h"
#include <map>
#include <mutex>
#define DEVICE_COUNT 4
#define CORE_MAX    6
std::mutex deviceid_lock;
uint32_t deviceid_[DEVICE_COUNT] = {0};

std::mutex map_graphs_lock;
std::map<uint32_t, std::shared_ptr<dg::DynamicGraph>> map_graphs;

int CreateDynamicGraph(int graphid, uint32_t deviceid, uint32_t channelid, std::string& sourceurl, dg::DynamicGraph& graphs);
int CreateDynamicGraph2(int graphid, uint32_t deviceid, uint32_t channelid, std::string& sourceurl, dg::DynamicGraph& graphs);

int CreateGraph(int graphid, unsigned int channelid, const char* sourceurl){
    int id = graphid;
    uint32_t deviceid = 0;
    deviceid_lock.lock();
    for(uint32_t i = 0; i<DEVICE_COUNT; i++){
        if(deviceid_[i] < CORE_MAX){
            deviceid_[i]++;
            deviceid = i;
            deviceid_lock.unlock();
            break;
        }else if(i == (DEVICE_COUNT - 1)){
            deviceid_lock.unlock();
            return -1;
        }
    }
    id = CreateGraph(id, deviceid, channelid, sourceurl);

    return id;
}

int CreateGraph(int graphid, unsigned int deviceid, unsigned int channelid, const char* sourceurl){
    int id = graphid;
    std::shared_ptr<dg::DynamicGraph> graph = std::make_shared<dg::DynamicGraph>();
    std::string url(sourceurl);
    id = CreateDynamicGraph2(id, deviceid, channelid, url, *graph);
    map_graphs_lock.lock();
    map_graphs.insert(std::map<uint32_t, std::shared_ptr<dg::DynamicGraph>>::value_type(channelid, graph));
    map_graphs_lock.unlock();
    return id;
}

int DestroyGraph(unsigned int channelid){
    std::map<uint32_t, std::shared_ptr<dg::DynamicGraph>>::iterator it;
    map_graphs_lock.lock();
    it = map_graphs.find(channelid);
    if(it == map_graphs.end()){
        map_graphs_lock.unlock();
        return -1;
    }else{
        std::shared_ptr<dg::DynamicGraph>& temp = it->second;
        uint32_t graphid = temp.get()->getGraphId();
        auto graphPtr = hiai::Graph::GetInstance(graphid);
        uint32_t deviceid = graphPtr->GetDeviceID();
        deviceid_lock.lock();
        deviceid_[deviceid]--;
        deviceid_lock.unlock();
        temp.get()->destroyGraph();
        it = map_graphs.erase(it);
        map_graphs_lock.unlock();
        return 0;
    }
}

int DestroyAllGraphs(){
    std::map<uint32_t, std::shared_ptr<dg::DynamicGraph>>::iterator it;
    for(it = map_graphs.begin(); it != map_graphs.end();){
        std::shared_ptr<dg::DynamicGraph>& temp = it->second;
        temp.get()->destroyGraph();
        it = map_graphs.erase(it);
    }
}
#define CHANNEL_MAX 6
std::map<uint32_t, dg::engine> streamer_engines;
std::map<uint32_t, dg::engine> vdec_engines;
std::map<uint32_t, dg::NodeInfo> input_nodes;

/*
 * 此接口用于一个graph创建多路分析，还未完成
 **/
int InitDynamicGraph(dg::DynamicGraph& graphs){
    HIAI_StatusT ret;
    int id = 100;
    dg::graph g(id++, 0);

    std::vector<dg::engine> engines;
    dg::engine engine_ssd("SSDDetection", id++, 1, dg::engine::DEVICE);
    engine_ssd.so_name.push_back("libSSDDetection.so");
    {
        dg::AIConfigItem item;
        item.name = "model";
        item.value= "../data/models/vgg_ssd_300x300.om";
        engine_ssd.ai_config.items.push_back(item);
    }
    g.addEngine(engine_ssd);

    for(int i=0; i<CHANNEL_MAX; i++){
        {
            dg::engine e("StreamPuller", id++, 1, dg::engine::HOST);
            e.so_name.push_back("./libStreamPuller.so");
            {
                dg::AIConfigItem item;
                item.name = "channel_id";
                item.value= std::to_string(i);
                e.ai_config.items.push_back(item);
            }
            streamer_engines.insert(std::map<uint32_t, dg::engine>::value_type(i, e));
            input_nodes.insert(std::map<uint32_t, dg::NodeInfo>::value_type(i, std::make_tuple(g, e, 0)));
            g.addEngine(e);
        }
        {
            dg::engine e("VDecEngine", id++, 1, dg::engine::DEVICE);
            e.so_name.push_back("libVDecEngine.so");
            vdec_engines.insert(std::map<uint32_t, dg::engine>::value_type(i, e));
            g.addEngine(e);
        }
        g.addConnection(dg::connection(streamer_engines[i], 0, vdec_engines[i], 0));
        g.addConnection(dg::connection(vdec_engines[i], 0, vdec_engines[i], 0));
    }

    graphs.addGraph(g);
}

int CreateDynamicGraph(int graphid, uint32_t deviceid, uint32_t channelid, std::string& sourceurl, dg::DynamicGraph& graphs){
    HIAI_StatusT ret;
    int id = graphid;
    dg::graph g(id++, deviceid);
    dg::engine e0("StreamPuller", id++, 1, dg::engine::HOST);
    e0.so_name.push_back("./libStreamPuller.so");
    {
        dg::AIConfigItem item;
        item.name = "channel_id";
        item.value= std::to_string(channelid);
        e0.ai_config.items.push_back(item);
    }
    {
        dg::AIConfigItem item;
        item.name = "stream_name";
        item.value= sourceurl;
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

    dg::engine e3("PostSsdResultsEngine", id++, 1, dg::engine::DEVICE);
    e3.so_name.push_back("libPostSsdResultsEngine.so");

    dg::engine e4("DstEngine", id++, 1, dg::engine::HOST);
    e4.so_name.push_back("libDstEngine.so");
    {
        dg::AIConfigItem item;
        item.name = "labelPath";
        item.value= "./imagenet1000_clsidx_to_labels.txt";
        e4.ai_config.items.push_back(item);
    }

    dg::engine e5("JpegEncode", id++, 1, dg::engine::DEVICE);
    e5.so_name.push_back("libJpegEncode.so");
    {
        dg::AIConfigItem item;
        item.name = "init_config";
        item.value= "";
        e5.ai_config.items.push_back(item);
    }
    {
        dg::AIConfigItem item;
        item.name = "passcode";
        item.value= "";
        e5.ai_config.items.push_back(item);
    }

    g.addEngine(e0);
    g.addEngine(e1);
    g.addEngine(e2);
    g.addEngine(e3);
    g.addEngine(e4);
//    g.addEngine(e5);

    g.addConnection(dg::connection(e0, 0, e1, 0));
    g.addConnection(dg::connection(e1, 0, e2, 0));
    g.addConnection(dg::connection(e2, 0, e3, 0));
//    g.addConnection(dg::connection(e2, 0, e5, 0));
//    g.addConnection(dg::connection(e5, 0, e3, 0));
    g.addConnection(dg::connection(e3, 0, e4, 0));

    graphs.addGraph(g);

    ret = graphs.createGraph();
    if (ret != HIAI_OK) {
        printf("createGraph failed %d\n", ret);
        return -1;
    }

    dg::NodeInfo inputNode = std::make_tuple(g, e0, 0);
    dg::NodeInfo outputNode= std::make_tuple(g, e4, 0);

    ret = graphs.setDataRecvFunctor(outputNode, std::make_shared<CustomDataRecvInterface>(""));
    if (ret != HIAI_OK) {
        printf("setDataRecvFunctor failed %d\n", ret);
        return -1;
    }
    ret = graphs.sendData(inputNode, "string", std::make_shared<std::string>());
    if (ret != HIAI_OK) {
        printf("sendData failed %d\n", ret);
        return -1;
    }

    return id;
}

int CreateDynamicGraph2(int graphid, uint32_t deviceid, uint32_t channelid, std::string& sourceurl, dg::DynamicGraph& graphs){
    HIAI_StatusT ret;
    int id = graphid;
    dg::graph g(id++, deviceid);
    dg::engine e0("video_decode", id++, 1, dg::engine::HOST);
    e0.so_name.push_back("./libvideo_decode.so");
    {
        dg::AIConfigItem item;
        item.name = "channel1";
        item.value= sourceurl;
        e0.ai_config.items.push_back(item);
    }
    {
        dg::AIConfigItem item;
        item.name = "channel1_id";
        item.value= std::to_string(channelid);
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

    dg::engine e3("PostSsdResultsEngine", id++, 1, dg::engine::DEVICE);
    e3.so_name.push_back("libPostSsdResultsEngine.so");

    dg::engine e4("DstEngine", id++, 1, dg::engine::HOST);
    e4.so_name.push_back("libDstEngine.so");
    {
        dg::AIConfigItem item;
        item.name = "labelPath";
        item.value= "./imagenet1000_clsidx_to_labels.txt";
        e4.ai_config.items.push_back(item);
    }

    dg::engine e5("JpegEncode", id++, 1, dg::engine::DEVICE);
    e5.so_name.push_back("libJpegEncode.so");
    {
        dg::AIConfigItem item;
        item.name = "init_config";
        item.value= "";
        e5.ai_config.items.push_back(item);
    }
    {
        dg::AIConfigItem item;
        item.name = "passcode";
        item.value= "";
        e5.ai_config.items.push_back(item);
    }

    g.addEngine(e0);
    g.addEngine(e1);
    g.addEngine(e2);
    g.addEngine(e3);
    g.addEngine(e4);
    g.addEngine(e5);

    g.addConnection(dg::connection(e0, 0, e1, 0));
    g.addConnection(dg::connection(e1, 0, e2, 0));
//    g.addConnection(dg::connection(e2, 0, e3, 0));
    g.addConnection(dg::connection(e2, 0, e5, 0));
    g.addConnection(dg::connection(e5, 0, e3, 0));
    g.addConnection(dg::connection(e3, 0, e4, 0));

    graphs.addGraph(g);

    ret = graphs.createGraph();
    if (ret != HIAI_OK) {
        printf("createGraph failed %d\n", ret);
        return -1;
    }

    dg::NodeInfo inputNode = std::make_tuple(g, e0, 0);
    dg::NodeInfo outputNode= std::make_tuple(g, e4, 0);

    ret = graphs.setDataRecvFunctor(outputNode, std::make_shared<CustomDataRecvInterface>(""));
    if (ret != HIAI_OK) {
        printf("setDataRecvFunctor failed %d\n", ret);
        return -1;
    }
    ret = graphs.sendData(inputNode, "string", std::make_shared<std::string>());
    if (ret != HIAI_OK) {
        printf("sendData failed %d\n", ret);
        return -1;
    }

    return id;
}

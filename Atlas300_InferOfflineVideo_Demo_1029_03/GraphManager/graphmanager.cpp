#include "graphmanager.h"

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

    dg::engine e4("DstEngine", id++, 1, dg::engine::HOST);
    e4.so_name.push_back("libDstEngine.so");
    {
        dg::AIConfigItem item;
        item.name = "labelPath";
        item.value= "./imagenet1000_clsidx_to_labels.txt";
        e4.ai_config.items.push_back(item);
    }

    g.addEngine(e0);
    g.addEngine(e1);
    g.addEngine(e2);
    g.addEngine(e4);

    g.addConnection(dg::connection(e0, 0, e1, 0));
    g.addConnection(dg::connection(e1, 0, e2, 0));
    g.addConnection(dg::connection(e2, 0, e4, 0));

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

#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H
#ifdef __cplusplus
extern "C"{
#endif
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#ifdef __cplusplus
}
#endif
#include "DataRecv.h"
#include "hiaiengine/api.h"
#include "DynamicGraph.h"
#include "CommandLine.h"
#include "CommandParser.h"
#include <map>
#include <mutex>
int DestroyAllGraphs();
int DestroyGraph(unsigned int channelid);
int CreateGraph(int graphid, unsigned int channelid, const char* sourceurl);
int CreateGraph(int graphid, unsigned int deviceid, unsigned int channelid, const char *sourceurl);
int CreateMuiltEngineGraph(int graphid, unsigned int deviceid, unsigned int channelid, const char* sourceurl);

int CreateDynamicGraph(int graphid, uint32_t deviceid, uint32_t channelid, std::string& sourceurl, dg::DynamicGraph& graphs);
int CreateDynamicGraph2(int graphid, uint32_t deviceid, uint32_t channelid, std::string& sourceurl, dg::DynamicGraph& graphs);
int CreateDynamicGraph3(int graphid, uint32_t deviceid, uint32_t channelid, std::string& sourceurl, dg::DynamicGraph& graphs);

#endif // GRAPHMANAGER_H

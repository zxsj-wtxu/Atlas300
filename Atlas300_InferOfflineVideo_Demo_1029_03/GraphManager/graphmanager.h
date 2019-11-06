#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H
#include "DataRecv.h"
#include "hiaiengine/api.h"
#include "DynamicGraph.h"
#include "CommandLine.h"
#include "CommandParser.h"

int CreateDynamicGraph(int graphid, uint32_t deviceid, uint32_t channelid, std::string& sourceurl, dg::DynamicGraph& graphs);

#endif // GRAPHMANAGER_H

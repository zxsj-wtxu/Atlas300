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
int DestroyAllGraphs();
int DestroyGraph(unsigned int channelid);
int CreateGraph(int graphid, unsigned int channelid, const char* sourceurl);
int CreateGraph(int graphid, unsigned int deviceid, unsigned int channelid, const char *sourceurl);

#endif // GRAPHMANAGER_H

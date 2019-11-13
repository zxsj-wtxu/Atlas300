#include "WebServer.h"

int webserver_init(){
    return WebServer::Instance()->init();
}

void webserver_start(){
    WebServer::Instance()->start();
}

#ifdef __cplusplus
extern "C"{
#endif
int WebServerInit(){
    return webserver_init();
}

void WebServerStart(){
    webserver_start();
}

#ifdef __cplusplus
}
#endif



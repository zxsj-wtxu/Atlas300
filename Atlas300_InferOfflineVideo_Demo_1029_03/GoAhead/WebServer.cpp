/*
 * WebServer.cpp
 *
 *  Created on: 2019年7月21日
 *      Author: wtxu
 */

#include "WebServer.h"

int WebServer::finished = 0;
WebServer* WebServer::_this = NULL;
pthread_mutex_t WebServer::mutex_this = PTHREAD_MUTEX_INITIALIZER;

WebServer* WebServer::Instance(){
	if(pthread_mutex_lock(&mutex_this) == 0){
		if(_this == NULL)
			_this = new WebServer;
		pthread_mutex_unlock(&mutex_this);
	}
	return _this;
}

void WebServer::Destroy(){
	if(pthread_mutex_lock(&mutex_this) == 0){
		if(_this != NULL)
			delete _this;
		pthread_mutex_unlock(&mutex_this);
	}
}

WebServer::WebServer() {
	// TODO Auto-generated constructor stub
}

WebServer::~WebServer() {
	// TODO Auto-generated destructor stub
	finished = 1;
	printf("~WebServer()\n");
}

void* WebServer::ServerLoop(void* arg){
	WebServer* _this = (WebServer*)arg;
    websServiceEvents(&_this->finished);
    logmsg(1, "Instructed to exit");
    websClose();
//    websCloseAuth();
    websCloseRoute();
    websFsClose();
    websRuntimeClose();
    logClose();
}

void WebServer::start(){
	pthread_create(&tid_start, NULL, ServerLoop, (void*)this);
}

void WebServer::testPost(Webs *wp)
{
    WebsKey *s;
    websSetStatus(wp, 200);
    websWriteHeaders(wp, -1, 0);
    websWriteEndHeaders(wp);

    if (scaselessmatch(wp->method, "POST")) {
        printf("%s\n",wp->input.buf);
        const char* value = websGetVar(wp, "key", 0);
        printf("value:%s\n",value);

        websWrite(wp, "done\n");
    }

    websDone(wp);
}

void WebServer::HandleSystemInfoAction(Webs *wp){
    WebsKey     *s;

    if (scaselessmatch(wp->method, "POST")) {
        websSetStatus(wp, 200);
        websWriteHeaders(wp, -1, 0);
        websWriteEndHeaders(wp);

        const char* Name = websGetVar(wp, "Name", 0);
        const char* Option = websGetVar(wp, "Option", 0);
        const char* DeviceId = websGetVar(wp, "Id", 0);
        const char* Data = wp->input.buf;
        if(strncmp("Add", Option, 3) == 0){
        	if(BoardParams::Instance()->addSystemParam(Name, Data) < 0){
        		websWrite(wp, "add System param failed.\n");
        	}else{
        		websWrite(wp, "done\n");
        	}
        }else if(strncmp("Update", Option, 6) == 0){
        	if(BoardParams::Instance()->updateSystemParam(Name, Data) < 0){
        		websWrite(wp, "Update System param failed.\n");
        	}else{
        		websWrite(wp, "done\n");
        	}
        }else if(strncmp("Delete", Option, 6) == 0){
        	if(BoardParams::Instance()->removeSystemParam(DeviceId) < 0){
        		websWrite(wp, "Delete System param failed.\n");
        	}else{
        		websWrite(wp, "done\n");
        	}
        }else if(strncmp("Query", Option, 5) == 0){
            const MyString systemid(DeviceId);
        	SystemInfo sysinfo(systemid);
            websWrite(wp, "Name=%s\r\n", Name);
            websWrite(wp, "Option=%s\r\n", Option);
            websWrite(wp, "Id=%s\r\n", DeviceId);
        	if(BoardParams::Instance()->querySystemParam(sysinfo) < 0){
                websWrite(wp, "Data=null\r\n");
        	}else{
                websWrite(wp, "Data=%s\r\n", sysinfo.toJson().c_str());
        	}
        }else{
        	websWrite(wp, "Wrong method\r\n");
        }
    }else{
        websSetStatus(wp, 200);
        websWriteHeaders(wp, -1, 0);
        websWriteEndHeaders(wp);
        websWrite(wp, "Error\n");
    }

    websDone(wp);
}

void WebServer::HandleDeviceInfoAction(Webs *wp){
    WebsKey     *s;
    if (scaselessmatch(wp->method, "POST")) {
        websSetStatus(wp, 200);
        websWriteHeaders(wp, -1, 0);
        websWriteEndHeaders(wp);

        const char* Name = websGetVar(wp, "Name", 0);
        const char* Option = websGetVar(wp, "Option", 0);
        const char* DeviceId = websGetVar(wp, "Id", 0);
        const char* Data = wp->input.buf;
		printf("input:%s\n",wp->input.buf);
        if(strncmp("Add", Option, 3) == 0){
        	if(BoardParams::Instance()->addDeviceParam(Name, Data) < 0){
        		websWrite(wp, "add device param failed.\n");
        	}else{
        		websWrite(wp, "done\n");
        	}
        }else if(strncmp("Update", Option, 6) == 0){
        	if(BoardParams::Instance()->updateDeviceParam(Name, Data) < 0){
        		websWrite(wp, "Update device param failed.\n");
        	}else{
        		websWrite(wp, "done\n");
        	}
        }else if(strncmp("Delete", Option, 6) == 0){
        	if(BoardParams::Instance()->removeDeviceParam(DeviceId) < 0){
        		websWrite(wp, "Delete device param failed.\n");
        	}else{
        		websWrite(wp, "done\n");
        	}
        }else if(strncmp("Query", Option, 5) == 0){
        	DeviceInfo device(MyString(DeviceId).toInt());
            websWrite(wp, "Name=%s\r\n", Name);
            websWrite(wp, "Option=%s\r\n", Option);
            websWrite(wp, "Id=%s\r\n", DeviceId);
        	if(BoardParams::Instance()->queryDeviceParam(device) < 0){
                websWrite(wp, "Data=null\r\n");
        	}else{
                websWrite(wp, "Data=%s\r\n", device.toJson().c_str());
        	}
        }else{
        	websWrite(wp, "Wrong method\r\n");
        }
    }else{
        websSetStatus(wp, 200);
        websWriteHeaders(wp, -1, 0);
        websWriteEndHeaders(wp);
        websWrite(wp, "Error\n");
    }

    websDone(wp);
}

void WebServer::RegisterActionMethod(MyString webapi, void* fn){
	websDefineAction(webapi.c_str(), fn);
}

void WebServer::RegisterJstMethod(MyString webapi, WebsJstProc fn){
	websDefineJst(webapi.c_str(), fn);
}

int WebServer::myAspTtest(int eid, Webs *wp, int argc, char **argv)
{
    char	*name, *address;

    printf("%d\n", argc);
    for(int i=0; i<argc; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");

    return (int) websWrite(wp, "hello asp test");
}


void WebServer::HandleTest(Webs *wp){
    websSetStatus(wp, 200);
    websWriteHeaders(wp, -1, 0);
    websWriteEndHeaders(wp);
    websWrite(wp, "HandleTest\n");
    websDone(wp);
}

int WebServer::init(){
    int     argind;
    char buffer[1024] = {0};
	char *p = getcwd(buffer , sizeof(buffer));
    route = "config/goahead/route.txt";
    auth = "config/goahead/auth.txt";
    char home[1024] = {0};
    char documents[1024] = {0};
    sprintf(home, "%s/%s",p,"config/goahead");
    sprintf(documents, "%s/%s",p,"config/www/web");

    char *endpoint = "0.0.0.0:8081";

    logSetPath("stdout:2");

    initPlatform();
    if (websOpen(documents, route) < 0) {
        error("Cannot initialize server. Exiting.");
        return -1;
    }
    if (websLoad(auth) < 0) {
        error("Cannot load %s", auth);
        return -1;
    }
    logHeader();
	if (websListen(endpoint) < 0) {
		wfree(endpoints);
		return -1;
	}

	RegisterActionMethod("device", (void*)HandleDeviceInfoAction);
	RegisterActionMethod("system", (void*)HandleSystemInfoAction);
    RegisterActionMethod("test", (void*)HandleTest);
    websDefineJst("myAspTtest", myAspTtest);

}

void WebServer::logHeader(void)
{
    char    home[ME_GOAHEAD_LIMIT_STRING];

    getcwd(home, sizeof(home));
    logmsg(2, "Configuration for %s", ME_TITLE);
    logmsg(2, "---------------------------------------------");
    logmsg(2, "Version:            %s", ME_VERSION);
    logmsg(2, "BuildType:          %s", ME_DEBUG ? "Debug" : "Release");
    logmsg(2, "CPU:                %s", ME_CPU);
    logmsg(2, "OS:                 %s", ME_OS);
    logmsg(2, "Host:               %s", websGetServer());
    logmsg(2, "Directory:          %s", home);
    logmsg(2, "Documents:          %s", websGetDocuments());
    logmsg(2, "Configure:          %s", ME_CONFIG_CMD);
    logmsg(2, "---------------------------------------------");
}


void WebServer::initPlatform(void)
{
#if ME_UNIX_LIKE
    signal(SIGTERM, sigHandler);
    #ifdef SIGPIPE
        signal(SIGPIPE, SIG_IGN);
    #endif
#elif ME_WIN_LIKE
    _fmode=_O_BINARY;
#endif
}

#if ME_UNIX_LIKE
void WebServer::sigHandler(int signo)
{
    finished = 1;
}
#endif



/*
 * WebServer.h
 *
 *  Created on: 2019年7月21日
 *      Author: wtxu
 */

#ifndef WEBSERVER_H_
#define WEBSERVER_H_
#include "BoardParams.h"
#include "goahead.h"
//#include "BoardParams.h"

class WebServer {
public:
	virtual ~WebServer();

	static WebServer* Instance();
	static void Destroy();
	int init();
	void start();
	void RegisterJstMethod(MyString webapi, WebsJstProc fn);
	void RegisterActionMethod(MyString webapi, void* fn);

private:
	WebServer();
	static void* ServerLoop(void* arg);

	char *argp = NULL;
	char *endpoints = NULL;
	char *route = NULL;
	char *auth = NULL;
	char *tok = NULL;
	char *lspec = NULL;

	pthread_t tid_start;
	static int finished;
	static WebServer* _this;
	static pthread_mutex_t mutex_this;

	void logHeader(void);
	void initPlatform(void);
	static void sigHandler(int signo);
	static void testPost(Webs *wp);
	static void HandleDeviceInfoAction(Webs *wp);
	static void HandleSystemInfoAction(Webs *wp);
    static void HandleTest(Webs *wp);
    static int myAspTtest(int eid, Webs *wp, int argc, char **argv);
};

#endif /* WEBSERVER_H_ */

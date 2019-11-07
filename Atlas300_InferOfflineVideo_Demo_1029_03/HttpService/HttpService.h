/*
 * HttpService.h
 *
 *  Created on: 2019年6月10日
 *      Author: wtxu
 */

#ifndef HTTPSERVICE_H_
#define HTTPSERVICE_H_
#include <string>
#include <sstream>
#include <iostream>
#include <curl/curl.h>
extern"C"{
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
}
#include <openssl/md5.h>
#include "rlog.h"
using namespace std;

class HttpService {
	enum REQUESTTYPE{
		STRING,
		FILETYPE,
	};
	typedef struct Package{
		REQUESTTYPE type;
		string response;
		string writefilename;
	}DATA;
public:
	static HttpService* Instance();
	static void Release();

	void setHostUrl(string url);
	string get(string interface);
	string get(string interface, string filename);
	void SendPostData(const string& send_url, const string& data);
	static int jj_Openssl_GetFile_MD5(const char *filename, char *output);
	void Ftp_UploadFile(std::string ip, std::string port, std::string username, std::string passdword, std::string uploadpath, std::string uploadfile);
	virtual ~HttpService();
protected:
	HttpService();
private:
	CURLcode curl_get_req(const std::string &url, DATA &data);
	CURLcode curl_post_req(const string &url, const string &postParams, string &response);
	static int jj_Openssl_Get_MD5(const char *input, char *output);
	static size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream);

	string url;

	static HttpService* _this;
	static pthread_mutex_t mutex_this;
};

#endif /* HTTPSERVICE_H_ */

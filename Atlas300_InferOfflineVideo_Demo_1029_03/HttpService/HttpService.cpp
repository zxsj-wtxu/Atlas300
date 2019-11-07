/*
 * HttpService.cpp
 *
 *  Created on: 2019年6月10日
 *      Author: wtxu
 */

#include "HttpService.h"
HttpService* HttpService::_this = nullptr;
pthread_mutex_t HttpService::mutex_this = PTHREAD_MUTEX_INITIALIZER;

HttpService::HttpService() {
	// TODO Auto-generated constructor stub
	curl_global_init(CURL_GLOBAL_ALL);
}

HttpService* HttpService::Instance(){
	if(pthread_mutex_lock(&mutex_this) == 0){
		if(_this == nullptr)
			_this = new HttpService;
		pthread_mutex_unlock(&mutex_this);
	}
	return _this;
}

void HttpService::Release(){
	if(pthread_mutex_lock(&mutex_this) == 0){
		if(_this != nullptr)
			delete _this;
		pthread_mutex_unlock(&mutex_this);
	}
}

HttpService::~HttpService() {
	// TODO Auto-generated destructor stub
	curl_global_cleanup();
}

void HttpService::setHostUrl(string url){
	if(_this == NULL) return;
	this->url = url;
}

string HttpService::get(string interface){
	if(_this == NULL) return "";
	HttpService::DATA data;
	data.type = HttpService::STRING;
    auto res = curl_get_req(url+interface, data);
    if (res != CURLE_OK)
        return "";
    return data.response;
}

void HttpService::Ftp_UploadFile(std::string ip, std::string port, std::string username, std::string passdword, std::string uploadpath, std::string uploadfile){
	std::string url = "ftp://"+ip + "//" + uploadpath+"/"+uploadfile;
	std::string usrpwd = username + ":" + passdword;

	cout << url <<endl;
	FILE* pfile = NULL;
	struct stat file_info;

	CURLcode res;
	curl_off_t fsize;
	CURL *curl = curl_easy_init();

	if (stat(uploadfile.c_str(), &file_info)) {
		printf("Couldn't open '%s': %s\n", uploadfile.c_str(), strerror(errno));
		return;
	}

	fsize = (curl_off_t)file_info.st_size;
	pfile = fopen(uploadfile.c_str(), "rb");
	curl_global_init(CURL_GLOBAL_ALL);


	if (curl) {
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
//curl_easy_setopt(curl, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_NOCWD);

		curl_easy_setopt(curl, CURLOPT_USERPWD, usrpwd.c_str());
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
		curl_easy_setopt(curl, CURLOPT_PORT, atoi(port.c_str()));

		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_READDATA, pfile);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		curl_easy_cleanup(curl);
	}
	fclose(pfile);
	curl_global_cleanup();
	return;
}

void HttpService::SendPostData(const string& send_url, const string& data){
    CURL *curl = curl_easy_init();
    CURLcode res;

    struct curl_slist *head = NULL;
    if (curl)
    {
        // set params
//    	head = curl_slist_append(head,"Content-type:tex/json;charset=utf-8");
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
        curl_easy_setopt(curl, CURLOPT_URL, send_url.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST"); // url

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str()); // params
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_HEADER, head);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
}

string HttpService::get(string interface, string filename){
	if(_this == NULL) return "";
	HttpService::DATA data;
	data.type = HttpService::FILETYPE;
	data.writefilename = filename;
    auto res = curl_get_req(url+interface, data);
    if (res != CURLE_OK)
        return "";
    return data.response;
}
int HttpService::jj_Openssl_Get_MD5(const char *input, char *output) {
	char password[1024 * 1024 * 5] = { 0 };
	MD5_CTX x;
	int i = 0, len;
	char *out = NULL;
	unsigned char d[16];
	unsigned char tmp[128] = { 0 };
	strcpy(password, input);
	MD5_Init(&x);
	MD5_Update(&x, (char *) password, strlen(password));
	MD5_Final(d, &x);
	out = (char *) malloc(35);
	memset(out, 0x00, 35);
	strcpy(out, "$1$");
	for (i = 0; i < 16; i++) {
		sprintf(out + (i * 2), "%02x", d[i]); //转换为32位
	}
	out[32] = 0;
	strcpy(output, out);
	free(out);
	return 0;
}

int HttpService::jj_Openssl_GetFile_MD5(const char *filename, char *output) {
	MD5_CTX ctx;
	int len = 0;
	char *out = NULL;
	unsigned char buffer[1024] = {0};
	unsigned char digest[16] = {0};

	FILE *pFile = fopen (filename, "rb"); // 我没有判断空指针

	MD5_Init (&ctx);
	while ((len = fread (buffer, 1, 1024, pFile)) > 0)
	{
		MD5_Update (&ctx, buffer, len);
	}

	MD5_Final (digest, &ctx);

	fclose(pFile);

	out = (char *) malloc(35);
	memset(out, 0x00, 35);
	strcpy(out, "$1$");
	for (int i = 0; i < 16; i++) {
		sprintf(out + (i * 2), "%02x", digest[i]); //转换为32位
	}
	out[32] = 0;
	strcpy(output, out);
	free(out);

    return 0;
}

// reply of the requery
size_t HttpService::req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
{
	HttpService::DATA *datapackage = (HttpService::DATA*)stream;

	datapackage->response.append((char*)ptr, size*nmemb);
    return size * nmemb;
}

int my_progress_func(char *progress_data,
                     double t, /* dltotal */
                     double d, /* dlnow */
                     double ultotal,
                     double ulnow)
{
  LOG_DEBUG("%s %g / %g (%g %%)", progress_data, d, t, d*100.0/t);
  return 0;
}

CURLcode HttpService::curl_get_req(const std::string &url, DATA &data)
{
	char *progress_data = "* ";
    // init curl
    CURL *curl = curl_easy_init();
    // res code
    CURLcode res;

    if (curl)
    {
        // set params
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
//        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
//        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);
//        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress_data);
//        curl_easy_setopt(curl, CURLOPT_HEADER, 1); //如果配置此项，则http的头信息也会写入到回调函数中
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        // start req
        res = curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);
    return res;
}

// http POST
CURLcode HttpService::curl_post_req(const string &url, const string &postParams, string &response)
{
    // init curl
    CURL *curl = curl_easy_init();
    // res code
    CURLcode res;
    if (curl)
    {
        // set params
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        // start req
        res = curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);
    return res;
}

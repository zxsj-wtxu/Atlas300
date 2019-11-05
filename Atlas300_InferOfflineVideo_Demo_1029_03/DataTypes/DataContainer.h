/*
 * DataContainer.h
 *
 *  Created on: 2019年7月11日
 *      Author: wtxu
 */

#ifndef DATACONTAINER_H_
#define DATACONTAINER_H_
#include <pthread.h>
#include <map>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RAPIDJSON
#ifdef RAPIDJSON
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#endif
#include "MyString.h"
using namespace std;
using namespace rapidjson;

#ifndef LOG_DEBUG
#define LOG_DEBUG(fmt, args...) \
    do { printf("\033[1m\033[32m[Debug],[FILE]:%s[Func]:%s [Line]:%d [Info]:" \
                fmt"\033[0m", __FILE__, __FUNCTION__, __LINE__, ##args); \
    } while (0)
#endif

#ifndef LOG_INFO
#define LOG_INFO(fmt, args...) \
    do { printf("\033[1m\033[34m[Info],[FILE]:%s[Func]:%s [Line]:%d [Info]:" \
                            fmt"\033[0m",__FILE__, __FUNCTION__, __LINE__, ##args); \
    } while (0)
#endif

#ifndef LOG_WARN
#define LOG_WARN(fmt, args...) \
    do { printf("\033[1m\033[33m[Warn],[FILE]:%s[Func]:%s [Line]:%d [Info]:" \
                            fmt"\033[0m",__FILE__, __FUNCTION__, __LINE__,##args); \
    } while (0)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(fmt, args...) \
    do { printf("\033[1m\033[31m[Error],[FILE]:%s[Func]:%s [Line]:%d [Info]:" \
                            fmt"\033[0m",__FILE__, __FUNCTION__, __LINE__, ##args); \
    } while (0)
#endif

class DataContainer {
public:
	class Basedata{
	public:
		Basedata() = delete;
		Basedata(const MyString& data);
		Basedata(const Basedata& bd);

	    operator int();
	    operator double();
	    operator long();
	    operator long long();
	    operator bool();

	    bool empty();
	    void SetValue(const MyString& data);
	    MyString GetString() const;
	    const bool GetBool();
	    const int GetInt();
	    const double GetDouble();
	    const long GetLong();
	    const long long GetLongLong();

	    Basedata& operator=(const Basedata& data) = delete;
	private:
	    MyString value;
	};

public:
	DataContainer();
	virtual ~DataContainer();

	int clear();
	int remove(const MyString& key);
	Basedata operator[](const MyString& key);

	int insertKeyValue(const MyString& key, int value);
	int insertKeyValue(const MyString& key, bool value);
	int insertKeyValue(const MyString& key, double value);
	int insertKeyValue(const MyString& key, long int value);
	int insertKeyValue(const MyString& key, const MyString& value);

	DataContainer& operator=(const DataContainer& data);
#ifdef RAPIDJSON
	const MyString toJson();
	DataContainer(const MyString& json);
	int setJson(const MyString& json);
#endif
private:
	map<const MyString, Basedata*> params;
	pthread_mutex_t mutx_params = PTHREAD_MUTEX_INITIALIZER;
protected:
	bool checkJsonInt(const rapidjson::Value& document, const char* member);
	bool checkJsonString(const rapidjson::Value& document, const char* member);
	bool checkJsonObject(const rapidjson::Value& document, const char* member);
};

#endif /* DATACONTAINER_H_ */

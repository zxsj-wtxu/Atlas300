/*
 * DataBaseContainer.h
 *
 *  Created on: 2019年7月12日
 *      Author: wtxu
 */

#ifndef DATABASECONTAINER_H_
#define DATABASECONTAINER_H_
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <pthread.h>
#include "MyString.h"
using namespace std;

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

class DataBaseContainer {
public:
	DataBaseContainer() = delete;
	virtual ~DataBaseContainer();

	DataBaseContainer(const MyString& table);
	DataBaseContainer(const DataBaseContainer& container);

	int size();
	void clear();
	MyString operator[](MyString& key);
	void set(map<const MyString, MyString> key_value);
	map<const MyString, MyString> getData();
	MyString getTableName();
	MyString getCreateTableSqlCmd();
	MyString getInsertCmd(const MyString& key, const MyString& value);
	MyString getUpdateCmd(const MyString& key, const MyString& value);
	MyString getQueryCmd(const MyString& key);
	MyString getDeleteCmd(const MyString& key);
	MyString toDatabaseString();

	int insertKeyValue(const MyString& key, int value);
	int insertKeyValue(const MyString& key, double value);
	int insertKeyValue(const MyString& key, long int value);
	int insertKeyValue(const MyString& key, MyString& value);
	int insertKeyValue(const MyString& key, MyString value);
	int insertKeyValue(MyString& key, MyString& value);

private:
	const MyString tablename;
	map<const MyString, MyString> key_value;
	pthread_mutex_t mutex_key_value = PTHREAD_MUTEX_INITIALIZER;

	MyString queryParamSqlCmd();
	MyString queryParamSqlCmd(const MyString& key);
	MyString deleteParamSqlCmd(const MyString& key);
	MyString insertParamSqlCmd(const MyString& field, const MyString& value);
	MyString updateParamSqlCmd(const MyString& field, const MyString& value);
};

#endif /* DATABASECONTAINER_H_ */

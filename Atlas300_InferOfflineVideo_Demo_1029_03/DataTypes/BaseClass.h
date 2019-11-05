/*
 * BaseClass.h
 *
 *  Created on: 2019年7月11日
 *      Author: wtxu
 */

#ifndef BASECLASS_H_
#define BASECLASS_H_
#include "DataContainer.h"

class BaseClass{
public:
	BaseClass();
	virtual ~BaseClass();
#ifdef RAPIDJSON
	MyString toJson();
	BaseClass(const MyString& json);
	BaseClass(const BaseClass& b);
	int setJson(const MyString& json);
#endif
	DataContainer::Basedata operator[](const MyString& key);

	int clear();
	int remove(const MyString& key);

	BaseClass& operator=(const BaseClass& data);

	int insertKeyValue(const MyString& key, int value);
	int insertKeyValue(const MyString& key, double value);
	int insertKeyValue(const MyString& key, long int value);
	int insertKeyValue(const MyString& key, const MyString& value);
protected:
	DataContainer deviceData;

	bool checkJsonInt(const rapidjson::Value& document, const char* member);
	bool checkJsonString(const rapidjson::Value& document, const char* member);
	bool checkJsonObject(const rapidjson::Value& document, const char* member);
};

#endif /* BASECLASS_H_ */

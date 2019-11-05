/*
 * SystemDatabase.cpp
 *
 *  Created on: 2019年7月20日
 *      Author: wtxu
 */

#include "SystemDatabase.h"

SystemDatabase::SystemDatabase(const MyString& tablename, const MyString& id):
DataBaseContainer(tablename),SystemInfo(id){
	DataBaseContainer::insertKeyValue(SystemInfo::getSystemId(), MyString(SystemInfo::toJson()));
}

SystemDatabase::SystemDatabase(const MyString& tablename, const char* json):
		SystemInfo(json), DataBaseContainer(tablename){
	insertToDatabase(SystemInfo::toJson());
}

SystemDatabase::~SystemDatabase() {
	// TODO Auto-generated destructor stub
}

int SystemDatabase::setJson(const MyString& json){
	int ret = SystemInfo::setJson(json);
	if(ret < 0) return -1;
	insertToDatabase(SystemInfo::toJson());
	return 0;
}

int SystemDatabase::clear(){
	SystemInfo::clear();
	insertToDatabase(SystemInfo::toJson());
	return 0;
}

int SystemDatabase::remove(const MyString& key){
	SystemInfo::remove(key);
	insertToDatabase(SystemInfo::toJson());
	return 0;
}

int SystemDatabase::insertKeyValue(const MyString& key, int value){
	SystemInfo::insertKeyValue(key,value);
	insertToDatabase(SystemInfo::toJson());
	return 0;
}

int SystemDatabase::insertKeyValue(const MyString& key, double value){
	SystemInfo::insertKeyValue(key,value);
	insertToDatabase(SystemInfo::toJson());
	return 0;
}

int SystemDatabase::insertKeyValue(const MyString& key, long int value){
	SystemInfo::insertKeyValue(key,value);
	insertToDatabase(SystemInfo::toJson());
	return 0;
}

int SystemDatabase::insertKeyValue(const MyString& key, MyString value){
	SystemInfo::insertKeyValue(key,value);
	insertToDatabase(SystemInfo::toJson());
	return 0;
}

int SystemDatabase::insertToDatabase(const MyString& json){
	DataBaseContainer::insertKeyValue(SystemInfo::getSystemId(), json);
	return 0;
}

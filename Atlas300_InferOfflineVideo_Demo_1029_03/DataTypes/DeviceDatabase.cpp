/*
 * DeviceDatabase.cpp
 *
 *  Created on: 2019年7月12日
 *      Author: wtxu
 */

#include "DeviceDatabase.h"

DeviceDatabase::DeviceDatabase(const MyString& tablename, const char* json):
DeviceInfo(json),DataBaseContainer(tablename){
	DataBaseContainer::insertKeyValue(std::to_string(DeviceInfo::getDeviceCode()), MyString(DeviceInfo::toJson()));
}

DeviceDatabase::DeviceDatabase(const MyString& tablename, int deviceid):
DeviceInfo(deviceid), DataBaseContainer(tablename) {
	// TODO Auto-generated constructor stub
	insertToDatabase(DeviceInfo::toJson());
}

DeviceDatabase::~DeviceDatabase() {
	// TODO Auto-generated destructor stub
}

int DeviceDatabase::setJson(const char* json){
	int ret = DeviceInfo::setJson(json);
	if(ret < 0) return -1;
	insertToDatabase(DeviceInfo::toJson());
	return 0;
}

int DeviceDatabase::clear(){
	DeviceInfo::clear();
	insertToDatabase(DeviceInfo::toJson());
	return 0;
}

int DeviceDatabase::remove(const MyString& key){
	DeviceInfo::remove(key);
	insertToDatabase(DeviceInfo::toJson());
	return 0;
}

int DeviceDatabase::clearAreas(){
	DeviceInfo::clearAreas();
	insertToDatabase(DeviceInfo::toJson());
	return 0;
}

int DeviceDatabase::removeArea(int type){
	DeviceInfo::removeArea(type);
	insertToDatabase(DeviceInfo::toJson());
	return 0;
}

int DeviceDatabase::createArea(const vector<AreaInfo::POINT>& points,  int type){
	DeviceInfo::createArea(points, type);
	insertToDatabase(DeviceInfo::toJson());
	return 0;
}

int DeviceDatabase::insertKeyValue(const MyString& key, int value){
	DeviceInfo::insertKeyValue(key,value);
	insertToDatabase(DeviceInfo::toJson());
	return 0;
}

int DeviceDatabase::insertKeyValue(const MyString& key, double value){
	DeviceInfo::insertKeyValue(key,value);
	insertToDatabase(DeviceInfo::toJson());
	return 0;
}

int DeviceDatabase::insertKeyValue(const MyString& key, long int value){
	DeviceInfo::insertKeyValue(key,value);
	insertToDatabase(DeviceInfo::toJson());
	return 0;
}

int DeviceDatabase::insertKeyValue(const MyString& key, MyString value){
	DeviceInfo::insertKeyValue(key,value);
	insertToDatabase(DeviceInfo::toJson());
	return 0;
}

int DeviceDatabase::insertToDatabase(const MyString& json){
	DataBaseContainer::insertKeyValue(DeviceInfo::getDeviceCode(), json);
	return 0;
}

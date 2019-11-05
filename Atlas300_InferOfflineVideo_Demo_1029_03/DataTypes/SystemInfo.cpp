/*
 * SystemInfo.cpp
 *
 *  Created on: 2019年7月11日
 *      Author: wtxu
 */

#include "SystemInfo.h"

SystemInfo::SystemInfo(const MyString& id):systemid(id) {
	// TODO Auto-generated constructor stub
	BaseClass::insertKeyValue("systemid", systemid);
}

SystemInfo::SystemInfo(const char* json):BaseClass(json){
	this->systemid = deviceData["systemid"].GetString();
	BaseClass::insertKeyValue("systemid", systemid);
}

SystemInfo::~SystemInfo() {
	// TODO Auto-generated destructor stub
}

MyString SystemInfo::getSystemId(){
	return systemid;
}

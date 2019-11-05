/*
 * BaseClass.cpp
 *
 *  Created on: 2019年7月11日
 *      Author: wtxu
 */

#include "BaseClass.h"

BaseClass::BaseClass() {
	// TODO Auto-generated constructor stub

}

BaseClass::~BaseClass() {
	// TODO Auto-generated destructor stub
}
#ifdef RAPIDJSON
BaseClass::BaseClass(const MyString& json) {
	Document document;
	document.Parse(json.c_str());
	if (document.HasParseError()) {
		rapidjson::ParseErrorCode code = document.GetParseError();
		LOG_INFO("JSON parsing error\n");
		return;
	}
	if (!document.IsObject())
		return;
	const rapidjson::Value &root = document.GetObject();

	for (Value::ConstMemberIterator itr = root.MemberBegin();
			itr != root.MemberEnd(); ++itr) {

		if (checkJsonString(root, itr->name.GetString())) {
			deviceData.insertKeyValue(MyString(itr->name.GetString()),
					MyString(itr->value.GetString()));
		} else if (checkJsonInt(root, itr->name.GetString())) {
			deviceData.insertKeyValue(MyString(itr->name.GetString()),
					std::to_string(itr->value.GetInt()));
		}
	}
}

/*
 * 功能：以JSON格式返回数据，返回的数据必须使用delete[] 释放内存
 * */
MyString BaseClass::toJson() {
	return deviceData.toJson();;
}
#endif
bool BaseClass::checkJsonInt(const rapidjson::Value& document,
		const char* member) {
	if (!document.HasMember(member)) {
		LOG_ERROR("JSON data error, no member of %s \n", member);
		return false;
	} else {
		if (!document[member].IsInt()) {
			LOG_ERROR("JSON data error, member of %s should be Int. \n", member);
			return false;
		}
	}
	return true;
}

bool BaseClass::checkJsonString(const rapidjson::Value& document,
		const char* member) {
	if (!document.HasMember(member)) {
		LOG_ERROR("JSON data error, no member of %s \n", member);
		return false;
	} else {
		if (!document[member].IsString()) {
			LOG_ERROR("JSON data error, member of %s should be MyString. \n",
					member);
			return false;
		}
	}
	return true;
}

bool BaseClass::checkJsonObject(const rapidjson::Value& document,
		const char* member) {
	if (!document.HasMember(member)) {
		LOG_ERROR("JSON data error, no member of %s \n", member);
		return false;
	} else {
		if (!document[member].IsObject()) {
			LOG_ERROR("JSON data error, member of %s should be Object. \n",
					member);
			return false;
		}
	}
	return true;
}

int BaseClass::setJson(const MyString& json){
	return deviceData.setJson(json);
}

int BaseClass::clear(){
	return deviceData.clear();
}

int BaseClass::remove(const MyString& key){
	return deviceData.remove(key);
}

BaseClass::BaseClass(const BaseClass& b){
	this->operator =(b);
}

BaseClass& BaseClass::operator=(const BaseClass& data){
	this->deviceData = data.deviceData;
	return *this;
}

int BaseClass::insertKeyValue(const MyString& key, int value) {
	deviceData.insertKeyValue(key, value);
}

int BaseClass::insertKeyValue(const MyString& key, double value) {
	deviceData.insertKeyValue(key, value);
}

int BaseClass::insertKeyValue(const MyString& key, long int value) {
	deviceData.insertKeyValue(key, value);
}

int BaseClass::insertKeyValue(const MyString& key, const MyString& value) {
	deviceData.insertKeyValue(key, value);
}

DataContainer::Basedata BaseClass::operator[](const MyString& key) {
	return deviceData[key];
}


/*
 * AreaInfo.cpp
 *
 *  Created on: 2019年7月11日
 *      Author: wtxu
 */

#include "AreaInfo.h"

AreaInfo::AreaInfo(const AreaInfo& area){
	this->operator =(area);
}

AreaInfo::AreaInfo(int type){
	this->type = type;
}

#ifdef RAPIDJSON
AreaInfo::AreaInfo(MyString json){
	char obj_name[10] = {0};
	Document document;
	document.Parse(json.c_str());
	if (document.HasParseError()) {
		LOG_INFO("JSON parsing error\n");
		return;
	}
	if(!document.IsObject()) return;
	const rapidjson::Value &root = document.GetObject();
	if(!checkJsonInt(root, "type")) return;
	if(!checkJsonInt(root, "pointsnum")) return;
	if(!checkJsonString(root, "name")) return;

	this->type = root["type"].GetInt();
	this->pointNum = root["pointsnum"].GetInt();
	this->name = root["name"].GetString();

	if(!checkJsonObject(root, "coordinate_x")) return;
	if(!checkJsonObject(root, "coordinate_y")) return;

	{
		StringBuffer buffer;
		rapidjson::Writer<StringBuffer> writer(buffer);
		root["coordinate_x"].Accept(writer);
		containerX.setJson(buffer.GetString());
	}

	{
		StringBuffer buffer;
		rapidjson::Writer<StringBuffer> writer(buffer);
		root["coordinate_y"].Accept(writer);
		containerY.setJson(buffer.GetString());
	}
	this->insertDoneStatus = true;
}
/*
 * 功能：以JSON格式返回数据，返回的数据必须使用delete[] 释放内存
 * */
MyString AreaInfo::toJson(){

	rapidjson::Document document;
	document.SetObject();

	Value obj;
	rapidjson::Document coordinatedoc;

	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	document.AddMember("type", this->type, allocator);
	document.AddMember("pointsnum", this->pointNum, allocator);
	document.AddMember("name", Value(this->name.c_str(), document.GetAllocator()).Move(), allocator);

	MyString json_x = containerX.toJson();
	coordinatedoc.Parse(json_x.c_str());
	if (coordinatedoc.HasParseError()) {
		LOG_INFO("JSON parsing error\n");
		return 0;
	}
	obj.CopyFrom(coordinatedoc, coordinatedoc.GetAllocator());
	document.AddMember(Value("coordinate_x", document.GetAllocator()).Move(), obj.Move(), allocator);

	MyString json_y = containerY.toJson();
	coordinatedoc.Parse(json_y.c_str());
	if (coordinatedoc.HasParseError()) {
		rapidjson::ParseErrorCode code = coordinatedoc.GetParseError();
		LOG_INFO("JSON parsing error\n");
		return 0;
	}
	obj.CopyFrom(coordinatedoc, coordinatedoc.GetAllocator());
	document.AddMember(Value("coordinate_y", document.GetAllocator()).Move(), obj.Move(), allocator);

	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	document.Accept(writer);

//	LOG_DEBUG("%s\n",buffer.GetString());
	return MyString(buffer.GetString());
}
#endif

AreaInfo::~AreaInfo(){
	clear();
}

int AreaInfo::size(){
	return this->pointNum;
}

int AreaInfo::getType(){
	return this->type;
}

int AreaInfo::clear(){
	if(!this->insertDoneStatus) {
		LOG_INFO("clear failed, you must call insertDone() first.\n");
		return -1;
	}
	this->pointNum = 0;
	containerX.clear();
	containerY.clear();
	this->insertDoneStatus = false;
	return 0;
}

int AreaInfo::getPoints(vector<POINT>& temp){
	if(!this->insertDoneStatus){
		LOG_INFO("Get Points failed, still instert...\n");
		return -1;
	}

	for(int i=0; i<this->pointNum; i++){
		MyString key_x = "x"+std::to_string(i);
		MyString key_y = "y"+std::to_string(i);
		temp.push_back(POINT(containerX[key_x].GetInt(), containerY[key_y].GetInt()));
	}

	return 0;
}

bool AreaInfo::isInsertDone(){
	return 	this->insertDoneStatus;
}

void AreaInfo::insertDone(){
//	LOG_INFO("create area succeed.\n");
	this->insertDoneStatus = true;
}

const int AreaInfo::insertPoint(int x, int y){
	if(insertDoneStatus) {
		LOG_INFO("cannot insert Point.\n");
		return this->pointNum;
	}

	MyString key_x = "x" + std::to_string(this->pointNum);
	MyString key_y = "y" + std::to_string(this->pointNum);

	containerX.insertKeyValue(key_x, x);
	containerY.insertKeyValue(key_y, y);

	this->pointNum += 1;

	return this->pointNum;
}

AreaInfo::POINT AreaInfo::operator [](int index){
	int x = containerX["x"+std::to_string(index)].GetInt();
	int y = containerY["y"+std::to_string(index)].GetInt();

	return POINT(x, y);
}

AreaInfo& AreaInfo::operator=(const AreaInfo& area){
	cout <<"AreaInfo& AreaInfo::operator=(const AreaInfo& area)" <<endl;
	BaseClass::operator =(area);
	this->type = area.type;
	this->name = area.name;
	this->pointNum = area.pointNum;
	this->containerX = area.containerX;
	this->containerY = area.containerY;
	this->insertDoneStatus = area.insertDoneStatus;
	return *this;
}

MyString AreaInfo::toString(){
	stringstream sstr;
	sstr <<"pointsnum:" << this->pointNum;
	for(int i=0; i<this->pointNum; i++){
		sstr <<"|" <<"x" <<i <<":" << containerX["x"+std::to_string(i)].GetInt();
		sstr <<"|" <<"y" <<i <<":" << containerY["y"+std::to_string(i)].GetInt();
	}
	sstr << endl;

	return sstr.str();
}

AreaInfo::POINT::POINT(int x, int y):x(x),y(y){

}

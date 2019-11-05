/*
 * DeviceInfo.cpp
 *
 *  Created on: 2019年7月9日
 *      Author: wtxu
 */

#include "DeviceInfo.h"

DeviceInfo::DeviceInfo(int deviceid) {
	this->deviceid = deviceid;
}

DeviceInfo::~DeviceInfo() {
	// TODO Auto-generated destructor stub
	vector<AreaInfo*>::iterator it;
	if (pthread_mutex_lock(&areas_mutex) == 0) {
		for (it = this->areas.begin(); it != this->areas.end();) {
			delete *it;
			it = this->areas.erase(it);
		}
		pthread_mutex_unlock(&areas_mutex);
	}
}

AreaInfo* DeviceInfo::operator[](int index) {
	AreaInfo* area = NULL;
	if (index > areas.size())
		return NULL;
	return areas[index];
}

DeviceInfo::DeviceInfo(const DeviceInfo& device){
	this->operator =(device);
}

DeviceInfo&  DeviceInfo::operator=(const DeviceInfo& device){
	BaseClass::operator =(device);
	this->deviceid = device.deviceid;
	this->areaid = device.areaid;

	vector<AreaInfo*>::const_iterator it;
	for(it = device.areas.begin(); it != device.areas.end(); it++){
		this->areas.push_back(new AreaInfo((*it)->toJson()));
	}
	return *this;
}

int DeviceInfo::getDeviceCode(){
	return deviceid;
}

int DeviceInfo::areaSize() {
	int ret = 0;
	if (pthread_mutex_lock(&areas_mutex) == 0) {
		ret = this->areas.size();
		pthread_mutex_unlock(&areas_mutex);
	}

	return ret;
}

int DeviceInfo::getDeivceCode(){
	return this->deviceid;
}

int DeviceInfo::clearAreas() {
	int ret = 0;
	if (pthread_mutex_lock(&areas_mutex) == 0) {
		vector<AreaInfo*>::iterator it;
		for (it = this->areas.begin(); it != this->areas.end();) {
			if(*it != NULL)
				delete *it;
			it = this->areas.erase(it);
		}
		pthread_mutex_unlock(&areas_mutex);
	}

	return ret;
}

int DeviceInfo::removeArea(int type) {
	int ret = -1;
	type = type & 0xff;
	vector<AreaInfo*>::iterator it;
	if (pthread_mutex_lock(&areas_mutex) == 0) {
		for(it = areas.begin(); it!=areas.end();){
			if(*it == NULL) {
				LOG_WARN("area is NULL, type = %d.\n",type);
				continue;
			}
			AreaInfo* area = *it;
			if(area->getType() == type){
				if(*it != NULL)
					delete *it;
				it = this->areas.erase(it);
				ret = 0;
				break;
			}else{
				it++;
				ret = -1;
			}
		}
		pthread_mutex_unlock(&areas_mutex);
	}

	return ret;
}
/*
 * 功能：将一个区域保存至设备类中，区域的类型值是唯一的
 * 返回值：成功返回当前设备类中的区域个数，失败返回-1
 * */
int DeviceInfo::createArea(const vector<AreaInfo::POINT>& points, int type) {
	AreaInfo* area = new AreaInfo(type);
	type = type & 0xff;
	int size = points.size();
	for (int i = 0; i < size; i++) {
		area->insertPoint(points[i].x, points[i].y);
	}
	area->insertDone();

	size = inserArea(area, type);
	if (size < 0) {
		delete area;
		return -1;
	}
	return size;
}
/*
 * 功能：将一个区域保存至设备类中，区域的类型值是唯一的
 * 返回值：成功返回当前设备类中的区域个数，失败返回-1
 * */
const int DeviceInfo::inserArea(AreaInfo* area, int type) {
	if (!area->isInsertDone())
		return -1;
	int size = 0;
	type = type & 0xff;
	vector<AreaInfo*>::iterator it;
	if (pthread_mutex_lock(&areas_mutex) == 0) {
		for(it = areas.begin(); it!=areas.end(); it++){
			if(*it == NULL) {
				LOG_WARN("area is NULL, type = %d.\n",type);
				continue;
			}
			AreaInfo* area = *it;
			if(area->getType() == type){
				LOG_WARN("area type %d alread exist!\n", type);
				break;
			}
		}
		if(it == areas.end()){
			areas.push_back(area);
		}
		pthread_mutex_unlock(&areas_mutex);
	}
	return size;
}

#ifdef RAPIDJSON
int DeviceInfo::setJson(const char* json){
	char areaName[20] = { 0 };
	Document document;
	document.Parse(json);
	if (document.HasParseError()) {
		rapidjson::ParseErrorCode code = document.GetParseError();
		LOG_INFO("JSON parsing error\n");
		return -1;
	}
	if (!document.IsObject())
		return -1;
	if (!checkJsonObject(document, "BaseInfo"))
		return -1;
	if(!checkJsonInt(document, "DeivceCode"))
		return -1;
	this->deviceid = document["DeivceCode"].GetInt();
	const rapidjson::Value &root = document["BaseInfo"];

	for (Value::ConstMemberIterator itr = root.MemberBegin();
			itr != root.MemberEnd(); ++itr) {

		if (checkJsonString(root, itr->name.GetString())) {
			deviceData.insertKeyValue(MyString(itr->name.GetString()), MyString(itr->value.GetString()));
		} else if (checkJsonInt(root, itr->name.GetString())) {
			deviceData.insertKeyValue(MyString(itr->name.GetString()), std::to_string(itr->value.GetInt()));
		}else{
			LOG_WARN("Incomplete data %s-%s.\n",itr->name.GetString(),itr->value.GetString());
		}
	}

	if (!checkJsonObject(document, "Areas")) {
		LOG_WARN("Incomplete data.\n");
		return -1;
	}
	const rapidjson::Value &jvareas = document["Areas"];

	if (!checkJsonInt(jvareas, "AreasNum")) {
		LOG_WARN("Incomplete data.\n");
		return -1;
	}
	int AreasNum = jvareas["AreasNum"].GetInt();

	vector<AreaInfo*>::iterator it;
	for (int i = 0; i < AreasNum; i++) {
		sprintf(areaName, "area%d", i);
		if (!checkJsonObject(jvareas, areaName)){
			LOG_WARN("Incomplete data.\n");
			return -1;
		}

		StringBuffer buffer;
		rapidjson::Writer<StringBuffer> writer(buffer);
		jvareas[areaName].Accept(writer);
		this->areas.push_back(new AreaInfo(buffer.GetString()));
	}
	return 0;
}

DeviceInfo::DeviceInfo(const char* json) {
	char areaName[20] = { 0 };
	Document document;
	document.Parse(json);
	if (document.HasParseError()) {
		rapidjson::ParseErrorCode code = document.GetParseError();
		LOG_INFO("JSON parsing error\n");
		return;
	}
	if (!document.IsObject())
		return;
	if (!checkJsonObject(document, "BaseInfo"))
		return;
	if(!checkJsonInt(document, "DeivceCode"))
		return;
	this->deviceid = document["DeivceCode"].GetInt();
	const rapidjson::Value &root = document["BaseInfo"];

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

	if (!checkJsonObject(document, "Areas")) {
		LOG_WARN("Incomplete data.\n");
		return;
	}
	const rapidjson::Value &jvareas = document["Areas"];

	if (!checkJsonInt(jvareas, "AreasNum")) {
		LOG_WARN("Incomplete data.\n");
		return;
	}
	int AreasNum = jvareas["AreasNum"].GetInt();

	for (int i = 0; i < AreasNum; i++) {
		sprintf(areaName, "area%d", i);
		if (!checkJsonObject(jvareas, areaName))
			return;

		StringBuffer buffer;
		rapidjson::Writer<StringBuffer> writer(buffer);
		jvareas[areaName].Accept(writer);
		AreaInfo* area = new AreaInfo(buffer.GetString());
		this->areas.push_back(area);
	}
}

/*
 * 功能：以JSON格式返回数据，返回的数据必须使用delete[] 释放内存
 * */
const MyString DeviceInfo::toJson() {
	int ret = 0;
	char object_name[10] = { 0 };

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	Value obj;
	rapidjson::Document areadoc;

	rapidjson::Document AreaObj;
	AreaObj.SetObject();

	document.AddMember(Value("DeivceCode", document.GetAllocator()).Move(),
			deviceid, document.GetAllocator());

	MyString temp = deviceData.toJson();
	areadoc.Parse(temp.c_str());
	if (areadoc.HasParseError()) {
		rapidjson::ParseErrorCode code = areadoc.GetParseError();
		LOG_INFO("JSON parsing error\n");
		return "";
	}
	obj.CopyFrom(areadoc, areadoc.GetAllocator());
	document.AddMember(Value("BaseInfo", document.GetAllocator()).Move(),
			obj.Move(), document.GetAllocator());

	AreaObj.AddMember(Value("AreasNum", AreaObj.GetAllocator()).Move(),
			this->areas.size(), AreaObj.GetAllocator());

	if (pthread_mutex_lock(&areas_mutex) == 0) {
		vector<AreaInfo*>::iterator it;

		int i=0;
		for (it = this->areas.begin(); it != this->areas.end(); ++it) {
			AreaInfo* area = *it;

			sprintf(object_name, "area%d", i++);
			MyString area_json = area->toJson();
			areadoc.Parse(area_json.c_str());
			if (areadoc.HasParseError()) {
				rapidjson::ParseErrorCode code = areadoc.GetParseError();
				LOG_INFO("JSON parsing error\n");
				return 0;
			}
			obj.CopyFrom(areadoc, areadoc.GetAllocator());

			AreaObj.AddMember(Value(object_name, AreaObj.GetAllocator()).Move(),
					obj.Move(), AreaObj.GetAllocator());
		}
		pthread_mutex_unlock(&areas_mutex);
	}

	document.AddMember(Value("Areas", document.GetAllocator()).Move(),
			AreaObj.GetObject(), document.GetAllocator());

	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	document.Accept(writer);

//	LOG_DEBUG("device area:\n%s\n", buffer.GetString());

	return MyString(buffer.GetString());
}
#endif


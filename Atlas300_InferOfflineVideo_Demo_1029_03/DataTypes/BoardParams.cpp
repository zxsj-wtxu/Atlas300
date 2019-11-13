/*
 * BoardParams.cpp
 *
 *  Created on: 2019年7月20日
 *      Author: wtxu
 */

#include "BoardParams.h"
int BoardParams::graphid = 100;
BoardParams* BoardParams::_this = nullptr;
pthread_mutex_t BoardParams::mutex_this = PTHREAD_MUTEX_INITIALIZER;

BoardParams::BoardParams() {
	// TODO Auto-generated constructor stub

}

BoardParams* BoardParams::Instance(){
	if(pthread_mutex_lock(&mutex_this) == 0){
		if(_this == nullptr)
			_this = new BoardParams;
		pthread_mutex_unlock(&mutex_this);
	}
	return _this;
}

void BoardParams::Release(){
	printf("hi3559a->Exit(); done.\n");

	if(pthread_mutex_lock(&mutex_this) == 0){
		if(_this != nullptr)
			delete _this;
		pthread_mutex_unlock(&mutex_this);
	}
}

BoardParams::~BoardParams() {
	// TODO Auto-generated destructor stub
	map<MyString, DeviceDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_deviceinfos) == 0){
		for(it=deviceinfos.begin(); it!=deviceinfos.end();){
			if(it->second != nullptr)
				delete it->second;
			it = deviceinfos.erase(it);
		}
		pthread_mutex_unlock(&mutex_deviceinfos);
	}
	map<MyString, SystemDatabase*>::iterator it_sys;
	if(pthread_mutex_lock(&mutex_sysdatas) == 0){
		for(it_sys=sysdatas.begin(); it_sys!=sysdatas.end();){
			if(it_sys->second != nullptr)
				delete it_sys->second;
			it_sys = sysdatas.erase(it_sys);
		}
		pthread_mutex_unlock(&mutex_sysdatas);
	}
}

int BoardParams::queryDeviceParam(DeviceInfo& device){
	int ret = 0;
	map<MyString, DeviceDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_deviceinfos) == 0){
		it = deviceinfos.find(device.getDeivceCode());
		if(it != deviceinfos.end()){
			if(it->second != nullptr){
				DeviceDatabase& temp = *(it->second);
				device.setJson(temp.toJson().c_str());
				ret = 0;
			}else{
				ret = -1;
			}
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_deviceinfos);
	}
	return ret;
}

int BoardParams::queryDeviceParam(MyString deviceid, DeviceInfo& device){
	int ret = 0;
	map<MyString, DeviceDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_deviceinfos) == 0){
		it = deviceinfos.find(deviceid);
		if(it != deviceinfos.end()){
			if(it->second != nullptr){
				DeviceDatabase& temp = *(it->second);
				device.setJson(temp.toJson().c_str());
				ret = 0;
			}else{
				ret = -1;
			}
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_deviceinfos);
	}
	return ret;
}

int BoardParams::querySystemParam(SystemInfo& sysinfo){
	int ret = 0;
	map<MyString, SystemDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_sysdatas) == 0){
		it = sysdatas.find(sysinfo.getSystemId());
		if(it != sysdatas.end()){
			if(it->second != nullptr){
				SystemDatabase& temp = *(it->second);
				sysinfo.setJson(temp.toJson());
				ret = 0;
			}else{
				ret = -1;
			}
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_sysdatas);
	}
	return ret;
}

int BoardParams::querySystemParam(MyString systemid, SystemInfo& sysinfo){
	int ret = 0;
	map<MyString, SystemDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_sysdatas) == 0){
		it = sysdatas.find(systemid);
		if(it != sysdatas.end()){
			if(it->second != nullptr){
				SystemDatabase& temp = *(it->second);
				sysinfo.setJson(temp.toJson());
				ret = 0;
			}else{
				ret = -1;
			}
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_sysdatas);
	}
	return ret;
}

int BoardParams::getDevicesIdCode(vector<MyString>& devicesid){
	int ret = 0;
	map<MyString, DeviceDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_deviceinfos) == 0){
		for(it=deviceinfos.begin(); it!=deviceinfos.end();it++){
			devicesid.push_back(it->second->getDeivceCode());
		}
		pthread_mutex_unlock(&mutex_deviceinfos);
	}
	return 0;
}

int BoardParams::removeSystemParam(MyString systemid){
	int ret = 0;
	map<MyString, SystemDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_sysdatas) == 0){
		it = sysdatas.find(systemid);
		if(it != sysdatas.end()){
			if(it->second != nullptr){
				DatabaseController::Instance()->Delete(it->second);
				delete it->second;
				it = sysdatas.erase(it);
				ret = 0;
			}else{
				ret = -1;
			}
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_sysdatas);
	}
	return ret;
}

int BoardParams::removeDeviceParam(MyString deviceid){
	int ret = 0;
	DeviceDatabase* device = NULL;
	map<MyString, DeviceDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_deviceinfos) == 0){
		it = deviceinfos.find(deviceid);
		if(it != deviceinfos.end()){
			if(it->second != nullptr){
				DatabaseController::Instance()->Delete(it->second);
				device = new DeviceDatabase(*it->second);
				delete it->second;
				it = deviceinfos.erase(it);
				ret = 0;
			}else{
				printf("remove failed. DeviceDatabase is NULL\n");
				ret = -1;
			}
		}else{
			printf("remove failed. cannot find deviceid %s\n", deviceid.c_str());
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_deviceinfos);
	}
	if(device == NULL){
		return ret;
	}
	//删除区域配置
    Toolset::Instance()->removeDeviceParam(device->getDeivceCode());
	delete device;

    unsigned int channelid = device->getDeivceCode();
    DestroyGraph(channelid);

	return ret;
}

int BoardParams::updateDeviceParam(MyString DeviceDatabase_tablename, const MyString& json){
	int ret = -1;
	DeviceDatabase* device = new DeviceDatabase(DeviceDatabase_tablename, json.c_str());
	if(DatabaseController::Instance()->Update(device) < 0){
		delete device;
		return -1;
	}
	map<MyString, DeviceDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_deviceinfos) == 0){
		it = deviceinfos.find(device->getDeivceCode());
		if(it != deviceinfos.end()){
			if(it->second != nullptr){
				delete it->second;
				it->second = device;
				ret = 0;
			}else{
				ret = -1;
			}
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_deviceinfos);
	}
	//3、更新系统的区域配置
	Toolset::Instance()->registCalibrationInfo(device->getDeivceCode(), "bicycle", 1920, 1080);
	int size = device->areaSize();
	for(int i=0; i<size; i++){
		vector<cv::Point> points;
		vector<AreaInfo::POINT> temp;
		AreaInfo* area = (*device)[i];
		if(area != NULL){
			if(area->getPoints(temp) < 0){
				LOG_ERROR("get area points failed.\n");
			}else{
				for(int i=0; i<temp.size(); i++){
					points.push_back(cv::Point(temp[i].x, temp[i].y));
				}
				Toolset::Instance()->updateArea(device->getDeivceCode(), "bicycle", area->getType(), points);
			}
		}else{
			LOG_ERROR("area is NULL.\n");
		}
	}
	char filename[20] = {0};
	sprintf(filename, "temp%d.jpg", device->getDeivceCode());

	if(!Toolset::Instance()->toImageFile(device->getDeivceCode(), "bicycle", filename)){
		LOG_ERROR("write image failed.\n");
	}

	return ret;
}

int BoardParams::updateSystemParam(MyString SystemDatabase_tablename, const MyString& json){
	int ret = -1;
	SystemDatabase* sysdata = new SystemDatabase(SystemDatabase_tablename, json.c_str());
	if(DatabaseController::Instance()->Update(sysdata) < 0){
		delete sysdata;
		return -1;
	}
	map<MyString, SystemDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_sysdatas) == 0){
		it = sysdatas.find(sysdata->getSystemId());
		if(it != sysdatas.end()){
			if(it->second != nullptr){
				delete it->second;
				it->second = sysdata;
				ret = 0;
			}else{
				ret = -1;
			}
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_sysdatas);
	}

	return ret;
}

int BoardParams::addSystemParam(MyString SystemDatabase_tablename, const MyString& json){
	int ret = -1;
	SystemDatabase* sys = new SystemDatabase(SystemDatabase_tablename, json.c_str());
	if(DatabaseController::Instance()->Insert(sys) < 0){
		delete sys;
		return -1;
	}
	map<MyString, SystemDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_sysdatas) == 0){
		it = sysdatas.find(sys->getSystemId());
		if(it != sysdatas.end()){
			if(it->second != nullptr){
				delete it->second;
				it->second = sys;
				ret = 0;
			}else{
				ret = -1;
			}
		}else{
			sysdatas.insert(map<MyString, SystemDatabase*>::value_type(sys->getSystemId(), sys));
			ret = 0;
		}
		pthread_mutex_unlock(&mutex_sysdatas);
	}

	return ret;
}

int BoardParams::addDeviceParam(MyString DeviceDatabase_tablename, const MyString& json){
	int ret = -1;
	//1、将数据写入数据库
	DeviceDatabase* device = new DeviceDatabase(DeviceDatabase_tablename, json.c_str());
	if(DatabaseController::Instance()->Insert(device) < 0){
		delete device;
		return -1;
	}
	//2、保存数据至系统内存中，动态条用
	map<MyString, DeviceDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_deviceinfos) == 0){
		it = deviceinfos.find(device->getDeivceCode());
		if(it != deviceinfos.end()){
			if(it->second != nullptr){
				delete it->second;
				it->second = device;
				ret = 0;
			}else{
				ret = -1;
			}
		}else{
			deviceinfos.insert(map<MyString, DeviceDatabase*>::value_type(device->getDeivceCode(), device));
			ret = 0;
		}
		pthread_mutex_unlock(&mutex_deviceinfos);
	}
	//3、更新系统的区域配置
	Toolset::Instance()->registCalibrationInfo(device->getDeivceCode(), "bicycle", 1920, 1080);
	int size = device->areaSize();
	for(int i=0; i<size; i++){
		vector<cv::Point> points;
		vector<AreaInfo::POINT> temp;
		AreaInfo* area = (*device)[i];
		if(area != NULL){
			if(area->getPoints(temp) < 0){
				LOG_ERROR("get area points failed.\n");
			}else{
				for(int i=0; i<temp.size(); i++){
					points.push_back(cv::Point(temp[i].x, temp[i].y));
				}
				Toolset::Instance()->addArea(device->getDeivceCode(), "bicycle", area->getType(), points);
			}
		}else{
			LOG_ERROR("area is NULL.\n");
		}
	}
	char filename[20] = {0};
	sprintf(filename, "temp%d.jpg", device->getDeivceCode());

	if(!Toolset::Instance()->toImageFile(device->getDeivceCode(), "bicycle", filename)){
		LOG_ERROR("write image failed.\n");
	}
	MyString url = (*device)["sourceurl"].GetString();
	cout << "source url:" << url << ", deviceId:"<<device->getDeivceCode() <<endl;

    int channelid = device->getDeivceCode();
    graphid = CreateGraph(graphid, channelid, url.c_str());
	return ret;
}

int BoardParams::initLoadParam(MyString DeviceDatabase_tablename){
	DeviceDatabase db(DeviceDatabase_tablename, 1);
	if(DatabaseController::Instance()->CreateTable(&db) < 0){
		printf("%s exist.\n", DeviceDatabase_tablename.c_str());
	}
	vector<DataBaseContainer*> containers;
	DatabaseController::Instance()->Export(DeviceDatabase_tablename, containers);
	if(containers.size() == 0){
		printf("%s is empty.\n", DeviceDatabase_tablename.c_str());
		return -1;
	}
	if(pthread_mutex_lock(&mutex_deviceinfos) == 0){
		map<const MyString, MyString>::iterator it;
		map<MyString, DeviceDatabase*>::iterator it_deviceinfos;
		for(it_deviceinfos=deviceinfos.begin(); it_deviceinfos!=deviceinfos.end(); ){
			if(it_deviceinfos->second != nullptr)
				delete it_deviceinfos->second;
			it_deviceinfos = deviceinfos.erase(it_deviceinfos);
		}
		for(int i=0; i<containers.size(); i++){
			DataBaseContainer& temp = *containers[i];
			map<const MyString, MyString> key_values = temp.getData();
			for(it=key_values.begin(); it!=key_values.end();++it){
				DeviceDatabase* db_temp = new DeviceDatabase(DeviceDatabase_tablename, it->first.toInt());
				if(db_temp->setJson(it->second.c_str()) < 0){
					LOG_ERROR("set device database failed.\n");
				}
				deviceinfos.insert(map<MyString, DeviceDatabase*>::value_type(it->first, db_temp));
			}
			delete containers[i];
		}
		pthread_mutex_unlock(&mutex_deviceinfos);
	}

	char filename[20] = {0};
	map<MyString, DeviceDatabase*>::iterator it;
	for(it = deviceinfos.begin(); it!=deviceinfos.end(); it++){
		DeviceDatabase* device = it->second;

		sprintf(filename, "temp%s.jpg", it->first.c_str());
        if(Toolset::Instance()->registCalibrationInfo(device->getDeivceCode(), "bicycle", 1920, 1080) < 0){
            std::cout << "registCalibrationInfo failed" << std::endl;
        }
		int size = device->areaSize();
        LOG_DEBUG("device id:%d, area size:%d\n", device->getDeivceCode(), size);
		for(int i=0; i<size; i++){
			vector<cv::Point> points;
			vector<AreaInfo::POINT> temp;
			AreaInfo* area = (*device)[i];
			if(area != NULL){
				if(area->getPoints(temp) < 0){
					LOG_ERROR("get area points failed.\n");
				}else{
					for(int i=0; i<temp.size(); i++){
						points.push_back(cv::Point(temp[i].x, temp[i].y));
					}
					Toolset::Instance()->addArea(device->getDeivceCode(), "bicycle", area->getType(), points);
				}
			}else{
				LOG_ERROR("area is NULL.\n");
			}
		}

		if(!Toolset::Instance()->toImageFile(device->getDeivceCode(), "bicycle", filename)){
			LOG_ERROR("write image failed.\n");
		}

        unsigned int channelid = device->getDeivceCode();
        MyString url = (*device)["sourceurl"].GetString();
        graphid = CreateGraph(graphid, channelid, url.c_str());
	}
	return 0;
}

int BoardParams::initLoadSystemParam(MyString SystemDatabase_tablename){
	int ret = -1;
	SystemDatabase sysdb(SystemDatabase_tablename, MyString("3559a"));
	if(DatabaseController::Instance()->CreateTable(&sysdb) < 0){
		printf("%s exist.\n", SystemDatabase_tablename.c_str());
	}

	vector<DataBaseContainer*> containers;
	if(DatabaseController::Instance()->Export(SystemDatabase_tablename, containers) < 0){
		return -1;
	}
	if(containers.size() == 0){
		return -1;
	}
	if(pthread_mutex_lock(&mutex_sysdatas) == 0){
		map<const MyString, MyString>::iterator it;
		map<MyString, SystemDatabase*>::iterator it_sysdatas;
		for(it_sysdatas=sysdatas.begin(); it_sysdatas!=sysdatas.end(); ){
			if(it_sysdatas->second != nullptr)
				delete it_sysdatas->second;
			it_sysdatas = sysdatas.erase(it_sysdatas);
		}
		for(int i=0; i<containers.size(); i++){
			DataBaseContainer& temp = *containers[i];
			map<const MyString, MyString> key_values = temp.getData();
			for(it=key_values.begin(); it!=key_values.end();++it){
				SystemDatabase* db_temp = new SystemDatabase(SystemDatabase_tablename, it->first);
				db_temp->setJson(it->second.c_str());
				cout << it->first <<" : " << db_temp->toJson() <<endl;
				sysdatas.insert(map<MyString, SystemDatabase*>::value_type(it->first, db_temp));
			}
			delete containers[i];
		}
		pthread_mutex_unlock(&mutex_sysdatas);
	}
	return ret;
}

MyString BoardParams::getDeviceId(int VdecChn){
    MyString deviceid;
    map<const MyString, MyString>::iterator it_deviceid_vdec_match;
    if(pthread_mutex_lock(&mutex_deviceid_vdec_match) == 0){
        it_deviceid_vdec_match = deviceid_vdec_match.find(VdecChn);
        if(it_deviceid_vdec_match != deviceid_vdec_match.end()){
            deviceid = it_deviceid_vdec_match->second;
        }else{
            LOG_ERROR("getDeviceId failed\n");
        }
        pthread_mutex_unlock(&mutex_deviceid_vdec_match);
    }
    return deviceid;
}
MyString BoardParams::toString(){
	MyString str;
	map<MyString, DeviceDatabase*>::iterator it;
	if(pthread_mutex_lock(&mutex_deviceinfos) == 0){
		for(it=deviceinfos.begin(); it!=deviceinfos.end(); ++it){
			DeviceDatabase* temp = it->second;
			str += temp->toJson();
			str += "\n";
		}
		pthread_mutex_unlock(&mutex_deviceinfos);
	}
	map<MyString, SystemDatabase*>::iterator it_sysdatas;
	if(pthread_mutex_lock(&mutex_sysdatas) == 0){
		for(it_sysdatas=sysdatas.begin(); it_sysdatas!=sysdatas.end(); ++it_sysdatas){
			SystemDatabase* temp = it_sysdatas->second;
			str += temp->toJson();
			str += "\n";
		}
		pthread_mutex_unlock(&mutex_sysdatas);
	}
	return str;
}

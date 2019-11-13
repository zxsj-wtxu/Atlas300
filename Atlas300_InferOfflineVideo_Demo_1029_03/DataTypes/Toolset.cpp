/*
 * Toolset.cpp
 *
 *  Created on: 2019年7月13日
 *      Author: wtxu
 */

#include "Toolset.h"
Toolset* Toolset::ins_toolset = NULL;
pthread_mutex_t Toolset::mutex_ins_toolset = PTHREAD_MUTEX_INITIALIZER;

Toolset::Toolset(){

}

Toolset::~Toolset(){
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		for(it=devices_calinfo.begin(); it != devices_calinfo.end();){
			if(it->second != NULL)
				delete it->second;
			it = devices_calinfo.erase(it);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	cout << "~Toolset" <<endl;
}

Toolset* Toolset::Instance(){
	if(pthread_mutex_lock(&mutex_ins_toolset) == 0){
		if(ins_toolset == NULL){
			ins_toolset = new Toolset;
		}
		pthread_mutex_unlock(&mutex_ins_toolset);
	}
	return ins_toolset;
}

void Toolset::Destroy(){
	if(pthread_mutex_lock(&mutex_ins_toolset) == 0){
		if(ins_toolset != NULL){
			cout << "Toolset::Destroy" <<endl;
			delete ins_toolset;
		}
		pthread_mutex_unlock(&mutex_ins_toolset);
	}
}

double Toolset::RectOverlap(cv::Rect& r, cv::Rect& rOther)
{
	int x0 = std::max(r.x , rOther.x);
	int x1 = std::min(r.x + r.width, rOther.x + rOther.width);
	int y0 = std::max(r.y, rOther.y);
	int y1 = std::min(r.y + r.height, rOther.y + rOther.height);

	if (x0 >= x1 || y0 >= y1) return 0.f;

	double areaInt = (x1-x0)*(y1-y0);
	return areaInt/((double)r.width*r.height+(double)rOther.width*rOther.height-areaInt);
}

bool Toolset::toImageFile(const MyString& deviceid, const MyString& areatype, const MyString& filepath){
	bool ret = false;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = false;
		}else{
			ret = it->second->toFile(areatype, filepath);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::clear(const MyString& deviceid, const MyString& areatype){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = -1;
		}else{
			ret = it->second->clear(areatype);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::removeDeviceParam(const MyString& deviceid){
    int ret = 0;
    map<const MyString, CalibrationInfo*>::iterator it;
    if(pthread_mutex_lock(&mutex_images) == 0){
        it = devices_calinfo.find(deviceid);
        if(it == devices_calinfo.end()){
            ret = -1;
        }else{
            delete it->second;
            it = devices_calinfo.erase(it);
        }
        pthread_mutex_unlock(&mutex_images);
    }
    return ret;
}

int Toolset::removeArea(const MyString& deviceid, const MyString& areatype, int value){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = -1;
		}else{
			ret = it->second->removeArea(areatype, value);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::AreaValue(const MyString& deviceid, const MyString& areatype, int x, int y){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = -1;
//            printf("cannot find %s deviceid.\n", deviceid.c_str());
		}else{
			ret = it->second->AreaValue(areatype, x,y);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::addArea(const MyString& deviceid, const MyString& areatype, int value, const vector<cv::Point>& points){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = -1;
		}else{
			ret = it->second->addArea(areatype, value, points);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::updateArea(const MyString& deviceid, const MyString& areatype, int value, const vector<cv::Point>& points){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			ret = -1;
		}else{
			ret = it->second->updateArea(areatype, value, points);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int Toolset::registCalibrationInfo(const MyString& deviceid, const MyString& areatype, int width, int height){
	int ret = 0;
	map<const MyString, CalibrationInfo*>::const_iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = devices_calinfo.find(deviceid);
		if(it == devices_calinfo.end()){
			devices_calinfo.insert(map<const MyString, CalibrationInfo*>::value_type(deviceid, new CalibrationInfo(deviceid, areatype, width, height)));
			ret = 0;
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_images);
	}
	cout << "registCalibrationInfo done" <<endl;
	return ret;
}

void Toolset::GetNowTime(char* timE)
{
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(timE,48,"%Y-%m-%d %H:%M:%S\n",timeinfo);
}

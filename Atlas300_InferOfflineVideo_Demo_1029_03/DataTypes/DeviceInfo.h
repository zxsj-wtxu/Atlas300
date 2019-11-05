/*
 * DeviceInfo.h
 *
 *  Created on: 2019年7月9日
 *      Author: wtxu
 */

#ifndef DEVICEINFO_H_
#define DEVICEINFO_H_

#include "AreaInfo.h"

class DeviceInfo : public BaseClass{
private:
	int deviceid = -1;
	int areaid = 0;

	vector<AreaInfo*> areas;
	pthread_mutex_t areas_mutex = PTHREAD_MUTEX_INITIALIZER;

	const int inserArea(AreaInfo* area, int type=-1);

public:
	DeviceInfo();
	DeviceInfo(int deviceid);
	DeviceInfo(const DeviceInfo& device);
	virtual ~DeviceInfo();

	int areaSize();
	int getDeviceCode();

	AreaInfo* operator[](int type);
//	using BaseClass::operator[];

	DeviceInfo& operator=(const DeviceInfo& device);

	int getDeivceCode();
	int clearAreas();
	int removeArea(int type);
	int createArea(const vector<AreaInfo::POINT>& points,  int type=-1);
public:
#ifdef RAPIDJSON
	const MyString toJson();
	int setJson(const char* json);
	DeviceInfo(const char* json);
#endif
};
#endif /* DEVICEINFO_H_ */

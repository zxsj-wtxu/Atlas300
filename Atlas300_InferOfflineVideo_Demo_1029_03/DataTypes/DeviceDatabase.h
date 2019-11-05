/*
 * DeviceDatabase.h
 *
 *  Created on: 2019年7月12日
 *      Author: wtxu
 */

#ifndef DEVICEDATABASE_H_
#define DEVICEDATABASE_H_

#include "DeviceInfo.h"
#include "DataBaseContainer.h"

class DeviceDatabase: public DeviceInfo, public DataBaseContainer {
public:
	DeviceDatabase(const MyString& tablename, const char* json);
	DeviceDatabase(const MyString& tablename, int deviceid);
//	DeviceDatabase(const DeviceDatabase& device);
	virtual ~DeviceDatabase();

//	DeviceDatabase& operator=(const DeviceDatabase& device);
	int insertToDatabase(const MyString& json);
	int setJson(const char* json);

	int clear();
	int remove(const MyString& key);
	int insertKeyValue(const MyString& key, int value);
	int insertKeyValue(const MyString& key, double value);
	int insertKeyValue(const MyString& key, long int value);
	int insertKeyValue(const MyString& key, MyString value);

	int clearAreas();
	int removeArea(int type);
	int createArea(const vector<AreaInfo::POINT>& points,  int type);

	using DeviceInfo::operator[];
	using BaseClass::operator[];
//	using DataBaseContainer::operator[];
};

#endif /* DEVICEDATABASE_H_ */

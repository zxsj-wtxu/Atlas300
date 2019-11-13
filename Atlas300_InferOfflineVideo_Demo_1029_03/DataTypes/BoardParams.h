/*
 * BoardParams.h
 *
 *  Created on: 2019年7月20日
 *      Author: wtxu
 */

#ifndef BOARDPARAMS_H_
#define BOARDPARAMS_H_
#include "Toolset.h"
#include "DeviceDatabase.h"
#include "SystemDatabase.h"
#include "DatabaseController.h"
#include "graphmanager.h"
#include "bicycledetectionapi.h"

class BoardParams {
public:
	BoardParams();
	virtual ~BoardParams();
	static BoardParams* Instance();
	static void Release();

	//获取所有设备的id标识
	int getDevicesIdCode(vector<MyString>& devicesid);
	//增加

	int addDeviceParam(MyString DeviceDatabase_tablename, const MyString& json);
	int addSystemParam(MyString SystemDatabase_tablename, const MyString& json);
	//删除
	int removeDeviceParam(MyString deviceid);
	int removeSystemParam(MyString systemid);
	//查询
	int queryDeviceParam(DeviceInfo& device);
	int queryDeviceParam(MyString deviceid, DeviceInfo& device);
	int querySystemParam(SystemInfo& sysinfo);
	int querySystemParam(MyString systemid, SystemInfo& sysinfo);
	//修改
	int updateDeviceParam(MyString DeviceDatabase_tablename, const MyString& json);
	int updateSystemParam(MyString SystemDatabase_tablename, const MyString& json);
	//从数据库加载设备数据
	int initLoadParam(MyString DeviceDatabase_tablename);
	int initLoadSystemParam(MyString SystemDatabase_tablename);

	MyString getDeviceId(int VdecChn);
	MyString toString();
private:
	static BoardParams* _this;
	static pthread_mutex_t mutex_this;

    static int graphid;

	map<MyString, DeviceDatabase*> deviceinfos;
	pthread_mutex_t mutex_deviceinfos = PTHREAD_MUTEX_INITIALIZER;

	map<MyString, SystemDatabase*> sysdatas;
	pthread_mutex_t mutex_sysdatas = PTHREAD_MUTEX_INITIALIZER;

	map<const MyString, MyString> deviceid_vdec_match;
	pthread_mutex_t mutex_deviceid_vdec_match = PTHREAD_MUTEX_INITIALIZER;
};

#endif /* BOARDPARAMS_H_ */

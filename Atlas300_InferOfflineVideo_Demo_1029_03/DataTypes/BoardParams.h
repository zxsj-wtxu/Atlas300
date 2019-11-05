/*
 * BoardParams.h
 *
 *  Created on: 2019年7月20日
 *      Author: wtxu
 */

#ifndef BOARDPARAMS_H_
#define BOARDPARAMS_H_
#include "Hi3559A.h"
#include "Toolset.h"
#include "DeviceDatabase.h"
#include "SystemDatabase.h"
#include "DatabaseController.h"
#include "VencThreadParams.h"
#include "VpssThreadParams.h"
#include "NnieDetectParams.h"
#include "ffmpegCodec.h"
#include "BicycleDetection.h"
#include "NnieTaskScheduler.hpp"
#include "OsdThreadParams.h"
#include "RgnManager.h"

class BoardParams {
public:
	BoardParams();
	virtual ~BoardParams();
	static BoardParams* Instance();
	static void Release();

	int CreateVdecDetect(int vdecid);
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
	int Hi3559a_CreateOsdThread(int VdecChn);
	int Hi3559a_CancelOsdThread(int VdecChn);
	int Hi3559a_InitParams(int maxchn, int maxCore, int width, int height);
	int getVencThreadParams(VencThreadParams& arg);
	int getVpssThreadParams(VpssThreadParams& arg);
	int getNnieThreadParams(int VdecChn, NnieDetectParams& arg);
	int Hi3559a_DeleteChannel(MyString deviceid, int VdecChn);
	int Hi3559a_AddChannel(MyString deviceid, int VdecChn, MyString url);
	int createChannel(MyString deviceid, int VdecChn, int rtsp_fd);
	int removeChannel(MyString deviceid, int VdecChn, int rtsp_fd);
	int Copy_Ive_Image(int VdecChn, unsigned char* addr, int chn, int offset_stride, HI_U64& pts);
	int Ive_Image_Alloc(IVE_DST_IMAGE_S** pstDst, IVE_IMAGE_TYPE_E  enType, int width, int height, int stride);
	int Ive_Image_CSC(int VdecChn, VIDEO_FRAME_S* stVFrame, IVE_IMAGE_TYPE_E  enType, IVE_CSC_MODE_E enMode);
	MyString toString();
private:
	static BoardParams* _this;
	static pthread_mutex_t mutex_this;

	map<MyString, DeviceDatabase*> deviceinfos;
	pthread_mutex_t mutex_deviceinfos = PTHREAD_MUTEX_INITIALIZER;

	map<MyString, SystemDatabase*> sysdatas;
	pthread_mutex_t mutex_sysdatas = PTHREAD_MUTEX_INITIALIZER;

	map<const MyString, MyString> deviceid_vdec_match;
	pthread_mutex_t mutex_deviceid_vdec_match = PTHREAD_MUTEX_INITIALIZER;

	ffmpegCodec* ffmpeg;
	VencThreadParams* vencthreadargs = NULL;
	VpssThreadParams* vpssthreadargs = NULL;
	NnieTaskScheduler<int, vector<SsdResults>>* taskscheduler = NULL;

	map<MyString, NnieDetectParams*> pstDst;
	pthread_mutex_t mutex_pstDst = PTHREAD_MUTEX_INITIALIZER;

	map<MyString, OsdThreadParams*> osd_threads;
	pthread_mutex_t mutex_osd_threads = PTHREAD_MUTEX_INITIALIZER;


	static void* Osd(void* arg);
	static void* Osd_WithReverse(void* arg);
};

#endif /* BOARDPARAMS_H_ */

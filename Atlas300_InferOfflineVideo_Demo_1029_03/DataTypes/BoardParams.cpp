/*
 * BoardParams.cpp
 *
 *  Created on: 2019年7月20日
 *      Author: wtxu
 */

#include "BoardParams.h"
Hi3559A* hi3559a = NULL;
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
	hi3559a->Exit();
	printf("hi3559a->Exit(); done.\n");

	if(pthread_mutex_lock(&mutex_this) == 0){
		if(_this != nullptr)
			delete _this;
		pthread_mutex_unlock(&mutex_this);
	}
}

BoardParams::~BoardParams() {
	// TODO Auto-generated destructor stub
	taskscheduler->EventLoopStop();
	map<const MyString, MyString>::iterator it_vdec = deviceid_vdec_match.begin();
	for(it_vdec; it_vdec!=deviceid_vdec_match.end(); it_vdec++){
		cout << "VdecChn:" <<it_vdec->first.toInt()<<endl;
		Hi3559a_CancelOsdThread(it_vdec->first.toInt());
	}

	for(it_vdec = deviceid_vdec_match.begin(); it_vdec!=deviceid_vdec_match.end(); it_vdec++){
		cout << "VdecChn:" <<it_vdec->first.toInt()<<endl;
	    taskscheduler->removeTask(it_vdec->second.toInt());
	    hi3559a->removeChannel((*vpssthreadargs)[it_vdec->first.toInt()], (*vencthreadargs)[it_vdec->first.toInt()]);
		removeChannel(it_vdec->second.toInt(), it_vdec->first.toInt(), (*vencthreadargs)[it_vdec->first.toInt()]->pipe_fd);
	    ffmpeg->remove(it_vdec->first.toInt());
	}
	printf("hi3559a release; done.\n");
	hi3559a->SystemDeinit();

	if(ffmpeg != NULL){
		ffmpeg->exit();
		delete ffmpeg;
		ffmpeg = NULL;
	}

	if(vencthreadargs != NULL){
		delete vencthreadargs;
		vencthreadargs = NULL;
	}

	if(vpssthreadargs != NULL){
		delete vpssthreadargs;
		vpssthreadargs = NULL;
	}

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

	if(hi3559a != NULL)
		delete hi3559a;
}

int BoardParams::Ive_Image_Alloc(IVE_DST_IMAGE_S** pstDst, IVE_IMAGE_TYPE_E  enType, int width, int height, int stride){
	(*pstDst) = (IVE_DST_IMAGE_S*)malloc(sizeof(IVE_DST_IMAGE_S));
    (*pstDst)->enType = enType;
    (*pstDst)->u32Width = width;
    (*pstDst)->u32Height = height;
    (*pstDst)->au32Stride[0] = stride;
    (*pstDst)->au32Stride[1] = stride;
    (*pstDst)->au32Stride[2] = stride;

    HI_U32 pstDst_offset = (*pstDst)->au32Stride[0] * (*pstDst)->u32Height;
    HI_U32 pstDst_Size = pstDst_offset * 3;

    HI_U32 s32Ret = HI_MPI_SYS_MmzAlloc(&(*pstDst)->au64PhyAddr[0], (HI_VOID**)&(*pstDst)->au64VirAddr[0], NULL, HI_NULL, pstDst_Size);
    if (s32Ret != HI_SUCCESS) {
    	LOG_ERROR("Mmz Alloc fail,Error(%#x)", s32Ret);
        return s32Ret;
    }
    (*pstDst)->au64PhyAddr[1] = (*pstDst)->au64PhyAddr[0] + pstDst_offset;
    (*pstDst)->au64PhyAddr[2] = (*pstDst)->au64PhyAddr[1] + pstDst_offset;
    (*pstDst)->au64VirAddr[1] = (*pstDst)->au64VirAddr[0] + pstDst_offset;
    (*pstDst)->au64VirAddr[2] = (*pstDst)->au64VirAddr[1] + pstDst_offset;

    return 0;
}

int BoardParams::Copy_Ive_Image(int VdecChn, unsigned char* addr, int chn, int offset_stride, HI_U64& pts){
	static int imageid = 0;
	HI_U32 s32Ret;
	unsigned char* pu8PicAddr = addr;
    map<MyString, NnieDetectParams*>::iterator it;
    if(pthread_mutex_lock(&mutex_pstDst) == 0){
    	it = this->pstDst.find(VdecChn);
    	if(it != this->pstDst.end()){
    		if(it->second == NULL || it->second->image == NULL){
    			s32Ret = -1;
    		}else{
        		NnieDetectParams& temp = *it->second;
        		pts = temp.u64PTS;
    			for(int i = 0; i < chn; i++) {
    				memcpy(pu8PicAddr, (uchar*)temp.image->au64VirAddr[i], offset_stride);
    				pu8PicAddr += offset_stride;
    			}
    			s32Ret = 0;
    		}
    	}
    	pthread_mutex_unlock(&mutex_pstDst);
    }
    return s32Ret;
}

//转换视频帧的色彩空间至BGR
int BoardParams::Ive_Image_CSC(int VdecChn, VIDEO_FRAME_S* stVFrame, IVE_IMAGE_TYPE_E  enType, IVE_CSC_MODE_E enMode){
	HI_U32 s32Ret;
    HI_U64 *pu64VirAddr0;
    HI_U64 *pu64VirAddr1;

    HI_U32 pstDst_offset;
    IVE_HANDLE IveHandle;
    IVE_SRC_IMAGE_S pstSrc;
    IVE_CSC_CTRL_S pstCscCtrl;
    pstCscCtrl.enMode = enMode;

	pstSrc.enType = enType;

    map<MyString, NnieDetectParams*>::iterator it;
    if(pthread_mutex_lock(&mutex_pstDst) == 0){
    	it = this->pstDst.find(VdecChn);
    	if(it != this->pstDst.end()){
    		NnieDetectParams& temp = *it->second;

    	    pstSrc.u32Width = temp.image->u32Height;
    	    pstSrc.u32Height = temp.image->u32Width;

    	    pstDst_offset = temp.image->au32Stride[0] * temp.image->u32Height;
    	    pu64VirAddr0 = (HI_U64*)HI_MPI_SYS_Mmap(stVFrame->u64PhyAddr[0], pstDst_offset);
    	    stVFrame->u64VirAddr[0] = (HI_U64)(HI_UL)pu64VirAddr0;

    	    pu64VirAddr1 = (HI_U64*)HI_MPI_SYS_Mmap(stVFrame->u64PhyAddr[1], pstDst_offset);
    	    stVFrame->u64VirAddr[1] = (HI_U64)(HI_UL)pu64VirAddr1;

    	    memcpy(pstSrc.au32Stride, stVFrame->u32Stride, sizeof(pstSrc.au32Stride));
    	    memcpy(pstSrc.au64PhyAddr, stVFrame->u64PhyAddr, sizeof(pstSrc.au64PhyAddr));
    	    memcpy(pstSrc.au64VirAddr, stVFrame->u64VirAddr, sizeof(pstSrc.au64VirAddr));

    	    s32Ret = HI_MPI_IVE_CSC(&IveHandle, &pstSrc, temp.image, &pstCscCtrl, HI_FALSE);
    	    if (s32Ret != HI_SUCCESS) {
    	    	LOG_ERROR("HI_MPI_IVE_CSC fail,Error(%#x)", s32Ret);
    	    }
    	    temp.u64PTS = stVFrame->u64PTS;
    	}
    	pthread_mutex_unlock(&mutex_pstDst);
    }

    s32Ret = HI_MPI_SYS_Munmap(pu64VirAddr0, pstDst_offset);
    if (HI_SUCCESS != s32Ret) {
    	LOG_ERROR("Error(%#x), HI_MPI_SYS_Munmap failed!", s32Ret);
    }
    s32Ret = HI_MPI_SYS_Munmap(pu64VirAddr1, pstDst_offset);
    if (HI_SUCCESS != s32Ret) {
    	LOG_ERROR("Error(%#x), HI_MPI_SYS_Munmap failed!", s32Ret);
    }

    return s32Ret;
}

int BoardParams::removeChannel(MyString deviceid, int VdecChn, int rtsp_fd){
	if(vpssthreadargs == nullptr || vencthreadargs == nullptr) {
		return -1;
	}
	if(vpssthreadargs->remove(VdecChn) < 0){
		printf("add %d vpss thread param failed.\n", VdecChn);
		return -1;
	}
	if(vencthreadargs->remove(VdecChn) < 0){
		printf("add %d vpss thread param failed.\n", VdecChn);
		return -1;
	}
    map<MyString, NnieDetectParams*>::iterator it;
    if(pthread_mutex_lock(&mutex_pstDst) == 0){
    	it = pstDst.find(VdecChn);
    	if(it != pstDst.end()){
    		if(it->second != NULL){
        		HI_MPI_SYS_MmzFree(it->second->image->au64PhyAddr[0], (HI_VOID*)&it->second->image->au64VirAddr[0]);
        		delete it->second;
    		}
    		it = pstDst.erase(it);
    	}
    	pthread_mutex_unlock(&mutex_pstDst);
    }
    map<const MyString, MyString>::iterator it_deviceid_vdec_match;
    if(pthread_mutex_lock(&mutex_deviceid_vdec_match) == 0){
    	it_deviceid_vdec_match = deviceid_vdec_match.find(VdecChn);
    	if(it_deviceid_vdec_match != deviceid_vdec_match.end()){
    		it_deviceid_vdec_match = deviceid_vdec_match.erase(it_deviceid_vdec_match);
    	}
    	pthread_mutex_unlock(&mutex_deviceid_vdec_match);
    }
    return 0;
}

MyString BoardParams::getDeviceId(int VdecChn){
	MyString deviceid;
    map<const MyString, MyString>::iterator it_deviceid_vdec_match;
    if(pthread_mutex_lock(&mutex_deviceid_vdec_match) == 0){
    	it_deviceid_vdec_match = deviceid_vdec_match.find(VdecChn);
    	if(it_deviceid_vdec_match != deviceid_vdec_match.end()){
    		deviceid = it_deviceid_vdec_match->second;
    	}
    	pthread_mutex_unlock(&mutex_deviceid_vdec_match);
    }
    return deviceid;
}

//VdecChn必须等于deviceid
int BoardParams::createChannel(MyString deviceid, int VdecChn, int rtsp_fd){

	if(vpssthreadargs == nullptr || vencthreadargs == nullptr) {
		return -1;
	}

	VpssThreadParams::BaseData* bd_vpss = new VpssThreadParams::BaseData;
	bd_vpss->hifb = new HiFrameBuffers(7);
	bd_vpss->VpssGrp_Chn = VdecChn;
	bd_vpss->VpssChn_ToVenc = 0;	//VPSS0通道用于视频播放
	bd_vpss->VpssChn_ToIve  = 2;	//VPSS2通道用于视频转BGR
	bd_vpss->VencChn_ToRtsp = VdecChn;
	bd_vpss->isSendToVenc = true;
	if(vpssthreadargs->add(VdecChn, bd_vpss) < 0){
		printf("add %d vpss thread param failed.\n", VdecChn);
		return -1;
	}

    VencThreadParams::BaseData* bd_venc = new VencThreadParams::BaseData;
    bd_venc->VencChn = VdecChn;
    bd_venc->isSave = false;
    bd_venc->type = SendType::TOMYRTSP;
    bd_venc->pipe_fd = rtsp_fd;
    bd_venc->size = SIZE_S{1920, 1080};
    bd_venc->isBindVpss = false;
    bd_venc->VpssGrp_bind = VdecChn;
    bd_venc->VpssChn_bind = 0;
    if(vencthreadargs->add(VdecChn, bd_venc) < 0){
    	printf("add %d venc thread param failed.\n", VdecChn);
    	if(vpssthreadargs->remove(VdecChn) < 0){
    		printf("remove %d vpss thread param failed.\n", VdecChn);
    	}
    	return -1;
    }
    //添加设备参数与vdec解码通道的映射
    map<const MyString, MyString>::iterator it_deviceid_vdec_match;
    if(pthread_mutex_lock(&mutex_deviceid_vdec_match) == 0){
    	it_deviceid_vdec_match = deviceid_vdec_match.find(VdecChn);
    	if(it_deviceid_vdec_match != deviceid_vdec_match.end()){
    		it_deviceid_vdec_match->second = deviceid;
    	}else{
    		deviceid_vdec_match.insert(map<const MyString, MyString>::value_type(VdecChn, deviceid));
    	}
    	pthread_mutex_unlock(&mutex_deviceid_vdec_match);
    }

    //为nnie识别开辟BGR缓存
    map<MyString, NnieDetectParams*>::iterator it;
    if(pthread_mutex_lock(&mutex_pstDst) == 0){
    	it = pstDst.find(VdecChn);
    	if(it != pstDst.end()){
    		HI_MPI_SYS_MmzFree(it->second->image->au64PhyAddr[0], (HI_VOID*)&it->second->image->au64VirAddr[0]);
    		it = pstDst.erase(it);
    	}
    	NnieDetectParams* temp = new NnieDetectParams;
    	temp->VdecChn = VdecChn;
    	temp->deviceid = deviceid_vdec_match[VdecChn];
    	temp->hi3559a_this = (void*)hi3559a;
    	Ive_Image_Alloc(&temp->image, IVE_IMAGE_TYPE_U8C3_PLANAR, 300, 300, 304);
    	pstDst.insert(map<MyString, NnieDetectParams*>::value_type(VdecChn, temp));
    	pthread_mutex_unlock(&mutex_pstDst);
    }
    return 0;
}

int BoardParams::getVencThreadParams(VencThreadParams& arg){
	if(this->vencthreadargs == NULL){
		return -1;
	}else{
		arg = *this->vencthreadargs;
		return 0;
	}
}

int BoardParams::getVpssThreadParams(VpssThreadParams& arg){
	if(this->vpssthreadargs == NULL){
		return -1;
	}else{
		arg = *this->vpssthreadargs;
		return 0;
	}
}

int BoardParams::getNnieThreadParams(int VdecChn, NnieDetectParams& arg){
	int ret = -1;
    map<MyString, NnieDetectParams*>::iterator it;
    if(pthread_mutex_lock(&mutex_pstDst) == 0){
    	it = pstDst.find(VdecChn);
    	if(it != pstDst.end()){
    		arg = *it->second;
    		ret = 0;
    	}else{
    		ret = -1;
    	}
    	pthread_mutex_unlock(&mutex_pstDst);
    }

    return ret;
}

int BoardParams::Hi3559a_DeleteChannel(MyString deviceid, int VdecChn){
	taskscheduler->EventLoopStop();
	hi3559a->Exit();

//	rtsp_resetChannel((*vencthreadargs)[VdecChn]->pipe_fd);
	Hi3559a_CancelOsdThread(VdecChn);
    taskscheduler->removeTask(deviceid.toInt());
    hi3559a->removeChannel((*vpssthreadargs)[VdecChn], (*vencthreadargs)[VdecChn]);
	removeChannel(deviceid, VdecChn, (*vencthreadargs)[VdecChn]->pipe_fd);
    ffmpeg->remove(VdecChn);

    hi3559a->start();
    taskscheduler->EventLoopStart();
    return 0;
}

int BoardParams::Hi3559a_AddChannel(MyString deviceid, int VdecChn, MyString url){
	taskscheduler->EventLoopStop();
    hi3559a->Exit();
    BicycleDetection::clearToatal(VdecChn);

    char rtsp_chn[100] = {0};
    sprintf(rtsp_chn, "/live/%s", deviceid.c_str());
	int chn = rtsp_createChannel((const char*)rtsp_chn);
	printf("rtsp url: rtsp://ip:5555%s\n",rtsp_chn);
    createChannel(deviceid, VdecChn, chn);
    hi3559a->CreateChannel((*vpssthreadargs)[VdecChn], (*vencthreadargs)[VdecChn]);

    Hi3559a_CreateOsdThread((*vpssthreadargs)[VdecChn]->VpssGrp_Chn);
    hi3559a->start();

    if(strncmp(url.c_str(), "rtsp", 4) == 0){
    	printf("add rtsp decode thread.\n");
    	ffmpeg->add(VdecChn , url.c_str(), Hi3559A::VDEC_SendFrame_API);
    }else{
    	printf("add h264 video file decode thread.\n");
    	ffmpeg->addFileDecodeThread(VdecChn, 1920, 1080, url.c_str(), Hi3559A::VDEC_SendFrame_API);
    }
	taskscheduler->insertTask(deviceid.toInt(), VdecChn);

	taskscheduler->EventLoopStart();
    return 0;
}
void* BoardParams::Osd(void* arg){
	OsdThreadParams* osdparam = (OsdThreadParams*)arg;

	int VdecChn = osdparam->VdecChn;
	OSDIMAGE _osdimage;
	int res = 0;
	char text[1024] = {0};

	Toolset::Instance()->ConvertTimeToImage(_osdimage, 36);

	RGNIMAGE image(NULL, _osdimage.width, _osdimage.height, 0);
	RGNIMAGE image_res(NULL, 1000, 36, 0);

	res = RgnManager::CreateRegionToVpss(image, VdecChn, 0, 0, 0, 0x7fff, 0, 0);
	if(res < 0){
		printf("create rgn to venc failed.\n");
	}
	res = RgnManager::CreateRegionToVpss(image_res, VdecChn, 0, 0, 36, 0x7fff, 0, 0);
	if(res < 0){
		printf("create rgn to venc failed.\n");
	}

	while(true){
		if(osdparam->isStop){
			RgnManager::DestroyRegions(image.Handle);
			RgnManager::ReleaseBitmap(image);
			RgnManager::DestroyRegions(image_res.Handle);
			RgnManager::ReleaseBitmap(image_res);

			Toolset::Instance()->ReleaseOsdImage(_osdimage);
			printf("Osd %d thread exit.\n", VdecChn);
			pthread_exit(NULL);
		}
		memset(text, 0, sizeof(text));
		sprintf(text, "Chn:%d, count1:%d , count2:%d", VdecChn, BicycleDetection::getCount1Total(VdecChn), BicycleDetection::getCount2Total(VdecChn));

		RgnManager::InitBitmap(image_res, 0);
		OSDIMAGE _osdimage_res;
		res = Toolset::Instance()->ConvertCharToImage(text, strlen(text), _osdimage_res, 36);

		if(res >= 0) {
			if(!RgnManager::SetBitmap(image_res, (const char**)_osdimage_res.pimage, _osdimage_res.width, _osdimage_res.height, 0xffff)){
				printf("SetBitmap failed, osd width:%d, height:%d\n",_osdimage_res.width, _osdimage_res.height);
			}
		}
		Toolset::Instance()->ReleaseOsdImage(_osdimage_res);

		RgnManager::InitBitmap(image, 0);
		res = Toolset::Instance()->ConvertTimeToImage(_osdimage, 36);
		if(res >= 0) {
			if(!RgnManager::SetBitmap(image, (const char**)_osdimage.pimage, _osdimage.width, _osdimage.height, 0xffff)){
				printf("SetBitmap failed.\n");
			}
		}
		sleep(1);
	}
}
void* BoardParams::Osd_WithReverse(void* arg){
	OsdThreadParams* osdparam = (OsdThreadParams*)arg;

	int VdecChn = osdparam->VdecChn;
	int res = 0;
	char text[1024] = {0};

	RGNIMAGE image_res(NULL, 1920, 1080, 0);
	image_res.Handle = RgnManager::CreateRegionToVenc(image_res, VdecChn, 0, 0, 0x7fff, 0, 0);
	if(image_res.Handle < 0){
		printf("create rgn to venc failed.\n");
	}

	while(true){
		if(osdparam->isStop){
			RgnManager::DetachRegionFromVpss(image_res.Handle, VdecChn, 0);
			RgnManager::DestroyRegions(image_res.Handle);
			RgnManager::ReleaseBitmap(image_res);
			printf("Osd %d thread exit.\n", VdecChn);
			pthread_exit(NULL);
		}
		RgnManager::InitBitmap(image_res, 0);

//		memset(text, 0, sizeof(text));
//		sprintf(text, "Chn:%d, count1:%d ,count2:%d ", VdecChn, BicycleDetection::getCount1Total(VdecChn), BicycleDetection::getCount2Total(VdecChn));
//		OSDIMAGE _osdimage(0, 72, strlen(text), 36);
//		res = Toolset::Instance()->ConvertCharToImage(text, _osdimage, _osdimage.font);
//		RgnManager::SetBitmap(VdecChn, image_res, _osdimage);

		char charText[100] = {0};
		GetNowTime(charText);
		OSDIMAGE _osdimage_res(0, 0, strlen(charText), 36);
		Toolset::Instance()->ConvertCharToImage(charText, _osdimage_res, _osdimage_res.font);
		RgnManager::SetBitmap(VdecChn, image_res, _osdimage_res);

//		Toolset::Instance()->ReleaseOsdImage(_osdimage);
		Toolset::Instance()->ReleaseOsdImage(_osdimage_res);
		sleep(1);
	}
}

int BoardParams::Hi3559a_CancelOsdThread(int VdecChn){
	int ret = -1;
	map<MyString, OsdThreadParams*>::iterator it;
	if(pthread_mutex_lock(&mutex_osd_threads) == 0){
		it = osd_threads.find(VdecChn);
		if(it != osd_threads.end()){
			if(it->second != NULL){
				it->second->isStop = true;
				printf("Hi3559a_CancelOsdThread.\n");
				pthread_join(it->second->tid, NULL);
				delete it->second;
				it = osd_threads.erase(it);
				ret = 0;
			}
		}else{
			cout <<"Hi3559a_CancelOsdThread cannot find." <<endl;
		}
		pthread_mutex_unlock(&mutex_osd_threads);
	}
	return ret;
}

int BoardParams::Hi3559a_CreateOsdThread(int VdecChn){
	map<MyString, OsdThreadParams*>::iterator it;

	if(pthread_mutex_lock(&mutex_osd_threads) == 0){
		it = osd_threads.find(VdecChn);
		if(it == osd_threads.end()){
			OsdThreadParams* osdthread = new OsdThreadParams;
			osdthread->VdecChn = (*vpssthreadargs)[VdecChn]->VpssGrp_Chn;
			osdthread->isStop = false;
			pthread_create(&osdthread->tid, NULL, Osd_WithReverse, (void*)osdthread);
			osd_threads.insert(map<MyString, OsdThreadParams*>::value_type(VdecChn, osdthread));
		}else{
			if(it->second != NULL){
				it->second->isStop = true;
				pthread_join(it->second->tid, NULL);
				delete it->second;
			}
			OsdThreadParams* osdthread = new OsdThreadParams;
			osdthread->VdecChn = (*vpssthreadargs)[VdecChn]->VpssGrp_Chn;
			osdthread->isStop = false;
			pthread_create(&osdthread->tid, NULL, Osd_WithReverse, (void*)osdthread);
			it->second = osdthread;
		}
		pthread_mutex_unlock(&mutex_osd_threads);
	}
	return 0;
}

int BoardParams::Hi3559a_InitParams(int maxchn, int maxCore, int width, int height){
    if(vencthreadargs == NULL){
    	vencthreadargs = new VencThreadParams(maxchn*2);
    }
    vencthreadargs->isStop = false;

    if(vpssthreadargs == NULL){
    	vpssthreadargs = new VpssThreadParams(maxchn);
    }
    vpssthreadargs->isStop = false;

	if(rtsp_init() < 0){
		printf("rtsp_init failed.\n");
		return -1;
	}

	if(ffmpeg == NULL)
		ffmpeg = new ffmpegCodec;

    if(hi3559a == NULL)
    	hi3559a = new Hi3559A(maxchn, width, height);

    hi3559a->SystemInit(maxchn);
    hi3559a->start();
    ffmpeg->run();

	taskscheduler = new NnieTaskScheduler<int, vector<SsdResults>>(maxCore);
	taskscheduler->SetTaskCallback(Hi3559A::ssd_detection);	//设置需要执行得方法，由调度器循环调用
	taskscheduler->SetOutPutResultHandlerCallback(BicycleDetection::run_version_muilt);
	taskscheduler->EventLoopStart();
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
	Toolset::Instance()->registCalibrationInfo(device->getDeivceCode(), "bicycle", 1920, 1080);
	int size = device->areaSize();
	for(int i=0; i<size; i++){
		vector<cv::Point> points;
		vector<AreaInfo::POINT> temp;
		AreaInfo* area = (*device)[i];
		if(area != NULL){
			Toolset::Instance()->removeArea(device->getDeivceCode(), "bicycle", area->getType());
		}else{
			LOG_ERROR("area is NULL.\n");
		}
	}
	char filename[20] = {0};
	sprintf(filename, "temp%d.jpg", device->getDeivceCode());

	if(!Toolset::Instance()->toImageFile(device->getDeivceCode(), "bicycle", filename)){
		LOG_ERROR("write image failed.\n");
	}

	BoardParams::Instance()->Hi3559a_DeleteChannel(device->getDeivceCode(), device->getDeivceCode());

	delete device;
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
	Hi3559a_AddChannel(device->getDeivceCode(), device->getDeivceCode(), (*device)["sourceurl"].GetString());

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

		if(!Toolset::Instance()->toImageFile(device->getDeivceCode(), "bicycle", filename)){
			LOG_ERROR("write image failed.\n");
		}

		Hi3559a_AddChannel(device->getDeivceCode(), device->getDeivceCode(), (*device)["sourceurl"].GetString());
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

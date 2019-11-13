/*
* BicycleDetection.cpp
*
*  Created on: 2019年7月3日
*      Author: wtxu
*/
#include "BicycleDetection.h"
//#include "BoardParams.h"

struct timeval BicycleDetection::time1;
struct timeval BicycleDetection::time2;
int BicycleDetection::count1[CHANNEL_MAX] = {0};
int BicycleDetection::count1total[CHANNEL_MAX] = {0};
pthread_mutex_t BicycleDetection::count1_mutex[CHANNEL_MAX] = PTHREAD_MUTEX_INITIALIZER;

int BicycleDetection::count2[CHANNEL_MAX] = {0};
int BicycleDetection::count2total[CHANNEL_MAX] = {0};
pthread_mutex_t BicycleDetection::count2_mutex[CHANNEL_MAX] = PTHREAD_MUTEX_INITIALIZER;

float BicycleDetection::Flow1[CHANNEL_MAX] = {0};
float BicycleDetection::Flow2[CHANNEL_MAX] = {0};
int BicycleDetection::count_temp1[CHANNEL_MAX] = {0};
int BicycleDetection::count_temp2[CHANNEL_MAX] = {0};

BicycleObjectsBuffer BicycleDetection::bob[CHANNEL_MAX];

int ClearBicycleTotalCount_CPP(int channelid){
    return BicycleDetection::clearToatal(channelid);
}

int ClearBicycleTotalCount(int channelid){
    return ClearBicycleTotalCount_CPP(channelid);
}

int BicycleDetection::clearToatal(int taskid){
	int temp = 0;
	if(pthread_mutex_lock(&count1_mutex[taskid]) == 0){
		count1total[taskid] = 0;
		pthread_mutex_unlock(&count1_mutex[taskid]);
	}
	if(pthread_mutex_lock(&count2_mutex[taskid]) == 0){
		count2total[taskid] = 0;
		pthread_mutex_unlock(&count2_mutex[taskid]);
	}
	return temp;
}

int BicycleDetection::getCount1Total(int taskid){
	int temp = 0;
	if(pthread_mutex_lock(&count1_mutex[taskid]) == 0){
		temp = count1total[taskid];
		pthread_mutex_unlock(&count1_mutex[taskid]);
	}
	return temp;
}

int BicycleDetection::getCount2Total(int taskid){
	int temp = 0;
	if(pthread_mutex_lock(&count2_mutex[taskid]) == 0){
		temp = count2total[taskid];
		pthread_mutex_unlock(&count2_mutex[taskid]);
	}
	return temp;
}

int BicycleDetection::getCount1(int taskid){
	int temp = 0;
	if(pthread_mutex_lock(&count1_mutex[taskid]) == 0){
		temp = count1[taskid];
		count1[taskid] = 0;

		pthread_mutex_unlock(&count1_mutex[taskid]);
	}
	return temp;
}

int BicycleDetection::getCount2(int taskid){
	int temp = 0;
	if(pthread_mutex_lock(&count2_mutex[taskid]) == 0){
		temp = count2[taskid];
		count2[taskid] = 0;
		pthread_mutex_unlock(&count2_mutex[taskid]);
	}
	return temp;
}

void BicycleDetection::setCount1(int taskid){
	if(pthread_mutex_lock(&count1_mutex[taskid]) == 0){
		count1[taskid] += 1;
		count1total[taskid] += 1;
		pthread_mutex_unlock(&count1_mutex[taskid]);
	}
}

void BicycleDetection::setCount2(int taskid){
	if(pthread_mutex_lock(&count2_mutex[taskid]) == 0){
		count2[taskid] += 1;
		count2total[taskid] += 1;
		pthread_mutex_unlock(&count2_mutex[taskid]);
	}
}

void BicycleDetection::run_version_muilt(std::shared_ptr<SsdResults>& results, int VdecChn){
    MyString deviceid(VdecChn);
	bool is_buffer_empty = bob[VdecChn].empty();

	static int imageid = 0;
	ObjectBicycle test_obj;
	test_obj.insertKeyValue(test_obj.xmin, 300);
	test_obj.insertKeyValue(test_obj.ymin, 300);
	test_obj.insertKeyValue(test_obj.xmax, 400);
	test_obj.insertKeyValue(test_obj.ymax, 400);
	//保存位于region中的检测信息于list（依据框的右下角坐标）
    int result_size = results.get()->results.size();

	for(int i=0; i<result_size; i++){
        std::shared_ptr<BicycleDetectionResult> temp = results.get()->results[i];
		ObjectBicycle test_obj;
        test_obj.insertKeyValue(test_obj.xmin, temp.get()->xmin);
        test_obj.insertKeyValue(test_obj.ymin, temp.get()->ymin);
        test_obj.insertKeyValue(test_obj.xmax, temp.get()->xmax);
        test_obj.insertKeyValue(test_obj.ymax, temp.get()->ymax);
        test_obj.insertKeyValue(test_obj.type, temp.get()->type);
        test_obj.insertKeyValue(test_obj.confidence, temp.get()->confidence);

		int areaid = 255;
        areaid = Toolset::Instance()->AreaValue(deviceid, "bicycle", temp.get()->xmax, temp.get()->ymax);
		if(areaid != 1){ //收集进入区域的目标
//            printf("Target not entering area, device:%d, xmax:%d, ymax:%d, areaid:%d\n",deviceid.toInt(), temp.get()->xmax, temp.get()->ymax, areaid);
			continue;
		}
		if(is_buffer_empty){	//如果缓冲区为空，直接将目标添加进缓存区中
			bob[VdecChn].insert(test_obj);
		}else{
			if(bob[VdecChn].Match(test_obj, 0.3) < 0){	//匹配目标
				bob[VdecChn].insert(test_obj);
			}
		}
	}
	int size = bob[VdecChn].check(6);
	if(size < 0){
		printf("check buffer objects failed.\n");
	}

	ObjectBicycle* pobj = bob[VdecChn].begin();
	if(pobj == NULL) {
		return;
	}
	while(pobj != NULL){
		ObjectBicycle &temp = *pobj;
		int drop = temp[temp.drop];

		if(drop == 0){
			int counted_status = temp[temp.isCounted];
			int count_times = temp[temp.trackCount];
			if(!counted_status && count_times > 1){
				temp.insertKeyValue(temp.isCounted, 1);
				int type = temp[temp.type];
				switch(type){
				case 1:
					setCount1(VdecChn);
					count_temp1[VdecChn] += 1;
					break;
				case 2:
					setCount2(VdecChn);
					count_temp2[VdecChn] += 1;
					break;
				default:
					break;
				}
			}
		}
		pobj = bob[VdecChn].Next();
	}
//	printf("Chnanel:%d, count1:%d, count2:%d\n", VdecChn, count_temp1[VdecChn], count_temp2[VdecChn]);

}

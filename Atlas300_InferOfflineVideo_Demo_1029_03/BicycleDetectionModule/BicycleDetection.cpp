/*
* BicycleDetection.cpp
*
*  Created on: 2019年7月3日
*      Author: wtxu
*/
#include "BicycleDetection.h"

struct timeval BicycleDetection::time1;
struct timeval BicycleDetection::time2;
int BicycleDetection::count1[10] = {0};
int BicycleDetection::count1total[10] = {0};
pthread_mutex_t BicycleDetection::count1_mutex[10] = PTHREAD_MUTEX_INITIALIZER;

int BicycleDetection::count2[10] = {0};
int BicycleDetection::count2total[10] = {0};
pthread_mutex_t BicycleDetection::count2_mutex[10] = PTHREAD_MUTEX_INITIALIZER;

float BicycleDetection::Flow1[10] = {0};
float BicycleDetection::Flow2[10] = {0};
int BicycleDetection::count_temp1[10] = {0};
int BicycleDetection::count_temp2[10] = {0};

BicycleObjectsBuffer BicycleDetection::bob[10];

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

void BicycleDetection::run_version2(ReturnResults& returnResults, Core& core, int taskid){
	CalibrationMethod* cm = core.cm;

	if(cm == NULL) {
		printf("CalibrationMethod is NULL\n");
		return;
	}

	bool is_buffer_empty = bob[taskid].empty();

    vector<SsdResults>& results = returnResults.vector_SsdRes;
    vector<ObjectBicycle*>& trackResults = returnResults.trackResults;

	ObjectBicycle test_obj;
	test_obj.insertKeyValue(test_obj.xmin, 300);
	test_obj.insertKeyValue(test_obj.ymin, 300);
	test_obj.insertKeyValue(test_obj.xmax, 400);
	test_obj.insertKeyValue(test_obj.ymax, 400);
	//保存位于region中的检测信息于list（依据框的右下角坐标）
	for(int i=0; i<results.size(); i++){
		SsdResults& temp = results[i];
		ObjectBicycle test_obj;
		test_obj.insertKeyValue(test_obj.xmin, temp.xmin);
		test_obj.insertKeyValue(test_obj.ymin, temp.ymin);
		test_obj.insertKeyValue(test_obj.xmax, temp.xmax);
		test_obj.insertKeyValue(test_obj.ymax, temp.ymax);
		test_obj.insertKeyValue(test_obj.type, temp.type);
		test_obj.insertKeyValue(test_obj.confidence, temp.confidence);

		int areaid = 255;
		if(!cm->AreaId(temp.xmax, temp.ymax, areaid)){ //收集进入区域的目标
			continue;
		}
		if(is_buffer_empty){	//如果缓冲区为空，直接将目标添加进缓存区中
			bob[taskid].insert(test_obj);
		}else{
			if(bob[taskid].Match(test_obj, 0.3) < 0){	//匹配目标
				bob[taskid].insert(test_obj);
			}
		}
	}
	int size = bob[taskid].check(6);
	cout <<"current objects size:" <<size <<endl;

	ObjectBicycle* pobj = bob[taskid].begin();
	while(pobj != NULL){
		trackResults.push_back(pobj);
		pobj = bob[taskid].Next();
	}
}


void BicycleDetection::run_version2(vector<SsdResults>& results, Core& core, int taskid, vector<ObjectBicycle*>& trackResults){
	CalibrationMethod* cm = core.cm;

	if(cm == NULL) {
		printf("CalibrationMethod is NULL\n");
		return;
	}

	bool is_buffer_empty = bob[taskid].empty();

	ObjectBicycle test_obj;
	test_obj.insertKeyValue(test_obj.xmin, 300);
	test_obj.insertKeyValue(test_obj.ymin, 300);
	test_obj.insertKeyValue(test_obj.xmax, 400);
	test_obj.insertKeyValue(test_obj.ymax, 400);
	//保存位于region中的检测信息于list（依据框的右下角坐标）
	for(int i=0; i<results.size(); i++){
		SsdResults& temp = results[i];
		ObjectBicycle test_obj;
		test_obj.insertKeyValue(test_obj.xmin, temp.xmin);
		test_obj.insertKeyValue(test_obj.ymin, temp.ymin);
		test_obj.insertKeyValue(test_obj.xmax, temp.xmax);
		test_obj.insertKeyValue(test_obj.ymax, temp.ymax);
		test_obj.insertKeyValue(test_obj.type, temp.type);
		test_obj.insertKeyValue(test_obj.confidence, temp.confidence);

		int areaid = 255;
		if(!cm->AreaId(temp.xmax, temp.ymax, areaid)){ //收集进入区域的目标
			continue;
		}
		if(is_buffer_empty){	//如果缓冲区为空，直接将目标添加进缓存区中
			bob[taskid].insert(test_obj);
		}else{
			if(bob[taskid].Match(test_obj, 0.3) < 0){	//匹配目标
				bob[taskid].insert(test_obj);
			}
		}
	}
	int size = bob[taskid].check(6);
	cout <<"current objects size:" <<size <<endl;

	ObjectBicycle* pobj = bob[taskid].begin();
	while(pobj != NULL){
		trackResults.push_back(pobj);
		pobj = bob[taskid].Next();
	}
}

void BicycleDetection::run_version2(vector<SsdResults>& results, Core& core, int taskid){
	CalibrationMethod* cm = core.cm;

	if(cm == NULL) {
		printf("CalibrationMethod is NULL\n");
		return;
	}

	bool is_buffer_empty = bob[taskid].empty();

	ObjectBicycle test_obj;
	test_obj.insertKeyValue(test_obj.xmin, 300);
	test_obj.insertKeyValue(test_obj.ymin, 300);
	test_obj.insertKeyValue(test_obj.xmax, 400);
	test_obj.insertKeyValue(test_obj.ymax, 400);
	//保存位于region中的检测信息于list（依据框的右下角坐标）
	for(int i=0; i<results.size(); i++){
		SsdResults& temp = results[i];
		ObjectBicycle test_obj;
		test_obj.insertKeyValue(test_obj.xmin, temp.xmin);
		test_obj.insertKeyValue(test_obj.ymin, temp.ymin);
		test_obj.insertKeyValue(test_obj.xmax, temp.xmax);
		test_obj.insertKeyValue(test_obj.ymax, temp.ymax);
		test_obj.insertKeyValue(test_obj.type, temp.type);
		test_obj.insertKeyValue(test_obj.confidence, temp.confidence);

		int areaid = 255;
		if(!cm->AreaId(temp.xmax, temp.ymax, areaid)){ //收集进入区域的目标
			continue;
		}
		if(is_buffer_empty){	//如果缓冲区为空，直接将目标添加进缓存区中
			bob[taskid].insert(test_obj);
		}else{
			if(bob[taskid].Match(test_obj, 0.3) < 0){	//匹配目标
				bob[taskid].insert(test_obj);
			}
		}
	}
	int size = bob[taskid].check(6);

	ObjectBicycle* pobj = bob[taskid].begin();
	if(pobj == NULL) {
		return;
	}
	while(pobj != NULL){
		ObjectBicycle &temp = *pobj;
		int drop = temp[temp.drop];

		if(drop == 0){
			cout << temp.toJson() <<endl;
			int counted_status = temp[temp.isCounted];
			int count_times = temp[temp.trackCount];
			if(!counted_status && count_times > 1){
				temp.insertKeyValue(temp.isCounted, 1);
				int type = temp[temp.type];
				switch(type){
				case 1:
					setCount1(taskid);
					count_temp1[taskid] += 1;
					break;
				case 2:
					setCount2(taskid);
					count_temp2[taskid] += 1;
					break;
				default:
					break;
				}
			}
		}
		pobj = bob[taskid].Next();
	}
	printf("count1:%d, count2:%d\n", count_temp1[taskid], count_temp2[taskid]);

}
void BicycleDetection::run_version_muilt(vector<SsdResults>& results, int VdecChn, int taskid){
	MyString deviceid = BoardParams::Instance()->getDeviceId(VdecChn);
	bool is_buffer_empty = bob[VdecChn].empty();

	static int imageid = 0;
	ObjectBicycle test_obj;
	test_obj.insertKeyValue(test_obj.xmin, 300);
	test_obj.insertKeyValue(test_obj.ymin, 300);
	test_obj.insertKeyValue(test_obj.xmax, 400);
	test_obj.insertKeyValue(test_obj.ymax, 400);
	//保存位于region中的检测信息于list（依据框的右下角坐标）
	int result_size = results.size();

	for(int i=0; i<result_size; i++){
		SsdResults& temp = results[i];
		ObjectBicycle test_obj;
		test_obj.insertKeyValue(test_obj.xmin, temp.xmin);
		test_obj.insertKeyValue(test_obj.ymin, temp.ymin);
		test_obj.insertKeyValue(test_obj.xmax, temp.xmax);
		test_obj.insertKeyValue(test_obj.ymax, temp.ymax);
		test_obj.insertKeyValue(test_obj.type, temp.type);
		test_obj.insertKeyValue(test_obj.confidence, temp.confidence);

		int areaid = 255;
		areaid = Toolset::Instance()->AreaValue(deviceid, "bicycle", temp.xmax, temp.ymax);
		if(areaid != 1){ //收集进入区域的目标
//			printf("Target not entering area, device:%d, xmax:%d, ymax:%d, areaid:%d\n",deviceid.toInt(), temp.xmax, temp.ymax, areaid);
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
//	cv::Mat videoimage(1080, 1920, CV_8UC3, cv::Scalar(255, 255, 255));
	while(pobj != NULL){
		ObjectBicycle &temp = *pobj;
		int drop = temp[temp.drop];

		if(drop == 0){
//			cout << temp.toJson() <<endl;
//			stringstream str;
//			str <<"id:"<< temp[temp.id].GetInt();
//			int xmin = temp[temp.xmin];
//			int ymin = temp[temp.ymin];
//			int xmax = temp[temp.xmax];
//			int ymax = temp[temp.ymax];
//			putText(videoimage, str.str(), cv::Point(xmin, ymin), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0,0,255), 2);
//			cv::rectangle(videoimage, cv::Point(xmin, ymin), cv::Point(xmax, ymax), cv::Scalar(0, 0, 255), 2);

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
//	umask(0);
//	if (access("./out", 0) == -1) {
//		umask(0);
//		mkdir("./out", 0777);
//	}
//	char imagepath[100] = { 0 };
//	sprintf(imagepath, "./out/image%d.jpg", imageid++);
//	imwrite(string(imagepath), videoimage);
	printf("Chnanel:%d, count1:%d, count2:%d\n", VdecChn, count_temp1[VdecChn], count_temp2[VdecChn]);

}
void BicycleDetection::run(vector<SsdResults>& results, Core& core, int taskid){
	CalibrationMethod* cm = core.cm;

	if(cm == NULL) {
		printf("CalibrationMethod is NULL\n");
		return;
	}
	gettimeofday(&time1, NULL);
	vector<SsdResults> list;

	//保存位于region中的检测信息于list（依据框的右下角坐标）
	for(int i=0; i<results.size(); i++){
		SsdResults& temp = results[i];
//			printf("venc_chn:%d, type:%d xmin:%d,ymin:%d,xmax:%d,ymax:%d,confidence:%f\n",
//					core.video_chn, temp.type, temp.xmin, temp.ymin, temp.xmax, temp.ymax, temp.confidence);
		int areaid = 255;
		if(cm->AreaId(temp.xmax, temp.ymax, areaid)){
			list.push_back(temp);
		}
	}

	if(list.empty()) {
		return;
	}else{
		for(int i=0; i<list.size(); i++){
			SsdResults result = list[i];
			switch(result.type){
			case 1:
				setCount1(taskid);
				count_temp1[taskid] += 1;
				break;
			case 2:
				setCount2(taskid);
				count_temp2[taskid] += 1;
				break;
			default:
				break;
			}
		}
	}
	printf("count1:%d, count2:%d\n", count_temp1[taskid], count_temp2[taskid]);
}

/*
 * BicycleDetection.h
 *
 *  Created on: 2019年6月24日
 *      Author: wtxu
 */

#ifndef BICYCLEDETECTION_H_
#define BICYCLEDETECTION_H_
#include <mutex>
#include <queue>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>
#include "CalibrationMethod.h"
#include "BicycleObjectsBuffer.h"
#include "Hi3559A.h"
using namespace std;

class BicycleDetection {
public:
	BicycleDetection(){}
	virtual ~BicycleDetection(){}

	static int getCount1(int taskid);
	static int getCount2(int taskid);
	static int clearToatal(int taskid);
	static int getCount1Total(int taskid);
	static int getCount2Total(int taskid);
	static void setCount1(int taskid);
	static void setCount2(int taskid);
	static void run(vector<SsdResults>& results, Core& core, int taskid);
	static void run_version2(vector<SsdResults>& results, Core& core, int taskid);
	static void run_version2(ReturnResults& returnResults, Core& core, int taskid);
	static void run_version2(vector<SsdResults>& results, Core& core, int taskid, vector<ObjectBicycle*>& trackResults);
	static void run_version_muilt(vector<SsdResults>& results, int VdecChn, int taskid);

	static int count_temp1[10];
	static int count_temp2[10];

	static struct timeval time1;
	static struct timeval time2;
	static int count1[10];
	static int count1total[10];
	static pthread_mutex_t count1_mutex[10];

	static int count2[10];
	static int count2total[10];
	static pthread_mutex_t count2_mutex[10];

	static float Flow1[10];
	static float Flow2[10];
	static BicycleObjectsBuffer bob[10];

};

#endif /* BICYCLEDETECTION_H_ */

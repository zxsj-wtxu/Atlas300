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
#include "bicyclecommon.h"
using namespace std;

#define CHANNEL_MAX 64

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
    static void run_version_muilt(std::shared_ptr<SsdResults> &results, int VdecChn);

    static int count_temp1[CHANNEL_MAX];
    static int count_temp2[CHANNEL_MAX];

	static struct timeval time1;
	static struct timeval time2;
    static int count1[CHANNEL_MAX];
    static int count1total[CHANNEL_MAX];
    static pthread_mutex_t count1_mutex[CHANNEL_MAX];

    static int count2[CHANNEL_MAX];
    static int count2total[CHANNEL_MAX];
    static pthread_mutex_t count2_mutex[CHANNEL_MAX];

    static float Flow1[CHANNEL_MAX];
    static float Flow2[CHANNEL_MAX];
    static BicycleObjectsBuffer bob[CHANNEL_MAX];

};

#endif /* BICYCLEDETECTION_H_ */

/*
 * CalibrationMethod.h
 *
 *  Created on: 2018年9月3日
 *      Author: wtxu
 */

#ifndef CALIBRATIONMETHOD_H_
#define CALIBRATIONMETHOD_H_

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "LaneAreaParameter.h"
#include <map>

using namespace std;

class CalibrationMethod {
public:
	CalibrationMethod();
	CalibrationMethod(int width, int height, bool isSaveImage);
	virtual ~CalibrationMethod();

	bool AreaId(int x, int y, int& areaid);
	void setSavePath(string path);
	static void callback(AbstractParameter* ap, CalibrationMethod* cm);
private:
	string savepath;
	bool isSaveImage = false;
	int default_image_width = 0;
	int default_image_height = 0;
	cv::Mat* image_vehicleflow = NULL;
	pthread_mutex_t mutex_image_vehicleflow = PTHREAD_MUTEX_INITIALIZER;

	int setMutexImageVehicleFlowLock();
	int setMutexImageVehicleFlowUnlock();
	void createArea(LaneAreaParameter& lap, cv::Mat** image_vehicleflow, int width, int height);
    void createBasedataArea(LaneAreaParameter& lap, cv::Mat* image_vehicleflow, int width, int height);
};

#endif /* CALIBRATIONMETHOD_H_ */

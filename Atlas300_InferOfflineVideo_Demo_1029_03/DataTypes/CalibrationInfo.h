/*
 * CalibrationInfo.h
 *
 *  Created on: 2019年7月13日
 *      Author: wtxu
 */

#ifndef CALIBRATIONINFO_H_
#define CALIBRATIONINFO_H_
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "MyString.h"
using namespace std;
using namespace cv;

class CalibrationInfo {
public:
	class ImageContainer{
	public:
		ImageContainer();
		~ImageContainer();
		ImageContainer(cv::Mat& image);
		ImageContainer(int width, int height);
		ImageContainer(const ImageContainer& image);
		ImageContainer(const ImageContainer* image);
		ImageContainer& operator=(const ImageContainer* image);
		ImageContainer& operator=(const ImageContainer& image);
		ImageContainer& operator=(const cv::Mat& image);
		ImageContainer& operator=(const cv::Mat*& image);

		int clear();
		int removeArea(int value);
		int AreaValue(int x, int y);
		int addArea(int value, const vector<cv::Point>& points);	//增加区域
		int updateArea(int value, const vector<cv::Point>& points);	//增加区域
		bool toFile(const MyString& filepath);
	private:
		cv::Mat *image = NULL;
		int values[1024] = {0};
		map<int, const vector<cv::Point>> value_points;
		pthread_mutex_t mutex_image = PTHREAD_MUTEX_INITIALIZER;
		void drawPolygon(int value, const vector<cv::Point>& points);
	};
public:
	CalibrationInfo();
	~CalibrationInfo();
	CalibrationInfo(const MyString& id, const MyString& type, int width, int height);
	CalibrationInfo(const CalibrationInfo& imageinfo);
	CalibrationInfo& operator=(const CalibrationInfo& imageinfo);

	int size();
	const MyString& getDeviceId();
	int removeTypeArea(const MyString& type);
	int addTypeArea(const MyString& type, int width, int height);
	bool toFile(const MyString& type, const MyString& filepath);

	int clear(const MyString& type);
	int removeArea(const MyString& type, int value);
	int AreaValue(const MyString& type, int x, int y);
	int addArea(const MyString& type, int value, const vector<cv::Point>& points);	//增加区域
	int updateArea(const MyString& type, int value, const vector<cv::Point>& points);	//增加区域

private:
	const MyString& deviceid; //标识属于哪台设备
	map<const MyString, ImageContainer*> images;
	pthread_mutex_t mutex_images = PTHREAD_MUTEX_INITIALIZER;
};

#endif /* CALIBRATIONINFO_H_ */

/*
 * CalibrationMethod.cpp
 *
 *  Created on: 2018年12月17日
 *      Author: wtxu
 */

#include "CalibrationMethod.h"

using namespace cv;

CalibrationMethod::CalibrationMethod() {
	// TODO Auto-generated constructor stub
	default_image_width = 1920;
	default_image_height= 1080;
	if(image_vehicleflow == NULL){
		image_vehicleflow = new cv::Mat(default_image_height, default_image_width, CV_8UC1, cv::Scalar(255));
	}
}

CalibrationMethod::CalibrationMethod(int width, int height, bool isSaveImage) {
	// TODO Auto-generated constructor stub
	this->isSaveImage = isSaveImage;
	default_image_width = width;
	default_image_height= height;
	if(image_vehicleflow == NULL){
		image_vehicleflow = new cv::Mat(default_image_height, default_image_width, CV_8UC1, cv::Scalar(255));
	}
}

int CalibrationMethod::setMutexImageVehicleFlowLock(){
	return pthread_mutex_lock(&mutex_image_vehicleflow);
}

int CalibrationMethod::setMutexImageVehicleFlowUnlock(){
	return pthread_mutex_unlock(&mutex_image_vehicleflow);
}

void CalibrationMethod::setSavePath(string path){
	savepath = path;
}

CalibrationMethod::~CalibrationMethod() {
	// TODO Auto-generated destructor stub
	if(!image_vehicleflow) delete image_vehicleflow;
}

void CalibrationMethod::callback(AbstractParameter* ap, CalibrationMethod* cm){
	LaneAreaParameter* lap = new LaneAreaParameter;
	if(strncmp(ap->getTableName().c_str(), lap->getLabel().c_str(), strlen(lap->getLabel().c_str())) == 0){
		*lap = *(LaneAreaParameter*)ap;
        //为每个区域创建一张图片，用来判断目标是否位于区域内
		if(cm != NULL){
			if(cm->setMutexImageVehicleFlowLock() == 0){
		        cm->createArea(*lap, &cm->image_vehicleflow, cm->default_image_width, cm->default_image_height);
		        cm->setMutexImageVehicleFlowUnlock();
			}
		}else{
			printf("CalibrationMethod is NULL.\n");
		}
	}
	delete lap;
	lap = NULL;
	return;
}

/**
 * 功能：创建区域图片
 **/
void CalibrationMethod::createBasedataArea(LaneAreaParameter& lap, cv::Mat* image_vehicleflow, int width, int height){

    if(image_vehicleflow == NULL) {
        image_vehicleflow = new cv::Mat(height, width, CV_8UC1, cv::Scalar(255));
    }else {
        delete image_vehicleflow;

        image_vehicleflow = new cv::Mat(height, width, CV_8UC1, cv::Scalar(255));
    }

    printf("size:%d\n",lap.size());
    for(int i=0; i<lap.size(); i++){
        LaneAreaParameter::Basedata bd = lap[i];

        cv::Point points[1][10];
        int npt[1] = {4};

        for(int k=0; k<bd.n; k++){
        	printf("x%d:%d, y%d:%d\n",k,bd[k].x,k,bd[k].y);
            points[0][k] = cv::Point(  bd[k].x, bd[k].y );
        }
        const cv::Point* pt[1] = { points[0] };
        npt[0] = bd.n;

        cv::fillPoly( *image_vehicleflow, pt, npt, 1, cv::Scalar(bd.id));
    }
    if(!savepath.empty() && isSaveImage){
    	string filepath = savepath;
        cv::imwrite(filepath.c_str(), *image_vehicleflow);
    }
}
/**
 * 功能：创建区域图片
 **/
void CalibrationMethod::createArea(LaneAreaParameter& lap, cv::Mat** image_vehicleflow, int width, int height){

    if((*image_vehicleflow) == NULL) {
        *image_vehicleflow = new cv::Mat(height, width, CV_8UC1, cv::Scalar(255));
    }else {
        delete (*image_vehicleflow);

        *image_vehicleflow = new cv::Mat(height, width, CV_8UC1, cv::Scalar(255));
    }

    printf("size:%d\n",lap.size());
    for(int i=0; i<lap.size(); i++){
        LaneAreaParameter::Basedata bd = lap[i];

        cv::Point points[1][10];
        int npt[1] = {4};

        for(int k=0; k<bd.n; k++){
        	printf("x%d:%d, y%d:%d\n",k,bd[k].x,k,bd[k].y);
            points[0][k] = cv::Point(  bd[k].x, bd[k].y );
        }
        const cv::Point* pt[1] = { points[0] };
        npt[0] = bd.n;

        cv::fillPoly( **image_vehicleflow, pt, npt, 1, cv::Scalar(bd.id));
    }
    if(!savepath.empty() && isSaveImage){
    	string filepath = savepath;
        cv::imwrite(filepath.c_str(), **image_vehicleflow);
    }
}
bool CalibrationMethod::AreaId(int x, int y, int& areaid){
	bool res = false;
	if(setMutexImageVehicleFlowLock() == 0){
		if(image_vehicleflow == NULL) {
			setMutexImageVehicleFlowUnlock();
			return false;
		}

		if(y >=  image_vehicleflow->rows || x >= image_vehicleflow->cols) {
			setMutexImageVehicleFlowUnlock();
			return false;
		}
	    areaid = (int)image_vehicleflow->at<uchar>(y,x);
		if(areaid == 255){
			res = false;
		}else{
			res = true;
		}
		setMutexImageVehicleFlowUnlock();
	}

	return res;
}

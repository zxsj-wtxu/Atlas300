/*
 * CalibrationInfo.cpp
 *
 *  Created on: 2019年7月13日
 *      Author: wtxu
 */

#include "CalibrationInfo.h"

CalibrationInfo::~CalibrationInfo() {
	// TODO Auto-generated destructor stub
	map<const MyString, ImageContainer*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		for(it=this->images.begin(); it != this->images.end(); ){
			if(it->second != NULL){
				delete it->second;
				it->second = NULL;
			}
			it = this->images.erase(it);
		}
		pthread_mutex_unlock(&mutex_images);
	}
	cout <<"~CalibrationInfo"<<endl;
}
/*
 * MyString deviceid:设备的编码
 * MyString type:区域的应用类型编码
 * int width：当前类型区域的宽度
 * int height：当前类型区域的高度
 * */
CalibrationInfo::CalibrationInfo(const MyString& id, const MyString& type, int width, int height):deviceid(id){
	cout << "create CalibrationInfo" <<endl;
	this->images.insert(map<const MyString, ImageContainer*>::value_type(type, new ImageContainer(width, height)));
}

CalibrationInfo::CalibrationInfo(const CalibrationInfo& imageinfo):deviceid(imageinfo.deviceid){
	map<const MyString, ImageContainer*>::const_iterator it;
	for(it = imageinfo.images.begin(); it != imageinfo.images.end(); it++){
		this->images.insert(map<const MyString, ImageContainer*>::value_type(it->first, new ImageContainer(it->second)));
	}
}

CalibrationInfo& CalibrationInfo::operator=(const CalibrationInfo& imageinfo){
	if(pthread_mutex_lock(&mutex_images) == 0){
		map<const MyString, ImageContainer*>::const_iterator it;

		for(it=this->images.begin(); it != this->images.end(); ){
			if(it->second != NULL)
				delete it->second;
			it = this->images.erase(it);
		}

		for(it=imageinfo.images.begin(); it != imageinfo.images.end(); ){
			this->images.insert(map<const MyString, ImageContainer*>::value_type(it->first, new ImageContainer(it->second)));
		}
		pthread_mutex_unlock(&mutex_images);
	}
}

int CalibrationInfo::size(){
	return this->images.size();
}

const MyString& CalibrationInfo::getDeviceId(){
	return this->deviceid;
}

int CalibrationInfo::removeTypeArea(const MyString& type){
	int ret = 0;
	map<const MyString, ImageContainer*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = this->images.find(type);
		if(it == this->images.end()){
			ret = -1;
		}else{
			if(it->second != NULL)
				delete it->second;
			it = this->images.erase(it);
			ret = 0;
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return 0;
}

bool CalibrationInfo::toFile(const MyString& type, const MyString& filepath){
	bool res = false;
	map<const MyString, ImageContainer*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = this->images.find(type);
		if(it != this->images.end()){
			if(it->second != NULL){
				res = it->second->toFile(filepath);
			}else{
				printf("cannot find image.\n");
				res = false;
			}
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return res;
}

int CalibrationInfo::addTypeArea(const MyString& type, int width, int height){
	map<const MyString, ImageContainer*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = this->images.find(type);
		if(it == this->images.end()){
			this->images.insert(map<const MyString, ImageContainer*>::value_type(type, new ImageContainer(width, height)));
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return 0;
}

int CalibrationInfo::clear(const MyString& type){
	int ret = 0;
	map<const MyString, ImageContainer*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = this->images.find(type);
		if(it != this->images.end()){
			it->second->clear();
			ret = 0;
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int CalibrationInfo::removeArea(const MyString& type, int value){
	int ret = 0;
	map<const MyString, ImageContainer*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = this->images.find(type);
		if(it != this->images.end()){
			it->second->removeArea(value);
			ret = 0;
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int CalibrationInfo::AreaValue(const MyString& type, int x, int y){
	int ret = 0;
	map<const MyString, ImageContainer*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = this->images.find(type);
		if(it != this->images.end()){
			ret = it->second->AreaValue(x,y);
		}else{
//			printf("cannot find %s area.\n", type.c_str());
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int CalibrationInfo::addArea(const MyString& type, int value, const vector<cv::Point>& points){
	int ret = 0;
	map<const MyString, ImageContainer*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = this->images.find(type);
		if(it != this->images.end()){
			ret = it->second->addArea(value, points);
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

int CalibrationInfo::updateArea(const MyString& type, int value, const vector<cv::Point>& points){
	int ret = 0;
	map<const MyString, ImageContainer*>::iterator it;
	if(pthread_mutex_lock(&mutex_images) == 0){
		it = this->images.find(type);
		if(it != this->images.end()){
			ret = it->second->updateArea(value, points);
		}else{
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_images);
	}
	return ret;
}

CalibrationInfo::ImageContainer::ImageContainer(cv::Mat& image){
	if(image.channels() != 1) return;
	this->operator =(image);
	cout <<"ImageContainer(cv::Mat& image)" <<endl;
}

CalibrationInfo::ImageContainer::ImageContainer(const ImageContainer* image){
	if(image->image == NULL || image->image->channels() != 1) return;
	this->operator =(image);
	cout <<"ImageContainer(const ImageContainer* image)" <<endl;
}

CalibrationInfo::ImageContainer::ImageContainer(const ImageContainer& image){
	if(image.image == NULL || image.image->channels() != 1) return;
	this->operator =(image);
	cout <<"ImageContainer(const ImageContainer& image)" <<endl;
}

void CalibrationInfo::ImageContainer::drawPolygon(int value, const vector<cv::Point>& points){
    cv::Point _points[1][10];
    int npt[1] = {4};

    int n = points.size();
    for(int k = 0; k < n; k++){
    	const cv::Point& point = points[k];
    	_points[0][k] = cv::Point( point.x, point.y );
    }
    const cv::Point* pt[1] = { _points[0] };
    npt[0] = n;

    cv::fillPoly( *this->image, pt, npt, 1, cv::Scalar(value) );
    return;
}

int CalibrationInfo::ImageContainer::addArea(int value, const vector<cv::Point>& points){
	if(pthread_mutex_lock(&mutex_image) == 0){
		if(values[value] != 0 || image == NULL || image->empty()) {
			pthread_mutex_unlock(&mutex_image);
			return -1;
		}
		if(points.empty()) return -1;

	    values[value] = 1;
		drawPolygon(value, points);
		value_points.insert(map<int, const vector<cv::Point>>::value_type(value, points));

		pthread_mutex_unlock(&mutex_image);
	}

    return 0;
}

int CalibrationInfo::ImageContainer::removeArea(int value){
	map<int, const vector<cv::Point>>::iterator it;
	if(pthread_mutex_lock(&mutex_image) == 0){
		if(image == NULL || image->empty()) {
			pthread_mutex_unlock(&mutex_image);
			return -1;
		}

		it = value_points.find(value);
		if(it != value_points.end()){
			//重置区域值
			drawPolygon(255, it->second);
            it = value_points.erase(it);
		}
		pthread_mutex_unlock(&mutex_image);
	}

    return 0;
}

bool CalibrationInfo::ImageContainer::toFile(const MyString& filepath){
	if(this->image == NULL) {
		printf("image is NULL.\n");
		return false;
	}
	return imwrite(string(filepath.c_str()), *this->image);
}

int CalibrationInfo::ImageContainer::updateArea(int value, const vector<cv::Point>& points){
	map<int, const vector<cv::Point>>::iterator it;
	if(pthread_mutex_lock(&mutex_image) == 0){
		if(image == NULL || image->empty()) {
			pthread_mutex_unlock(&mutex_image);
			return -1;
		}

		it = value_points.find(value);
		if(it == value_points.end()){
			drawPolygon(value, points);
		}else{
			//重置区域值
			drawPolygon(255, it->second);
			drawPolygon(value, points);
		}
		pthread_mutex_unlock(&mutex_image);
	}

    return 0;
}

int CalibrationInfo::ImageContainer::AreaValue(int x, int y){
	int res = -1;
	if(image == NULL || image->empty() || y >=  image->rows || x >= image->cols) {
//		printf("image is NULL.\n");
		return res;
	}
	if(pthread_mutex_lock(&mutex_image) == 0){
		res = (int)image->at<uchar>(y,x);
		pthread_mutex_unlock(&mutex_image);
	}

	return res;
}

CalibrationInfo::ImageContainer& CalibrationInfo::ImageContainer::operator=(const cv::Mat*& image){
	if((*image).channels() != 1) return *this;
	if(pthread_mutex_lock(&mutex_image) == 0){
		if(this->image == NULL) {
			this->image = new Mat(*image);
		}else{
			delete this->image;
			this->image = new Mat(*image);
		}
		pthread_mutex_unlock(&mutex_image);
	}
	cout <<"ImageContainer::operator=(const cv::Mat*& image)" <<endl;

	return *this;
}

CalibrationInfo::ImageContainer& CalibrationInfo::ImageContainer::operator=(const cv::Mat& image){
	if(image.channels() != 1) return *this;
	if(pthread_mutex_lock(&mutex_image) == 0){
		if(this->image == NULL) {
			this->image = new Mat(image);
		}else{
			delete this->image;
			this->image = new Mat(image);
		}
		pthread_mutex_unlock(&mutex_image);
	}
	cout <<"ImageContainer::operator=(const cv::Mat& image)" <<endl;

	return *this;
}

CalibrationInfo::ImageContainer& CalibrationInfo::ImageContainer::operator=(const ImageContainer* image){
	if(pthread_mutex_lock(&mutex_image) == 0){
		if(image->image == NULL || image->image->empty()) return *this;
		if(this->image == NULL){
			this->image = new Mat(*image->image);
		}else{
			delete this->image;
			this->image = new Mat(*image->image);
		}
		pthread_mutex_unlock(&mutex_image);
	}
	cout <<"ImageContainer::operator=(const ImageContainer* image)" <<endl;

	return *this;
}

CalibrationInfo::ImageContainer& CalibrationInfo::ImageContainer::operator=(const ImageContainer& image){
	if(pthread_mutex_lock(&mutex_image) == 0){
		if(image.image == NULL || image.image->empty()) return *this;
		if(this->image == NULL){
			this->image = new Mat(*image.image);
		}else{
			delete this->image;
			this->image = new Mat(*image.image);
		}
		pthread_mutex_unlock(&mutex_image);
	}
	cout <<"ImageContainer::operator=(const ImageContainer& image)" <<endl;

	return *this;
}

int CalibrationInfo::ImageContainer::clear(){
	if(image == NULL || image->empty()) return -1;
	memset(image->data, 255, image->cols * image->rows);
	return 0;
}

CalibrationInfo::ImageContainer::ImageContainer(int width, int height){
	cout <<"ImageContainer(int width, int height)" <<endl;
	if(image == NULL){
		image = new Mat(height, width, CV_8UC1, cv::Scalar(255));
	}
}

CalibrationInfo::ImageContainer::ImageContainer(){
	cout <<"ImageContainer()" <<endl;

}

CalibrationInfo::ImageContainer::~ImageContainer(){
	cout << "~ImageContainer" <<endl;
	if(image != NULL){
		image->release();
		delete image;
		image = NULL;
	}
}

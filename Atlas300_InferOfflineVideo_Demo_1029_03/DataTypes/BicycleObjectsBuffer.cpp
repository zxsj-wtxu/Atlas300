/*
 * BicycleObjectsBuffer.cpp
 *
 *  Created on: 2019年7月13日
 *      Author: wtxu
 */

#include "BicycleObjectsBuffer.h"

BicycleObjectsBuffer::BicycleObjectsBuffer() {
	// TODO Auto-generated constructor stub

}

BicycleObjectsBuffer::~BicycleObjectsBuffer() {
	// TODO Auto-generated destructor stub
	map<int, ObjectBicycle*>::iterator it;
	if(pthread_mutex_lock(&mutex_obj_state_buffer) == 0){
		for(it = obj_state_buffer.begin(); it != obj_state_buffer.end();){
			if(it->second != NULL)
				delete it->second;
			it = obj_state_buffer.erase(it);
		}
		pthread_mutex_unlock(&mutex_obj_state_buffer);
	}
}

int BicycleObjectsBuffer::check(const MyString conditon){
	int ret = -1;
	map<int, ObjectBicycle*>::iterator it;
	if(pthread_mutex_lock(&mutex_obj_state_buffer) == 0){
		for(it = obj_state_buffer.begin(); it != obj_state_buffer.end();){
			if(it->second == NULL){
				LOG_WARN("The Object id = %d is NULL.\n", it->first);
				continue;
			}
			ObjectBicycle& obj_temp = *it->second;

			int objdrop = -1;
			bool isMatchedFlag = obj_temp[obj_temp.isMatched];
			if(!isMatchedFlag){
				objdrop = obj_temp[obj_temp.drop];
				obj_temp.insertKeyValue(obj_temp.drop, objdrop+1);
			}else{
				obj_temp.insertKeyValue(obj_temp.isMatched, false);
			}

			objdrop = obj_temp[obj_temp.drop];
			if(objdrop >= conditon.toInt()){
				delete it->second;
				it = obj_state_buffer.erase(it);
			}else{
				it++;
			}
		}
		ret = obj_state_buffer.size();
		pthread_mutex_unlock(&mutex_obj_state_buffer);
	}
	return ret;
}

bool BicycleObjectsBuffer::empty(){
	bool res = false;
	if(pthread_mutex_lock(&mutex_obj_state_buffer) == 0){
		if(obj_state_buffer.empty())
			res = true;
		pthread_mutex_unlock(&mutex_obj_state_buffer);
	}
	return res;
}

int BicycleObjectsBuffer::insert(ObjectBicycle& obj){
	int ret = -1;
	map<int, ObjectBicycle*>::iterator it;
	if(pthread_mutex_lock(&mutex_obj_state_buffer) == 0){
		int id = obj[obj.id];
		it = obj_state_buffer.find(id);
		if(it != obj_state_buffer.end()){
			LOG_WARN("object is already exist.\n");
			ret = -1;
		}else{
			ObjectBicycle* obj_temp = new ObjectBicycle(obj);
			obj_temp->insertKeyValue(obj_temp->drop, 0);
			obj_temp->insertKeyValue(obj_temp->isMatched, true);
			obj_temp->insertKeyValue(obj_temp->id, obj_id);
			obj_state_buffer.insert(map<int, ObjectBicycle*>::value_type(obj_id, obj_temp));

			ret = 0;
			obj_id += 1;
		}
		pthread_mutex_unlock(&mutex_obj_state_buffer);

	}
	return 0;
}

ObjectBicycle* BicycleObjectsBuffer::begin(){
	is_it_begin = true;
	it = obj_state_buffer.begin();
	if(it != obj_state_buffer.end()){
		return it->second;
	}else{
		is_it_begin = false;
		return NULL;
	}
}

ObjectBicycle* BicycleObjectsBuffer::Next(){
	if(!is_it_begin) return NULL;
	++it;
	if(it == obj_state_buffer.end()){
		is_it_begin = false;
		return NULL;
	}else{
		return it->second;
	}
}

int BicycleObjectsBuffer::updateObject(ObjectBicycle& dist, ObjectBicycle& obj){
	int xmin = obj[obj.xmin];
	dist.insertKeyValue(dist.xmin, xmin);
	int ymin = obj[obj.ymin];
	dist.insertKeyValue(dist.ymin, ymin);
	int xmax = obj[obj.xmax];
	dist.insertKeyValue(dist.xmax, xmax);
	int ymax = obj[obj.ymax];
	dist.insertKeyValue(dist.ymax, ymax);
	double confidence = obj[obj.confidence];
	dist.insertKeyValue(dist.confidence, confidence);

	dist.insertKeyValue(dist.drop, 0);
	dist.insertKeyValue(dist.isMatched, true);

	int trackcount = dist[dist.trackCount];
	dist.insertKeyValue(dist.trackCount, trackcount+1);

	return dist[dist.id];
}

//Find the most matching target and return the id value
int BicycleObjectsBuffer::Match(ObjectBicycle& obj, double threshold){
	int ret = -1;
	ObjectBicycle* matched_obj = NULL;
	if(this->obj_state_buffer.size() == 0) {
		LOG_ERROR("match error, buffer is empty\n");
		return -1;
	}
	cv::Rect rect_src(cv::Point(obj[obj.xmin], obj[obj.ymin]), cv::Point(obj[obj.xmax], obj[obj.ymax]));

	vector<ObjectBicycle*> result;
	map<int, ObjectBicycle*>::iterator it;
	if(pthread_mutex_lock(&mutex_obj_state_buffer) == 0){
		for(it=this->obj_state_buffer.begin(); it!=this->obj_state_buffer.end(); it++){
			ObjectBicycle& obj_temp = *it->second;
			cv::Rect rect(cv::Point(obj_temp[obj_temp.xmin], obj_temp[obj_temp.ymin]), cv::Point(obj_temp[obj_temp.xmax], obj_temp[obj_temp.ymax]));

			double score = Toolset::Instance()->RectOverlap(rect_src, rect);
			if(score > threshold){
				it->second->insertKeyValue(obj_temp.matchScore, score);
				result.push_back(it->second);
			}
		}
		double max_score = 0.0f;
		for(int i=0; i<result.size(); i++){
			ObjectBicycle& obj_temp = *result[i];
			double obj_matchscore = obj_temp[obj_temp.matchScore];
			if(obj_matchscore > max_score){
				ret = obj_temp[obj_temp.id];
				matched_obj = result[i];
			}
		}
		//将匹配度最高的目标，isMatched标志位置为true,跟踪次数+1
		if(matched_obj != NULL){
			ObjectBicycle& obj_temp = *matched_obj;
			updateObject(obj_temp, obj);
		}
		pthread_mutex_unlock(&mutex_obj_state_buffer);
	}
	return ret;
}



















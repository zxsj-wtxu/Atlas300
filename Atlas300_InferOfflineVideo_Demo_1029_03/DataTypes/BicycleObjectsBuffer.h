/*
 * BicycleObjectsBuffer.h
 *
 *  Created on: 2019年7月13日
 *      Author: wtxu
 */

#ifndef BICYCLEOBJECTSBUFFER_H_
#define BICYCLEOBJECTSBUFFER_H_
#include "Toolset.h"
#include "ObjectBicycle.h"

class BicycleObjectsBuffer {
public:
	BicycleObjectsBuffer();
	virtual ~BicycleObjectsBuffer();

	bool empty();
	int insert(ObjectBicycle& obj);
	int check(const MyString conditon);
	int Match(ObjectBicycle& obj, double threshold);	//匹配缓冲区内的目标，返回匹配度最高的目标ID，没找到返回-1
	int updateObject(ObjectBicycle& dist, ObjectBicycle& obj); //更新目标，返回目标id

	ObjectBicycle* begin();
	ObjectBicycle* Next();
private:
	int obj_id = 0;
	bool is_it_begin = false;
	map<int, ObjectBicycle*>::iterator it;
	map<int, ObjectBicycle*> obj_state_buffer;
	pthread_mutex_t mutex_obj_state_buffer = PTHREAD_MUTEX_INITIALIZER;

};

#endif /* BICYCLEOBJECTSBUFFER_H_ */

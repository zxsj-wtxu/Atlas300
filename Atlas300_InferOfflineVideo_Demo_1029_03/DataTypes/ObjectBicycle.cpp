/*
 * ObjectBicycle.cpp
 *
 *  Created on: 2019年7月13日
 *      Author: wtxu
 */

#include "ObjectBicycle.h"

ObjectBicycle::ObjectBicycle() {
	// TODO Auto-generated constructor stub
	deviceData.insertKeyValue(MyString("id"), -1);
	deviceData.insertKeyValue(MyString("xmin"), 0);
	deviceData.insertKeyValue(MyString("xmax"), 0);
	deviceData.insertKeyValue(MyString("ymin"), 0);
	deviceData.insertKeyValue(MyString("ymax"), 0);
	deviceData.insertKeyValue(MyString("type"), 0);
	deviceData.insertKeyValue(MyString("confidence"), 0.0f);
	deviceData.insertKeyValue(MyString("drop"), 0);
	deviceData.insertKeyValue(MyString("isMatched"), false);
	deviceData.insertKeyValue(MyString("trackCount"), 0);
	deviceData.insertKeyValue(MyString("isCounted"), false);
	deviceData.insertKeyValue(MyString("matchScore"), 0.0f);

}

ObjectBicycle::~ObjectBicycle() {
	// TODO Auto-generated destructor stub
}

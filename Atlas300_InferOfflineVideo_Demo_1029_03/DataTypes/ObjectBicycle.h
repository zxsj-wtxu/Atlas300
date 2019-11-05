/*
 * ObjectBicycle.h
 *
 *  Created on: 2019年7月13日
 *      Author: wtxu
 */

#ifndef OBJECTBICYCLE_H_
#define OBJECTBICYCLE_H_
#include "BaseClass.h"

class ObjectBicycle:public BaseClass {
public:
	ObjectBicycle();
	virtual ~ObjectBicycle();

	using BaseClass::operator[];

	const MyString id = "id";
	const MyString xmin = "xmin";
	const MyString xmax = "xmax";
	const MyString ymin = "ymin";
	const MyString ymax = "ymax";
	const MyString type = "type";
	const MyString confidence = "confidence";
	const MyString drop = "drop";
	const MyString isMatched = "isMatched";
	const MyString trackCount = "trackCount";
	const MyString isCounted = "isCounted";
	const MyString matchScore = "matchScore";

};

#endif /* OBJECTBICYCLE_H_ */

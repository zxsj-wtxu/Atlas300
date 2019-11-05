/*
 * DetectionParam.h
 *
 *  Created on: 2019年7月12日
 *      Author: wtxu
 */

#ifndef DETECTIONPARAM_H_
#define DETECTIONPARAM_H_

#include "BaseClass.h"

class DetectionParam: public BaseClass {
public:
	DetectionParam();
	DetectionParam(const char* json);
	virtual ~DetectionParam();
};

#endif /* DETECTIONPARAM_H_ */

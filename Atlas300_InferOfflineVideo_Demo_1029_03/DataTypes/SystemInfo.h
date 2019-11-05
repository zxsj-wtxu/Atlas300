/*
 * SystemInfo.h
 *
 *  Created on: 2019年7月11日
 *      Author: wtxu
 */

#ifndef SYSTEMINFO_H_
#define SYSTEMINFO_H_

#include "BaseClass.h"

class SystemInfo : public BaseClass{
public:
	SystemInfo() = delete;
	SystemInfo(const MyString& id);
	SystemInfo(const char* json);
	virtual ~SystemInfo();

	MyString getSystemId();
private:
	MyString systemid;
};

#endif /* SYSTEMINFO_H_ */

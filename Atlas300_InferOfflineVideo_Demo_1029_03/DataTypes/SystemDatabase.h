/*
 * SystemDatabase.h
 *
 *  Created on: 2019年7月20日
 *      Author: wtxu
 */

#ifndef SYSTEMDATABASE_H_
#define SYSTEMDATABASE_H_

#include "SystemInfo.h"
#include "DataBaseContainer.h"

class SystemDatabase: public SystemInfo, public DataBaseContainer {
public:
	SystemDatabase(const MyString& tablename, const MyString& id);
	SystemDatabase(const MyString& tablename, const char* json);
	virtual ~SystemDatabase();

	int setJson(const MyString& json);

	int clear();
	int remove(const MyString& key);
	int insertKeyValue(const MyString& key, int value);
	int insertKeyValue(const MyString& key, double value);
	int insertKeyValue(const MyString& key, long int value);
	int insertKeyValue(const MyString& key, MyString value);

	using SystemInfo::operator[];
	using DataBaseContainer::operator[];
private:
	int insertToDatabase(const MyString& json);

};

#endif /* SYSTEMDATABASE_H_ */

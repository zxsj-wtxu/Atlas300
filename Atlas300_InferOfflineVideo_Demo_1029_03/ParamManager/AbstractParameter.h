/*
 * AbstractParameter.h
 *
 *  Created on: 2018年2月10日
 *      Author: wtxu
 */

#ifndef ABSTRACTPARAMETER_H_
#define ABSTRACTPARAMETER_H_

#include <iostream>
#include <string>
#include <map>
using namespace std;

class AbstractParameter {
public:
	AbstractParameter();
	virtual ~AbstractParameter();

	virtual string getTableName()=0;
	virtual string getParam(string field)=0;
	virtual string getCreateTableSqlCmd()=0;
	virtual map<string, string> getMapSet()=0;
	virtual void setMapSet(map<string, string> mapset)=0;
	virtual string getInsertSqlCmd(string field, string value)=0;
	virtual string updateParamSqlCmd(string field, string value)=0;
	virtual void printLog(int level, string log)=0;

};

#endif /* ABSTRACTPARAMETER_H_ */

/*
 * ParameterManager.h
 *
 *  Created on: 2018年2月9日
 *      Author: wtxu
 */

#ifndef PARAMETERMANAGER_H_
#define PARAMETERMANAGER_H_

#include "sqlite3.h"
#include <iostream>
#include <string>
#include <map>
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

#include "LaneAreaParameter.h"

using namespace SQLite;
using namespace std;

/**
 * 从数据库读取参数表，将表单里面的值更新至系统内存中
 * 如果数据库不存在，则创建数据库，并将系统的默认参数写入数据库中
 * 如果表单不存在，则创建表单，并将默认参数写入到数据库中
 * 所有的表单，其中的参数都是以键值对的形式进行存储，若是属性有多个值时，将多个值拼接成字符串以后再存储
 * */
class ParameterManager {
public:
	ParameterManager();
	virtual ~ParameterManager();

private:
	Database* db;

private:
    static ParameterManager* pm;
	LaneAreaParameter* lap;
	//保存所有的参数列表
	map<string, AbstractParameter*> mapObjects;
public:
	//检查并创建不存在的表单
    static ParameterManager* getInstance();
	void createTable();
	void readDatabase();
	void updateTable(AbstractParameter* ap);
	void printLog(int level, string log);
    void updateTable(map<string, string>& map_params, string tablename);

	AbstractParameter* operator[](string label);
};

#endif /* PARAMETERMANAGER_H_ */

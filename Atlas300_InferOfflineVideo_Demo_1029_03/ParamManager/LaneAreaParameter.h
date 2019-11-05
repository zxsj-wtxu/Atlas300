/*
 * LaneAreaParameter.h
 *
 *  Created on: 2018年1月3日
 *      Author: wtxu
 */

#ifndef LANEAREAPARAMETER_H_
#define LANEAREAPARAMETER_H_
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "AbstractParameter.h"


/**
 * 需要被插入的参数，其标签必须符合以下规定："id","类型","顶点个数","point1","point2","point3","point4"
 * */
class LaneAreaParameter: public AbstractParameter {
public:
	LaneAreaParameter();
	LaneAreaParameter(map<string, string> map_params);
	virtual ~LaneAreaParameter();
public:
	enum AreaType{
		VehicleFlowType,	//检测车流量类型
		ParkingEventType,	//检测停车事件类型
        CongestionEventType,//检测拥堵类型
        DetectROIAreaType	//检测拥堵类型
	};

	class Point{
	public:
		Point(int x, int y);
		string toString();
		int x;
		int y;
	};

	class Basedata{
	public:
		Basedata();
		Basedata(string data);
		string toString();
		//重载[]操作符，以下标的形式，访问vector_points中的每个点
		Point operator[](int index);

		int id;
		int n;
		int type;
        int laneid;
        int direction;
		int x_min = 10000;
		int x_max = -1;
		int y_min = 10000;
		int y_max = -1;
		float slop = 0;
		float offset = 0;

        int eventFlag = 0;
        bool isParkingEvent = false;
		vector<Point> vector_points;
	};
private:
	//以下参数为插入到数据库中使用的参数
	string sqlCmd;
	map<string, string> map_params;
	string tablename = "LaneAreaParameter";
	string version = "v20190616";

private:
	//保存数值类型的区域参数，供各个模块使用
	map<int, Basedata> map_areas;

public:
	string getTableName();
	string getParam(string field);
	string getCreateTableSqlCmd();
	map<string, string> getMapSet();
	void setMapSet(map<string, string> mapset);
	string getInsertSqlCmd(string field, string value);
	string updateParamSqlCmd(string field, string value);

	void printLog(int level, string log);
	int size();

	Basedata* find(int type);
	Basedata operator[](int index);
	string getLabel();
};

#endif /* LANEAREAPARAMETER_H_ */

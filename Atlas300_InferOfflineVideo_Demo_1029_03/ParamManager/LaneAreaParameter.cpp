/*
 * LaneAreaParameter.cpp
 *
 *  Created on: 2018年1月3日
 *      Author: wtxu
 */

#include "LaneAreaParameter.h"

LaneAreaParameter::LaneAreaParameter() {
	// TODO Auto-generated constructor stub

//	map_params.insert(map<string, string>::value_type ("1", "1,4,0,10,30,20,30,1000,1000,1000,1000"));
//	LaneAreaParameter::Basedata bd("1,4,0,10,30,20,30,1000,1000,1000,1000");
//	map_areas.insert(map<int, Basedata>::value_type (1,bd));

    tablename = "LaneAreaParameter";
	sqlCmd += "CREATE TABLE ";
	sqlCmd += this->tablename;
	sqlCmd += " (key TEXT, value TEXT)";
}

LaneAreaParameter::LaneAreaParameter(map<string, string> map_params){
    tablename = "LaneAreaParameter";
	setMapSet(map_params);
	sqlCmd += "CREATE TABLE ";
	sqlCmd += this->tablename;
	sqlCmd += " (key TEXT, value TEXT)";
}

LaneAreaParameter::~LaneAreaParameter() {
	// TODO Auto-generated destructor stub
}

string LaneAreaParameter::getParam(string field){
	return map_params[field];
}

string LaneAreaParameter::getCreateTableSqlCmd(){
	return this->sqlCmd;
}

string LaneAreaParameter::getTableName(){
	return tablename;
}

map<string, string> LaneAreaParameter::getMapSet(){
	return this->map_params;
}

string LaneAreaParameter::getInsertSqlCmd(string field, string value){
	string cmd = "INSERT INTO "+tablename+" VALUES (\""+field+"\",\""+value +"\")";

	return cmd;
}

string LaneAreaParameter::updateParamSqlCmd(string field, string value){
	//更新参数
	map_params[field] = value;
	//更新数据库语句
	string cmd = "UPDATE "+tablename+" SET value=\""+value+"\" WHERE key=\""+value+"\"";

	return cmd;
}
/**
 * 将符合规定的参数，插入或更新至参数列表中
 * */
void LaneAreaParameter::setMapSet(map<string, string> mapset){
	map<string,string>::iterator itemp;
	int i=0;

	if(mapset.empty()){
		return;
	}

	this->map_params.clear();
	this->map_areas.clear();

	for(itemp = mapset.begin(); itemp != mapset.end(); ++itemp){
		this->map_params[itemp->first] = itemp->second;

		LaneAreaParameter::Basedata bd(itemp->second);
		map_areas.insert(map<int, Basedata>::value_type (i++,bd));
	}
}

void LaneAreaParameter::printLog(int level, string log){
	cout <<__DATE__<<" " <<__TIME__<<" " << __FILE__ <<" " <<__LINE__ <<" " <<log;
}

int LaneAreaParameter::size(){
	return map_areas.size();
}

LaneAreaParameter::Point::Point(int x, int y){
	this->x = x;
	this->y = y;
}
string LaneAreaParameter::Point::toString(){
	stringstream strStream;
	strStream <<x <<"," <<y;

	return strStream.str();
}

LaneAreaParameter::Basedata::Basedata(){
}

LaneAreaParameter::Basedata::Basedata(string data){
    vector<string> Result;
	const char Regex = ',';

    int pos = 0;
    int npos = 0;
    int regexlen = sizeof(Regex);
    while((npos=data.find(Regex, pos))!=-1)
    {
        string tmp = data.substr(pos,npos-pos);
        Result.push_back(tmp);
        pos = npos+regexlen;
    }

    Result.push_back(data.substr(pos,data.length()-pos));
    string* pStr = Result.data();
    id 		= atoi((*pStr++).c_str());
    type 	= atoi((*pStr++).c_str());
    n 		= atoi((*pStr++).c_str());
    laneid  = atoi((*pStr++).c_str());
    direction = atoi((*pStr++).c_str());

    for(int i=0; i<n; i++){
        int x = atoi((*pStr++).c_str());
        int y = atoi((*pStr++).c_str());
        Point p(x, y);
        vector_points.push_back(p);

        //将四边形描述成一条直线
        x_min = min(x, x_min);
        x_max = max(x, x_max);
        y_min = min(y, y_min);
        y_max = max(y, y_max);
        //计算直线的直线方程
        slop = (float)(y_max - y_min)/(x_max - x_min);
        offset=(float)(y_max - slop * x_max);
    }
}

string LaneAreaParameter::Basedata::toString(){
	stringstream strStream;
	strStream <<id <<"," << n <<"," << type;

	for(int i=0; i<vector_points.size(); ++i){
		Point p = vector_points[i];
		strStream <<"," <<p.x <<"," <<p.y;
	}

	return strStream.str();
}

LaneAreaParameter::Basedata* LaneAreaParameter::find(int type){
	map<int, Basedata>::iterator itemp = map_areas.begin();
	for(itemp; itemp != map_areas.end(); itemp++){
		if(itemp->second.type == type) return &itemp->second;
	}
	return NULL;
}

LaneAreaParameter::Basedata LaneAreaParameter::operator [](int index){
    if(index < map_areas.size()){
		map<int, Basedata>::iterator itemp = map_areas.find(index);
		return itemp->second;
	}
	printLog(0, "Index Out Of Bounds Exception.");
    Basedata bd("-1,0,4,-1,-1,-1,-1,-1,-1,-1,-1");

	return bd;
}

LaneAreaParameter::Point LaneAreaParameter::Basedata::operator[](int index){
    if(index < vector_points.size()){
		return vector_points[index];
	}
	Point p(-1,-1);
	return p;
}

string LaneAreaParameter::getLabel(){
	return tablename;
}


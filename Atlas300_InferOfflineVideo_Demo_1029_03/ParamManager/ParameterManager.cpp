/*
 * ParameterManager.cpp
 *
 *  Created on: 2018年2月9日
 *      Author: wtxu
 */

#include "ParameterManager.h"
ParameterManager* ParameterManager::pm = new ParameterManager;

ParameterManager::ParameterManager() {
	// TODO Auto-generated constructor stub
    db = new Database("rvPub.db",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

    lap = new LaneAreaParameter;
    mapObjects.insert(map<string, AbstractParameter*>::value_type(lap->getTableName(), lap));
	//如果没有，创建表单，存在就跳过
	createTable();
	//读取数据库中的表单信息
	readDatabase();
}

ParameterManager* ParameterManager::getInstance()
{
    return pm;
}

ParameterManager::~ParameterManager() {
	// TODO Auto-generated destructor stub
	delete lap;
}

/**
 * AbstractParameter*必须为new 对象
 * */
void ParameterManager::updateTable(AbstractParameter* ap){
	string cmd;

	string tablename = ap->getTableName();
	map<string, string> mapset = ap->getMapSet();
	map<string, string>::iterator iter;

	//更新参数表，根据传递的参数类，找到对应的参数表
	map<string, AbstractParameter*>::iterator itemp = mapObjects.find(tablename);
	if(itemp != mapObjects.end()){
		itemp->second->setMapSet(mapset);
	}

	//更新数据库
	const bool bExists = db->tableExists(tablename.c_str());
	if(bExists){
		//先清空表单内的数据
		cmd = "delete from "+tablename;
		db->exec(cmd.c_str());

		for(iter = mapset.begin(); iter != mapset.end(); ++iter)
		{
			cmd = ap->getInsertSqlCmd(iter->first, iter->second);
			int nb = db->exec(cmd.c_str());
		}
	}
}

/**
 * AbstractParameter*必须为new 对象
 * */
void ParameterManager::updateTable(map<string, string> &map_params, string tablename)
{
    string cmd;

    map<string, string> mapset = map_params;
    map<string, string>::iterator iter;

    //更新参数表，根据传递的参数类，找到对应的参数表
    map<string, AbstractParameter*>::iterator itemp = mapObjects.find(tablename);
    AbstractParameter* ap = itemp->second;
    if(itemp != mapObjects.end()){
        itemp->second->setMapSet(mapset);
    }

    //更新数据库
    const bool bExists = db->tableExists(tablename.c_str());
    if(bExists){
        //先清空表单内的数据
        cmd = "delete from "+tablename;
        db->exec(cmd.c_str());

        for(iter = mapset.begin(); iter != mapset.end(); ++iter)
        {
            cmd = ap->getInsertSqlCmd(iter->first, iter->second);
            int nb = db->exec(cmd.c_str());
        }
    }
}

void ParameterManager::createTable(){
	string cmd;
	map<string, AbstractParameter*>::iterator iter_object = this->mapObjects.begin();

	string tablename;
	AbstractParameter* ap;
	map<string, string> mapset;
	map<string, string>::iterator iter;

	//循环所有的表单
	for(iter_object; iter_object != this->mapObjects.end(); ++iter_object){
		ap = iter_object->second;

		tablename = ap->getTableName();
		mapset = ap->getMapSet();
		iter = mapset.begin();

		//判断是否存在此表单,不存在则创建表单，并写入默认数据
		const bool bExists = db->tableExists(tablename.c_str());
		if(!bExists){
            cout << ap->getCreateTableSqlCmd() <<endl;
			db->exec(ap->getCreateTableSqlCmd().c_str());
			//插入数据
			for(iter = mapset.begin(); iter != mapset.end(); ++iter)
			{
				cmd = ap->getInsertSqlCmd(iter->first, iter->second);
				int nb = db->exec(cmd.c_str());
			}
		}else{
//			printLog(3, tablename+" is checked.");
		}
	}
	return;
}

void ParameterManager::readDatabase(){
	string cmd;
	map<string, AbstractParameter*>::iterator iter_object = this->mapObjects.begin();

	for(iter_object; iter_object != this->mapObjects.end(); ++iter_object){
		cmd = "SELECT * FROM "+ iter_object->second->getTableName();
		{
			map<string, string> map_temp;

			Statement   query(*db, cmd.c_str());
			while(query.executeStep()){
				const std::string key = query.getColumn(0);
				const std::string value = query.getColumn(1);
				map_temp[key] = value;
			}
			iter_object->second->setMapSet(map_temp);
		}
	}
}

void ParameterManager::printLog(int level, string log){

	cout <<__DATE__<<" "<<__TIME__<<" "<<__FILE__<<" "<< __LINE__<<" :" << log << endl;
}

/**
 *
 * */
AbstractParameter* ParameterManager::operator[](string label){
	map<string, AbstractParameter*>::iterator itemp = mapObjects.find(label);
	if(itemp != mapObjects.end()){	//如果存在规定的参数属性值，则将参数设置到参数列表中
		return itemp->second;
	}else{
		printLog(0, " The parameter :"+label+" is not found.");
		return NULL;
	}
}

/*
 * DatabaseController.cpp
 *
 *  Created on: 2019年7月12日
 *      Author: wtxu
 */

#include "DatabaseController.h"
pthread_mutex_t DatabaseController::mutex_init = PTHREAD_MUTEX_INITIALIZER;
DatabaseController* DatabaseController::controller = nullptr;

void DatabaseController::Release(){
	if(pthread_mutex_lock(&mutex_init) == 0){
		if(controller != nullptr){
			delete controller;
		}
		pthread_mutex_unlock(&mutex_init);
	}
}

DatabaseController* DatabaseController::Instance(){
	if(pthread_mutex_lock(&mutex_init) == 0){
		if(controller == nullptr){
			controller = new DatabaseController;
		}
		pthread_mutex_unlock(&mutex_init);
	}
	return controller;
}

DatabaseController::DatabaseController() {
	// TODO Auto-generated constructor stub
	if(db == nullptr)
		db = new Database("rvPub.db",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
}

DatabaseController::~DatabaseController() {
	// TODO Auto-generated destructor stub
	if(db != nullptr)
		delete db;
}

int DatabaseController::CreateTable(DataBaseContainer* container){
	int ret = -1;
	if(db->tableExists(container->getTableName().c_str())){
		ret = -1;
	}else{
		db->exec(container->getCreateTableSqlCmd().c_str());
		ret = 0;
	}
	return ret;
}

int DatabaseController::Export(const MyString& table, vector<DataBaseContainer*>& containers){
	if(!db->tableExists(table.c_str())){
		return -1;
	}
	DataBaseContainer container(table);
	Statement   query(*db, container.getQueryCmd("").c_str());

	while(query.executeStep()){
		container.clear();
		int count = query.getColumnCount();
		string _key = query.getColumn(0);
		string _value = query.getColumn(1);

		container.insertKeyValue(_key, _value);
		containers.push_back(new DataBaseContainer(container));
	}
	return 0;
}

int DatabaseController::Query(DataBaseContainer& container, const MyString& key){
	if(!db->tableExists(container.getTableName().c_str())){
		return -1;
	}
	Statement   query(*db, container.getQueryCmd(key).c_str());
	if(query.executeStep()){
		return 0;
	}else{
		return -1;
	}
}

int DatabaseController::GetItem(DataBaseContainer& container, const MyString& key){
	if(!db->tableExists(container.getTableName().c_str())){
		return -1;
	}
	Statement   query(*db, container.getQueryCmd(key).c_str());
	while(query.executeStep()){
		int count = query.getColumnCount();
		string _key = query.getColumn(0);
		string _value = query.getColumn(1);

		container.insertKeyValue(_key, _value);
	}
	return 0;
}

int DatabaseController::Update(DataBaseContainer* container){
	int ret = -1;
	MyString cmd;
	map<const MyString, MyString> mapset;
	map<const MyString, MyString>::iterator iter;
	bool IsTableExist = false;

	if(!db->tableExists(container->getTableName().c_str())){
		return -1;
	}else{
		mapset = container->getData();
		for(iter = mapset.begin(); iter != mapset.end(); ++iter) {
			if( Query(*container, iter->first) == 0 ){
				int nb = db->exec(container->getUpdateCmd(iter->first, iter->second).c_str());
				if(nb < 0){
					return -1;
				}else{
					return  0;
				}
			}
		}
	}

	return 0;
}

int DatabaseController::Delete(DataBaseContainer* container){
	int ret = -1;
	MyString cmd;
	map<const MyString, MyString> mapset;
	map<const MyString, MyString>::iterator iter;
	bool IsTableExist = false;

	if(!db->tableExists(container->getTableName().c_str())){
		return -1;
	}else{
		mapset = container->getData();
		for(iter = mapset.begin(); iter != mapset.end(); ++iter) {
			if( Query(*container, iter->first) == 0 ){
				cout << container->getDeleteCmd(iter->first) <<endl;
				int nb = db->exec(container->getDeleteCmd(iter->first).c_str());
				if(nb < 0){
					return -1;
				}else{
					ret = 0;
				}
			}
		}
	}

	return ret;
}

int DatabaseController::Insert(DataBaseContainer* container){
	int ret = -1;
	MyString cmd;
	map<const MyString, MyString> mapset;
	map<const MyString, MyString>::iterator iter;
	bool IsTableExist = false;

	if(!db->tableExists(container->getTableName().c_str())){
		return -1;
	}else{
		mapset = container->getData();
		for(iter = mapset.begin(); iter != mapset.end(); ++iter) {
			if( Query(*container, iter->first) >= 0 ){
				cout << "insert failed, The key is already existed." <<endl;
				return -1;
			}
		}
	}

	for(iter = mapset.begin(); iter != mapset.end(); ++iter) {
		cout << container->getInsertCmd(iter->first, iter->second) <<endl;
		int nb = db->exec(container->getInsertCmd(iter->first, iter->second).c_str());
		if(nb < 0){
			return -1;
		}
	}

	return 0;
}


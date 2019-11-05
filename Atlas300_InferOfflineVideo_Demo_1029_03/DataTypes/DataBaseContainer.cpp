/*
 * DataBaseContainer.cpp
 *
 *  Created on: 2019年7月12日
 *      Author: wtxu
 */

#include "DataBaseContainer.h"

DataBaseContainer::DataBaseContainer(const MyString& table):tablename(table) {
	// TODO Auto-generated constructor stub
}

DataBaseContainer::DataBaseContainer(const DataBaseContainer& container):tablename(container.tablename){
	this->key_value = container.key_value;
}

DataBaseContainer::~DataBaseContainer() {
	// TODO Auto-generated destructor stub
}

MyString DataBaseContainer::toDatabaseString(){
	stringstream sstr;
	map<const MyString, MyString>::iterator itemp;

	if(pthread_mutex_lock(&mutex_key_value) == 0){
		for(itemp = key_value.begin(); itemp != key_value.end(); ++itemp){
			sstr << itemp->first <<":" << itemp->second <<endl;
		}
		pthread_mutex_unlock(&mutex_key_value);
	}
	return sstr.str();
}

MyString DataBaseContainer::getTableName(){
	return tablename;
}

MyString DataBaseContainer::getCreateTableSqlCmd(){
	return "CREATE TABLE " + this->tablename + " (key TEXT PRIMARY KEY, value TEXT)";
}

MyString DataBaseContainer::insertParamSqlCmd(const MyString& key, const MyString& value){
	return "INSERT INTO "+tablename+" VALUES (\'"+key+"\',\'"+value +"\')";
}

MyString DataBaseContainer::updateParamSqlCmd(const MyString& key, const MyString& value){
	return "UPDATE "+tablename+" SET value=\'"+value+"\' WHERE key=\'"+key+"\'";
}

MyString DataBaseContainer::queryParamSqlCmd(const MyString& key){
	return "SELECT * FROM "+tablename+" WHERE key=\'"+key+"\'";
}

MyString DataBaseContainer::queryParamSqlCmd(){
	return "SELECT * FROM "+tablename;
}

MyString DataBaseContainer::deleteParamSqlCmd(const MyString& key){
	return "DELETE FROM "+tablename+" WHERE key=\'"+key+"\'";
}

map<const MyString, MyString> DataBaseContainer::getData(){
	return key_value;
}

void DataBaseContainer::set(map<const MyString, MyString> key_value){
	int i=0;

	if(key_value.empty()){
		LOG_WARN("The specified parameters is empty.");
		return;
	}
	if(!this->key_value.empty()){
		this->key_value.clear();
	}

	map<const MyString, MyString>::iterator itemp;
	for(itemp = key_value.begin(); itemp != key_value.end(); ++itemp){
		this->key_value.insert(map<const MyString, MyString>::value_type(itemp->first, itemp->second));
	}
}

int DataBaseContainer::insertKeyValue(const MyString& key, int value){
	int ret;
//	LOG_DEBUG("DataBaseContainer insert <%s,%d>.\n", key.c_str(), value);
	map<const MyString, MyString>::iterator it = key_value.find(key);
	if(pthread_mutex_lock(&mutex_key_value) == 0){
		if(it == key_value.end()){
			key_value.insert(map<const MyString, MyString>::value_type(key, std::to_string(value)));
			ret = 0;
		}else{
			LOG_INFO("Key-value pair <%s,%d> already exists, insertion failure.\n", key.c_str(), value);
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_key_value);
	}

	return ret;
}

int DataBaseContainer::insertKeyValue(const MyString& key, double value){
	int ret;
//	LOG_DEBUG("insert <%s,%f>.\n",key.c_str(), value);
	map<const MyString, MyString>::iterator it = key_value.find(key);
	if(pthread_mutex_lock(&mutex_key_value) == 0){
		if(it == key_value.end()){
			key_value.insert(map<const MyString, MyString>::value_type(key, std::to_string(value)));
			ret = 0;
		}else{
			LOG_INFO("Key-value pair <%s,%f> already exists, insertion failure.\n",key.c_str(), value);
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_key_value);
	}

	return ret;
}

int DataBaseContainer::insertKeyValue(const MyString& key, long int value){
	int ret;
//	LOG_DEBUG("insert <%s,%ld>.\n",key.c_str(), value);
	map<const MyString, MyString>::iterator it = key_value.find(key);
	if(pthread_mutex_lock(&mutex_key_value) == 0){
		if(it == key_value.end()){
			key_value.insert(map<const MyString, MyString>::value_type(key, std::to_string(value)));
			ret = 0;
		}else{
			LOG_INFO("Key-value pair <%s,%ld> already exists, insertion failure.\n",key.c_str(), value);
			ret = -1;
		}
		pthread_mutex_unlock(&mutex_key_value);
	}

	return ret;
}
int DataBaseContainer::insertKeyValue(MyString& key, MyString& value){
	int ret;
	map<const MyString, MyString>::iterator it = key_value.find(key);
	if(pthread_mutex_lock(&mutex_key_value) == 0){
		if(it == key_value.end()){
			key_value.insert(map<const MyString, MyString>::value_type(key, value));
			ret = 0;
		}else{
			LOG_INFO("Key-value pair already <%s,%s> exists, update pair.\n", key.c_str(), value.c_str());
			it->second = value;
			ret = 0;
		}
		pthread_mutex_unlock(&mutex_key_value);
	}

	return ret;
}

int DataBaseContainer::insertKeyValue(const MyString& key, MyString& value){
	int ret;
//	LOG_DEBUG("insert <%s,%s>.\n", key.c_str(), value.c_str());
	map<const MyString, MyString>::iterator it = key_value.find(key);
	if(pthread_mutex_lock(&mutex_key_value) == 0){
		if(it == key_value.end()){
			key_value.insert(map<const MyString, MyString>::value_type(key, value));
			ret = 0;
		}else{
//			LOG_INFO("Key-value pair already <%s,%s> exists, update pair.\n", key.c_str(), value.c_str());
			it->second = value;
			ret = 0;
		}
		pthread_mutex_unlock(&mutex_key_value);
	}

	return ret;
}

int DataBaseContainer::insertKeyValue(const MyString& key, MyString value){
	int ret;
//	LOG_DEBUG("DataBaseContainer insert <%s,%s>.\n", key.c_str(), value.c_str());
	if(pthread_mutex_lock(&mutex_key_value) == 0){
		map<const MyString, MyString>::iterator it = key_value.find(key);

		if(it == key_value.end()){
			key_value.insert(map<const MyString, MyString>::value_type(key, value));
			ret = 0;
		}else{
//			LOG_INFO("Key-value pair already <%s,%s> exists, update pair.\n", key.c_str(), value.c_str());
			it->second = value;
			ret = 0;
		}
		pthread_mutex_unlock(&mutex_key_value);
	}

	return ret;
}

MyString DataBaseContainer::getInsertCmd(const MyString& key, const MyString& value){
	return insertParamSqlCmd(key, value);
}

MyString DataBaseContainer::getUpdateCmd(const MyString& key, const MyString& value){
	return updateParamSqlCmd(key, value);
}

MyString DataBaseContainer::getQueryCmd(const MyString& key){
	if(key.empty()){
		return queryParamSqlCmd();
	}else{
		return queryParamSqlCmd(key);
	}
}

MyString DataBaseContainer::getDeleteCmd(const MyString& key){
	return deleteParamSqlCmd(key);
}

void DataBaseContainer::clear(){
	if(pthread_mutex_lock(&mutex_key_value) == 0){
		key_value.clear();
		pthread_mutex_unlock(&mutex_key_value);
	}
}

int DataBaseContainer::size(){
	int size = 0;
	if(pthread_mutex_lock(&mutex_key_value) == 0){
		size = key_value.size();
		pthread_mutex_unlock(&mutex_key_value);
	}
	return size;
}

MyString DataBaseContainer::operator[](MyString& key){
	MyString value = 0;
	if(pthread_mutex_lock(&mutex_key_value) == 0){
		map<const MyString, MyString>::iterator it = key_value.find(key);
		if(it != key_value.end()){
			value = it->second;
		}else{
			value = "";
		}
		pthread_mutex_unlock(&mutex_key_value);
	}
	return value;
}

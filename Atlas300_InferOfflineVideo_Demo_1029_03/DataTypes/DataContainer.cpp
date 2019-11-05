/*
 * DataContainer.cpp
 *
 *  Created on: 2019年7月11日
 *      Author: wtxu
 */

#include "DataContainer.h"

DataContainer::DataContainer() {
	// TODO Auto-generated constructor stub
}

bool DataContainer::checkJsonInt(const rapidjson::Value& document, const char* member){
	if(!document.HasMember(member)) {
		LOG_ERROR("JSON data error, no member of %s \n", member);
		return false;
	}else{
		if(!document[member].IsInt()){
			LOG_ERROR("JSON data error, member of %s should be Int. \n",member);
			return false;
		}
	}
	return true;
}

bool DataContainer::checkJsonString(const rapidjson::Value& document, const char* member){
	if(!document.HasMember(member)) {
		LOG_ERROR("JSON data error, no member of %s \n", member);
		return false;
	}else{
		if(!document[member].IsString()){
			LOG_ERROR("JSON data error, member of %s should be MyString. \n",member);
			return false;
		}
	}
	return true;
}

bool DataContainer::checkJsonObject(const rapidjson::Value& document, const char* member){
	if(!document.HasMember(member)) {
		LOG_ERROR("JSON data error, no member of %s \n", member);
		return false;
	}else{
		if(!document[member].IsObject()){
			LOG_ERROR("JSON data error, member of %s should be Object. \n",member);
			return false;
		}
	}
	return true;
}

DataContainer::~DataContainer() {
	// TODO Auto-generated destructor stub
	map<const MyString, Basedata*>::iterator it;
	for(it = this->params.begin(); it != this->params.end();){
		if(it->second != NULL){
			delete it->second;
			it = this->params.erase(it);
		}else{
			it = this->params.erase(it);
		}
	}

}

int DataContainer::clear(){
	int ret = -1;

	map<const MyString, Basedata*>::iterator it;
	for(it = this->params.begin(); it != this->params.end();){
		if(it->second != NULL){
//			LOG_INFO("The key-value %s %s is deleted  \n", it->first.c_str(), it->second->GetString().c_str());
			delete it->second;
			ret = 0;
		}else{
//			LOG_INFO("The key %s is exist, but its value is null\n", it->first.c_str());
			ret = 0;
		}
		it = params.erase(it);
	}
	return ret;
}

int DataContainer::remove(const MyString& key){
	int ret = -1;
	if(pthread_mutex_lock(&mutx_params) == 0){
		map<const MyString, Basedata*>::iterator it = params.find(key);
		if(it == params.end()){
			LOG_INFO("The key %s is not exist\n", key.c_str());
			ret = -1;
		}else{
			if(it->second != NULL){
//				LOG_INFO("The key-value %s %s is deleted  \n", key.c_str(), it->second->GetString().c_str());
				delete it->second;
				ret = 0;
			}else{
//				LOG_INFO("The key %s is exist, but its value is null\n", key.c_str());
				ret = 0;
			}
			it = params.erase(it);
		}
		pthread_mutex_unlock(&mutx_params);
	}
	return ret;
}

int DataContainer::insertKeyValue(const MyString& key, long int value){
	int ret = -1;
	if(pthread_mutex_lock(&mutx_params) == 0){
		map<const MyString, Basedata*>::iterator it = params.find(key);
		if(it == params.end()){
			Basedata* bd = new Basedata(value);
			params.insert(map<const MyString, Basedata*>::value_type(key, bd));
			ret = 0;
		}else{
			if(it->second != NULL){
				it->second->SetValue(value);
				ret = 0;
//				LOG_INFO("The key %s already exist, update value to %s\n", key.c_str(), std::to_string(value).c_str());
			}else{
				it->second = new Basedata(value);
//				LOG_INFO("The key %s already exist, but its value is null\n", key.c_str());
			}
		}
		pthread_mutex_unlock(&mutx_params);
	}
	return ret;
}

int DataContainer::insertKeyValue(const MyString& key, double value){
	int ret = -1;
	if(pthread_mutex_lock(&mutx_params) == 0){
		map<const MyString, Basedata*>::iterator it = params.find(key);
		if(it == params.end()){
			Basedata* bd = new Basedata(value);
			params.insert(map<const MyString, Basedata*>::value_type(key, bd));
			ret = 0;
		}else{
			if(it->second != NULL){
				it->second->SetValue(value);
				ret = 0;
//				LOG_INFO("The key %s already exist, update value to %s\n", key.c_str(), std::to_string(value).c_str());
			}else{
				it->second = new Basedata(value);
//				LOG_INFO("The key %s already exist, but its value is null\n", key.c_str());
			}
		}
		pthread_mutex_unlock(&mutx_params);
	}
	return ret;
}

int DataContainer::insertKeyValue(const MyString& key, bool value){
	int ret = -1;
	if(pthread_mutex_lock(&mutx_params) == 0){
		map<const MyString, Basedata*>::iterator it = params.find(key);
		if(it == params.end()){
			params.insert(map<const MyString, Basedata*>::value_type(key, new Basedata(value)));
			ret = 0;
		}else{
			if(it->second != NULL){
				it->second->SetValue(value);
				ret = 0;
//				LOG_INFO("The key %s already exist, update value to %s\n", key.c_str(), std::to_string(value).c_str());
			}else{
				it->second = new Basedata(value);
//				LOG_INFO("The key %s already exist, but its value is null\n", key.c_str());
			}
		}
		pthread_mutex_unlock(&mutx_params);
	}
	return ret;
}

int DataContainer::insertKeyValue(const MyString& key, int value){
	int ret = -1;
	if(pthread_mutex_lock(&mutx_params) == 0){
		map<const MyString, Basedata*>::iterator it = params.find(key);
		if(it == params.end()){
			Basedata* bd = new Basedata(value);
			params.insert(map<const MyString, Basedata*>::value_type(key, bd));
			ret = 0;
		}else{
			if(it->second != NULL){
				it->second->SetValue(value);
				ret = 0;
//				LOG_INFO("The key %s already exist, update value to %s\n", key.c_str(), std::to_string(value).c_str());
			}else{
				it->second = new Basedata(value);
//				LOG_INFO("The key %s already exist, but its value is null\n", key.c_str());
			}
		}
		pthread_mutex_unlock(&mutx_params);
	}
	return ret;
}

int DataContainer::insertKeyValue(const MyString& key, const MyString& value){
	int ret = -1;
	if(pthread_mutex_lock(&mutx_params) == 0){
		map<const MyString, Basedata*>::iterator it = params.find(key);
		if(it == params.end()){
			params.insert(map<const MyString, Basedata*>::value_type(key, new Basedata(value)));
			ret = 0;
		}else{
			if(it->second != NULL){
				it->second->SetValue(value);
				ret = 0;
//				LOG_INFO("The key %s already exist, update value to %s\n", key.c_str(), value.c_str());
			}else{
				it->second = new Basedata(value);
//				LOG_INFO("The key %s already exist, but its value is null\n", key.c_str());
			}
		}
		pthread_mutex_unlock(&mutx_params);
	}
	return ret;
}

DataContainer::Basedata DataContainer::operator[](const MyString& key){
	DataContainer::Basedata bd("");
	if(pthread_mutex_lock(&mutx_params) == 0){
		map<const MyString, Basedata*>::iterator it = params.find(key);
		if(it == params.end()){
			bd.SetValue("");
		}else{
			if(it->second != NULL){
				bd.SetValue(MyString(it->second->GetString()));
			}else{
				bd.SetValue("");
			}
		}
		pthread_mutex_unlock(&mutx_params);
	}
	return bd;
}

DataContainer& DataContainer::operator=(const DataContainer& data){
	if(pthread_mutex_lock(&mutx_params) == 0){
		this->params.clear();
		map<const MyString, Basedata*>::const_iterator it;
		for(it = data.params.begin(); it != data.params.end(); it++){
			this->params.insert(map<const MyString, Basedata*>::value_type(it->first, new Basedata(it->second->GetString())));
		}
		pthread_mutex_unlock(&mutx_params);
	}
	return *this;
}

#ifdef RAPIDJSON
int DataContainer::setJson(const MyString& json){
	char obj_name[10] = {0};
	Document document;
	document.Parse(json.c_str());
	if (document.HasParseError()) {
		LOG_INFO("JSON parsing error\n");
		return -1;
	}
	if(!document.IsObject()) return -1;
	const rapidjson::Value &root = document.GetObject();

	map<const MyString, Basedata*>::iterator it;
	for (Value::ConstMemberIterator itr = root.MemberBegin(); itr != root.MemberEnd(); ++itr){
		it = params.find(MyString(itr->name.GetString()));
		if(it == params.end()){
			if(checkJsonString(root, itr->name.GetString())){
				params.insert(map<const MyString, Basedata*>::value_type(MyString(itr->name.GetString()), new Basedata(MyString(itr->value.GetString()))));
			}else if(checkJsonInt(root, itr->name.GetString())){
				params.insert(map<const MyString, Basedata*>::value_type(MyString(itr->name.GetString()), new Basedata(std::to_string(itr->value.GetInt()))));
			}else{
				LOG_ERROR("Json type mismatch\n");
			}
		}else{
			if(checkJsonString(root, itr->name.GetString())){
				it->second->SetValue(MyString(itr->value.GetString()));
			}else if(checkJsonInt(root, itr->name.GetString())){
				it->second->SetValue(std::to_string(itr->value.GetInt()));
			}else{
				LOG_ERROR("Json type mismatch\n");
			}
		}

	}
	return 0;
}

DataContainer::DataContainer(const MyString& json){
	new (this)DataContainer();
	char obj_name[10] = {0};
	Document document;
	document.Parse(json.c_str());
	if (document.HasParseError()) {
		rapidjson::ParseErrorCode code = document.GetParseError();
		LOG_INFO("JSON parsing error\n");
		return;
	}
	if(!document.IsObject()) return;
	const rapidjson::Value &root = document.GetObject();

	for (Value::ConstMemberIterator itr = root.MemberBegin(); itr != root.MemberEnd(); ++itr){
		if(checkJsonString(root, itr->name.GetString())){
			params.insert(map<const MyString, Basedata*>::value_type(MyString(itr->name.GetString()), new Basedata(MyString(itr->value.GetString()))));
		}else if(checkJsonInt(root, itr->name.GetString())){
			params.insert(map<const MyString, Basedata*>::value_type(MyString(itr->name.GetString()), new Basedata(MyString(std::to_string(itr->value.GetInt())))));
		}else{
			LOG_ERROR("error.\n");
		}
	}
}

const MyString DataContainer::toJson(){
	int ret = 0;
	char object_name[10] = {0};

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	map<const MyString, Basedata*>::iterator it;
	for(it = this->params.begin(); it != this->params.end(); it++){
		if(it->second != NULL && !it->second->empty()){

			document.AddMember(Value(it->first.c_str(), document.GetAllocator()).Move(),
					Value(it->second->GetString().c_str(), document.GetAllocator()).Move(), allocator);
		}else{
			LOG_INFO("you must set %s.\n",it->first.c_str());
			return "";
		}
	}

	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	document.Accept(writer);

//	LOG_DEBUG("DataContainer Info:\n%s\n",buffer.GetString());

	return MyString(buffer.GetString());
}
#endif
DataContainer::Basedata::Basedata(const Basedata& bd){
	value = bd.GetString();
}

DataContainer::Basedata::Basedata(const MyString& data){
	value = data;
}

bool DataContainer::Basedata::empty(){
	return value.empty();
}

void DataContainer::Basedata::SetValue(const MyString& data){
	value = data;
}

MyString DataContainer::Basedata::GetString() const{
	return value;
}

const bool DataContainer::Basedata::GetBool(){
	if(atoi(value.c_str())){
		return true;
	}else{
		return false;
	}
	return false;
}

const int DataContainer::Basedata::GetInt(){
	return atoi(value.c_str());
}

const double DataContainer::Basedata::GetDouble(){
	return atof(value.c_str());
}

const long DataContainer::Basedata::GetLong(){
	return atol(value.c_str());
}

const long long DataContainer::Basedata::GetLongLong(){
	return atoll(value.c_str());
}

DataContainer::Basedata::operator int(){
	return value.toInt();
}

DataContainer::Basedata::operator long long(){
	return value.toLongLong();
}

DataContainer::Basedata::operator long(){
	return atol(value.c_str());
}

DataContainer::Basedata::operator double(){
	return value.toDouble();

}

DataContainer::Basedata::operator bool(){
	if(atoi(value.c_str())){
		return true;
	}else{
		return false;
	}
}

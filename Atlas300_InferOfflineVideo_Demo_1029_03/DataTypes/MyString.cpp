/*
 * MyString.cpp
 *
 *  Created on: 2019年7月12日
 *      Author: wtxu
 */

#include "MyString.h"

MyString::MyString() {
	// TODO Auto-generated constructor stub
	if(NULL == str){
		str = (char*)malloc(1);
		str[0] = '\0';
	}
}

MyString::MyString(const MyString& s){
	int size = s.length() + 1;
	this->str = (char*)malloc(size);
	memset(this->str, 0, size);
	memcpy(this->str, s.c_str(), s.length());
}

MyString::MyString(const string& s){
	int size = s.length() + 1;
	this->str = (char*)malloc(size);
	memset(this->str, 0, size);
	memcpy(this->str, s.c_str(), s.length());
}

MyString::MyString(int value){
	int size = std::to_string(value).length() + 1;
	this->str = (char*)malloc(size);
	memset(this->str, 0, size);
	memcpy(this->str, std::to_string(value).c_str(), std::to_string(value).length());
}

MyString::MyString(double value){
	string temp_str = std::to_string(value);
	int temp_size = temp_str.length();
	char* temp = new char[temp_size];
	memset(temp, 0, temp_size);
	memcpy(temp, temp_str.c_str(), temp_size);
	int nLen = temp_size;
	while( nLen ) {
		nLen -- ;
		if ( temp[nLen] == '.') {
			temp[nLen] = 0;
			break;
		} else if ( temp[nLen] == '0') {
			temp[nLen] = 0;
		}else{
			break;
		}
	}

	int size = temp_size + 1;
	this->str = (char*)malloc(size);
	memset(this->str, 0, size);
	memcpy(this->str, temp, temp_size);

	delete[] temp;
}

MyString::MyString(long int value){
	int size = std::to_string(value).length() + 1;
	this->str = (char*)malloc(size);
	memset(this->str, 0, size);
	memcpy(this->str, std::to_string(value).c_str(), std::to_string(value).length());
}

MyString::MyString(const char* str){
	if(NULL == str){
		this->str = (char*)malloc(1);
		this->str[0] = '\0';
	}else{
		int size = strlen(str) + 1;
		this->str = (char*)malloc(size);
		memset(this->str, 0, size);
		memcpy(this->str, str, strlen(str));
	}
}

bool MyString::compare(const MyString& value){
	if(strcmp(this->str, value.str) == 0){
		return true;
	}else{
		return false;
	}
}

MyString operator + (const MyString &it, int value){
	MyString str;
	int size = std::to_string(value).length() + it.size() + 1;
    if(str.str != NULL)
    	free(str.str);
	str.str = (char*)malloc(size);
	memset(str.str, 0, size);

	sprintf(str.str,"%s%s", it.c_str(), std::to_string(value).c_str());
    return str;
}

MyString operator + (int value, const MyString &it){
	MyString str;
	int size = std::to_string(value).length() + it.size() + 1;
    if(str.str != NULL)
    	free(str.str);
	str.str = (char*)malloc(size);
	memset(str.str, 0, size);

	sprintf(str.str,"%s%s", std::to_string(value).c_str(), it.c_str());
    return str;
}

MyString operator + (const MyString &it, double value){
	MyString str;
	string temp_str = std::to_string(value);
	int temp_size = temp_str.length();
	char* temp = new char[temp_size];
	memset(temp, 0, temp_size);
	memcpy(temp, temp_str.c_str(), temp_size);
	int nLen = temp_size;
	while( nLen ) {
		nLen -- ;
		if ( temp[nLen] == '.') {
			temp[nLen] = 0;
			break;
		} else if ( temp[nLen] == '0') {
			temp[nLen] = 0;
		}else{
			break;
		}
	}

	int size = strlen(temp) + it.size() + 1;
    if(str.str != NULL)
    	free(str.str);
	str.str = (char*)malloc(size);
	memset(str.str, 0, size);

	sprintf(str.str,"%s%s", it.c_str(), temp);
	delete[] temp;
    return str;
}
MyString operator + (double value, const MyString &it){
	MyString str;
	string temp_str = std::to_string(value);
	int temp_size = temp_str.length();
	char* temp = new char[temp_size];
	memset(temp, 0, temp_size);
	memcpy(temp, temp_str.c_str(), temp_size);
	int nLen = temp_size;
	while( nLen ) {
		nLen -- ;
		if ( temp[nLen] == '.') {
			temp[nLen] = 0;
			break;
		} else if ( temp[nLen] == '0') {
			temp[nLen] = 0;
		}else{
			break;
		}
	}

	int size = strlen(temp) + it.size() + 1;
    if(str.str != NULL)
    	free(str.str);
	str.str = (char*)malloc(size);
	memset(str.str, 0, size);

	sprintf(str.str,"%s%s", temp, it.c_str());
	delete[] temp;
    return str;
}

MyString operator + (const MyString &it, long int value){
	MyString str;
	int size = std::to_string(value).length() + it.size() + 1;
    if(str.str != NULL)
    	free(str.str);
	str.str = (char*)malloc(size);
	memset(str.str, 0, size);

	sprintf(str.str,"%s%s", it.c_str(), std::to_string(value).c_str());
    return str;
}

MyString operator + (long int value, const MyString &it){
	MyString str;
	int size = std::to_string(value).length() + it.size() + 1;
    if(str.str != NULL)
    	free(str.str);
	str.str = (char*)malloc(size);
	memset(str.str, 0, size);

	sprintf(str.str,"%s%s", std::to_string(value).c_str(), it.c_str());
    return str;
}

MyString operator + (const MyString &it, const char *str1)
{
	MyString str;
	int size = strlen(str1) + it.size() + 1;
    if(str.str != NULL)
    	free(str.str);
	str.str = (char*)malloc(size);
	memset(str.str, 0, size);

	sprintf(str.str,"%s%s", it.c_str(), str1);
    return str;
}

MyString operator + (const char *str1, const MyString &it)
{
	MyString str;
	int size = strlen(str1) + it.size() + 1;
    if(str.str != NULL)
    	free(str.str);
	str.str = (char*)malloc(size);
	memset(str.str, 0, size);

	sprintf(str.str,"%s%s", str1, it.c_str());
    return str;
}
MyString operator + (const string& str2, const string& str1)
{
    MyString str;
    int size = str1.size() + str2.size() + 1;
    if(str.str != NULL)
    	free(str.str);
    str.str = (char*)malloc(size);
    memset(str.str, 0, size);

    sprintf(str.str,"%s%s", str2.c_str(), str1.c_str());
    return str;
}

MyString operator + (const MyString& str2, const MyString& str1)
{
    MyString str;
    int size = str1.size() + str2.size() + 1;
    if(str.str != NULL)
    	free(str.str);
    str.str = (char*)malloc(size);
    memset(str.str, 0, size);

    sprintf(str.str,"%s%s", str2.c_str(), str1.c_str());
    return str;
}

ostream & operator<<(ostream &out, const MyString& obj)
{
    out << string(obj.str);
    return out;
}

int MyString::toInt() const{
	return atoi(str);
}

double MyString::toDouble() const{
	return atof(str);
}

long long MyString::toLongLong() const{
	return atoll(str);
}

bool MyString::operator<(const MyString& s)const{
	return strcmp(this->str , s.str) < 0? true:false;
}

const MyString& MyString::operator=(const string& s){
	if(str != NULL){
		free(this->str);
		str = NULL;
	}
	int size = s.length() + 1;
	this->str = (char*)malloc(size);
	memset(this->str, 0, size);
	memcpy(this->str, s.c_str(), s.length());

	return *this;
}

const MyString& MyString::operator=(int value){
	if(str != NULL){
		free(this->str);
		str = NULL;
	}

	int size = std::to_string(value).length() + 1;
	this->str = (char*)malloc(size);
	memset(this->str, 0, size);
	memcpy(this->str, std::to_string(value).c_str(), std::to_string(value).length());

	return *this;
}

const MyString& MyString::operator=(double value){
	if(str != NULL){
		free(this->str);
		str = NULL;
	}
	string temp_str = std::to_string(value);
	int temp_size = temp_str.length();
	char* temp = new char[temp_size];
	memset(temp, 0, temp_size);
	memcpy(temp, temp_str.c_str(), temp_size);
	int nLen = temp_size;
	while( nLen ) {
		nLen -- ;
		if ( temp[nLen] == '.') {
			temp[nLen] = 0;
			break;
		} else if ( temp[nLen] == '0') {
			temp[nLen] = 0;
		}else{
			break;
		}
	}

	int size = strlen(temp) + 1;
	this->str = (char*)malloc(size);
	memset(this->str, 0, size);
	memcpy(this->str, temp, strlen(temp));

	delete[] temp;
	return *this;
}

const MyString& MyString::operator=(long int value){
	if(str != NULL){
		free(this->str);
		str = NULL;
	}

	int size = std::to_string(value).length() + 1;
	this->str = (char*)malloc(size);
	memset(this->str, 0, size);
	memcpy(this->str, std::to_string(value).c_str(), std::to_string(value).length());

	return *this;
}

const MyString& MyString::operator=(const char* s){
	if(str != NULL){
		free(this->str);
		str = NULL;
	}
	if(s != NULL){
		int size = strlen(s) + 1;
		this->str = (char*)malloc(size);
		memset(this->str, 0, size);
		memcpy(this->str, s, strlen(s));
	}else{
		this->str = new char[1];
		this->str[0] = '\0';
	}
	return *this;
}

MyString& MyString::operator += (int value){
	int size = std::to_string(value).length() +this->size() + 1;
	char* _newstr= (char*)malloc(size);
	memset(_newstr, 0, size);

	sprintf(_newstr, "%s%s", this->c_str(), std::to_string(value).c_str());
	free(this->str);
	str = NULL;

	this->str = _newstr;
	return *this;
}

MyString& MyString::operator += (double value){
	string temp_str = std::to_string(value);
	int temp_size = temp_str.length();
	char* temp = new char[temp_size];
	memset(temp, 0, temp_size);
	memcpy(temp, temp_str.c_str(), temp_size);
	int nLen = temp_size;
	while( nLen ) {
		nLen -- ;
		if ( temp[nLen] == '.') {
			temp[nLen] = 0;
			break;
		} else if ( temp[nLen] == '0') {
			temp[nLen] = 0;
		}else{
			break;
		}
	}

	int size = strlen(temp) +this->size() + 1;
	char* _newstr= (char*)malloc(size);
	memset(_newstr, 0, size);

	sprintf(_newstr, "%s%s", this->c_str(), temp);
	free(this->str);
	str = NULL;

	this->str = _newstr;
	delete[] temp;
	return *this;
}

MyString& MyString::operator += (long int value){
	int size = std::to_string(value).length() +this->size() + 1;
	char* _newstr= (char*)malloc(size);
	memset(_newstr, 0, size);

	sprintf(_newstr, "%s%s", this->c_str(), std::to_string(value).c_str());
	free(this->str);
	str = NULL;

	this->str = _newstr;
	return *this;
}

MyString& MyString::operator += (const char* str2){
	int size = strlen(str2) +this->size() + 1;
	char* _newstr= (char*)malloc(size);

	memset(_newstr, 0, size);
	sprintf(_newstr, "%s%s", this->c_str(), str2);
	free(this->str);
	str = NULL;

	this->str = _newstr;

	return *this;
}

MyString& MyString::operator += (const string& str2){
	int size = str2.length() +this->size() + 1;
	char* _newstr= (char*)malloc(size);

	memset(_newstr, 0, size);
	sprintf(_newstr, "%s%s", this->c_str(), str2.c_str());
	free(this->str);
	str = NULL;

	this->str = _newstr;

	return *this;
}

MyString& MyString::operator += (const MyString& str2)
{
	int size = str2.length() +this->size() + 1;
	char* _newstr= (char*)malloc(size);

	memset(_newstr, 0, size);
	sprintf(_newstr, "%s%s", this->c_str(), str2.c_str());
	free(this->str);
	str = NULL;

	this->str = _newstr;

	return *this;
}

const MyString& MyString::operator=(const MyString& s){
	if (&s != this){
		if(str != NULL){
			free(str);
			str = NULL;
		}
		int size = s.length() + 1;
		this->str = (char*)malloc(size);
		memset(this->str, 0, size);
		memcpy(this->str, s.c_str(), s.length());
	}
	return *this;
}

int MyString::copy(char* dist, size_t __n, size_t __pos) const{
	memcpy(dist, str+__pos, __n);
	return strlen(str+__pos);
}

const char* MyString::c_str() const{
	return str;
}

bool MyString::empty() const{
	if(NULL == str) return true;

	if(strlen(str) == 0){
		return true;
	}else{
		return false;
	}
}

int MyString::size() const{
	return strlen(str);
}

int MyString::length() const{
	return strlen(str);
}

MyString::~MyString() {
	// TODO Auto-generated destructor stub
	free(str);
	str = NULL;
}


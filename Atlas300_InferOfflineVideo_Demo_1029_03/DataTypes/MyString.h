/*
 * MyString.h
 *
 *  Created on: 2019年7月12日
 *      Author: wtxu
 */

#ifndef MYSTRING_H_
#define MYSTRING_H_
#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
using namespace std;

class MyString {
public:
	MyString();
	MyString(int value);
	MyString(double value);
	MyString(long int value);
	MyString(const char* str);
	MyString(const string& s);
	MyString(const MyString& s);
	~MyString();

	int toInt() const;
	double toDouble() const;
	long long toLongLong() const;

	bool empty() const;
	int length() const;
	int size() const;
	const char* c_str() const;
	int copy(char* dist, size_t __n, size_t __pos = 0) const;

	bool operator<(const MyString& s)const;

	bool compare(const MyString& value);

	const MyString& operator=(int value);
	const MyString& operator=(double value);
	const MyString& operator=(long int value);
	const MyString& operator=(const char* s);
	const MyString& operator=(const string& s);
	const MyString& operator=(const MyString& s);

	MyString& operator += (int value);
	MyString& operator += (double value);
	MyString& operator += (long int value);
	MyString& operator += (const char* str2);
	MyString& operator += (const string& str2);
	MyString& operator += (const MyString& str2);

	friend MyString operator + (const MyString &it, const char *str1);
	friend MyString operator + (const MyString &it, int value);
	friend MyString operator + (const MyString &it, double value);
	friend MyString operator + (const MyString &it, long int value);
	friend MyString operator + (const string& str2, const string& str1);
	friend MyString operator + (const MyString& str2, const MyString& str1);

	friend MyString operator + (int value, const MyString &it);
	friend MyString operator + (double value, const MyString &it);
	friend MyString operator + (long int value, const MyString &it);
	friend MyString operator + (const char *str1,const MyString &it);
	friend MyString operator + (const string& str1, const string& str2);
	friend ostream & operator<<(ostream &out, const MyString& obj);

private:
	char* str = NULL;
};

#endif /* MYSTRING_H_ */

/*
 * DatabaseController.h
 *
 *  Created on: 2019年7月12日
 *      Author: wtxu
 */

#ifndef DATABASECONTROLLER_H_
#define DATABASECONTROLLER_H_
#include "sqlite3.h"
#include "DataBaseContainer.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
using namespace SQLite;
using namespace std;

class DatabaseController {
private:
	DatabaseController();
	static pthread_mutex_t mutex_init;
	static DatabaseController* controller;
	Database* db = nullptr;
public:
	static void Release();
	static DatabaseController* Instance();
	virtual ~DatabaseController();

	int CreateTable(DataBaseContainer* container);
	int Insert(DataBaseContainer* container);
	int Delete(DataBaseContainer* container);
	int Update(DataBaseContainer* container);
	int Query(DataBaseContainer& container, const MyString& key);
	int GetItem(DataBaseContainer& container, const MyString& key);
	int Export(const MyString& table, vector<DataBaseContainer*>& containers);

};

#endif /* DATABASECONTROLLER_H_ */

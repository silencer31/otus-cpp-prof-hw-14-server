#pragma once

#include <mutex>

#include "sqlite3.h"

class DataStorage
{
public:
	DataStorage() : 
		handle(nullptr)
	{}
	
	~DataStorage() {
		sqlite3_close(handle);
	}

	bool init_database();

	bool handle_request(const std::string& request);

private:
	sqlite3* handle;

	std::mutex data_mutex;

	const char* db_name = "tasks_sqlite_db.sqlite";
};
#pragma once

#include <mutex>

#include "sqlite3.h"

class DataStorage
{
public:
	DataStorage() = delete;

	explicit DataStorage(const char* file_path) :
		handle(nullptr),
		db_path(file_path),
		base_opened(false)
	{
		base_opened = init_database();
	}
	
	~DataStorage() {
		if (base_opened) {
			sqlite3_close(handle);
		}
	}

	// Открытие файла с базой данных.
	bool init_database();

	// Обработка запроса к базе данных.
	bool handle_request(const std::string& request);

private:
	sqlite3* handle;
	const char* db_path; // Путь к файлу с базой данных.

	bool base_opened; // Флаг успешного открытия базы данных.

	std::mutex data_mutex; // Для защиты базы данных при параллельном доступе.	
};
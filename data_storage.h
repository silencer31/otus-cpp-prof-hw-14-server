#pragma once

#include <mutex>

#include "sqlite3.h"

class DataStorage
{
public:
	DataStorage() = delete;

	explicit DataStorage(const char* file_path) :
		handle(nullptr),
		stmt(nullptr),
		db_path(file_path),
		base_opened(false)
	{
		base_opened = init_database();
	}
	
	~DataStorage() {
		if (stmt != nullptr) {
			sqlite3_finalize(stmt);
		}

		if (base_opened) {
			sqlite3_close(handle);
		}
	}

	// Открытие файла с базой данных.
	bool init_database();

	// Обработка запроса к базе данных.
	bool handle_request(const std::string& request);

	// Проверка пары логин/пароль.
	bool check_login(const std::string& username, const std::string& password);

private:
	sqlite3* handle;
	sqlite3_stmt* stmt;

	const char* db_path; // Путь к файлу с базой данных.

	bool base_opened; // Флаг успешного открытия базы данных.

	std::mutex data_mutex; // Для защиты базы данных при параллельном доступе.	
};

using storage_shared = std::shared_ptr<DataStorage>;
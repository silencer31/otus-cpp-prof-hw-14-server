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

	// Получить значение-число из определённой ячейки в таблице.
	bool get_row_value_int(const std::string& table, const std::string& col, const int row,
		int& value);

	// Получить значение-текст из определённой ячейки в таблице.
	bool get_row_value_txt(const std::string& table, const std::string& col, const int row,
		std::string& value);

	// Ищем строку, содержащую переданное значение в указанном столбце.
	// Если находим, возвращаем значение-число из той же строки, но из второго указанного столбца.
	bool get_corresp_col_value_int(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, int& get_value);

	// Ищем строку, содержащую переданное значение в указанном столбце.
	// Если находим, возвращаем значение-текст из той же строки, но из второго указанного столбца.
	bool get_corresp_col_value_txt(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, std::string& get_value);

	// Обновить значение в столбце таблицы с соблюдением условия.
	bool set_value_with_clause(const std::string& table,
		const std::string& target_col, const std::string& new_value,
		const std::string& clause_col, const std::string& clause_val);

	// Добавить в таблицу новую строку со значениями.
	bool insert_values(const std::string& table, const std::string& values);

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
#pragma once

#include <mutex>
#include <vector>

#include "sqlite3.h"

using vector_str = std::vector<std::string>;
using vector_int = std::vector<int>;

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

	// Выполнить запрос к базе данных с возвратом ошибки.
	bool execute_db_request(const std::string& request, char** err_ptr);

	// Получить максимальное числовое значение из указанного столбца.
	bool get_column_max_int_value(const std::string& table, const std::string& col, int& max_value);

	// Узнать, есть ли переданное строкое значение в указанном столбце таблицы.
	bool get_txt_value_presence(const std::string& table, const std::string& col, const std::string& find_value);

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

	// Узнать, сколько раз значение встречается в указанном столбце указанной таблицы.
	int get_occurances_number(const std::string& table, const std::string& find_col, const std::string& find_value);
		

	// Перебираем все строки, содержащие переданное значение в указанном столбце.
	// Если находим, берём значение-число из той же строки, но из второго указанного столбца и добавляем его в вектор чисел.
	void get_corresp_col_int_vector(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, vector_int& values);


	// Ищем строку, содержащую переданное значение в указанном столбце.
	// Если находим, возвращаем значение-текст из той же строки, но из второго указанного столбца.
	bool get_corresp_col_value_txt(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, std::string& get_value);

	// Ищем строку, содержащую переданное значение в указанном столбце.
	// Собираем значения из переданных столбцов в той же строке.
	bool get_corresp_columns_txt_values(const std::string& table,
		const std::string& find_col, const vector_str& target_columns,
		const std::string& find_value, vector_str& values);


	// Обновить значение в столбце таблицы с соблюдением условия.
	bool set_value_with_clause(const std::string& table,
		const std::string& target_col, const std::string& new_value,
		const std::string& clause_col, const std::string& clause_val);

	// Добавить в таблицу новую строку со значениями для всех столбцов.
	bool insert_row(const std::string& table, const std::string& values);

	// Добавить в таблицу несколько новых строк со значениями для всех столбцов.
	bool insert_rows(const std::string& table, const vector_str& rows);

	// Добавить в таблицу новую строку со значениями для указанных столбцов.
	bool insert_row_for_columns(const std::string& table, const vector_str& columns, const std::string& values);

	// Добавить в таблицу несколько новых строк со значениями для указанных столбцов.
	bool insert_rows_for_columns(const std::string& table, const vector_str& columns, const vector_str& rows);

	// Удалить из таблицы строку с указанием столбца и значения в нём.
	bool delete_row(const std::string& table, const std::string& column, const std::string& value);

	// Удалить из таблицы строки с указанием столбца и значений в нём.
	bool delete_rows(const std::string& table, const std::string& column, const vector_str& values);


private:
	sqlite3* handle;
	sqlite3_stmt* stmt;

	const char* db_path; // Путь к файлу с базой данных.

	bool base_opened; // Флаг успешного открытия базы данных.

	std::mutex data_mutex; // Для защиты базы данных при параллельном доступе.	
};

using storage_unique = std::unique_ptr<DataStorage>;
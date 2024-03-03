#pragma once

#include <mutex>
#include <vector>
#include <string>
#include <memory>

#include "sqlite3.h"

using vector_str = std::vector<std::string>;
using vector_int = std::vector<int>;

/**
* @brief Класс - хранилище данных. Реализует методы, позволяющие работать непосредственно с базой.
*/
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

	/**
	* Заблокировать мьютекс для доступа к БД.
	*/
	void lock_db() {
		data_mutex.lock();
	}
	
	/**
	* Разблокировать мьютекс для доступа к БД.
	*/
	void unlock_db() {
		data_mutex.unlock();
	}

	/**
	* Создать соединение с БД.
	*/
	bool init_database();

	/**
	* Обработка запроса к базе данных.
	* @param request запрос к базе данных
	*/
	bool handle_request(const std::string& request);

	/**
	* Выполнить запрос к базе данных с возвратом ошибки.
	* @param request запрос к базе данных
	* @param err_ptr параметр для получения ошибки выполнения запроса
	*/
	bool execute_db_request(const std::string& request, char** err_ptr);

	/**
	* Получить максимальное числовое значение из указанного столбца.
	* @param table имя таблицы
	* @param col столбец, в котором ищем значение
	* @param max_value параметр для получения целочисленного значения 
	*/
	bool get_column_max_int_value(const std::string& table, const std::string& col, int& max_value);

	/**
	* Узнать, есть ли переданное строкое значение в указанном столбце таблицы.
	* @param table имя таблицы
	* @param col столбец, который проверяем на наличие значения
	* @param find_value искомое значение  
	*/
	bool get_txt_value_presence(const std::string& table, const std::string& col, const std::string& find_value);

	/**
	* Получить значение-число из определённой ячейки в таблице.
	* @param table имя таблицы
	* @param col столбец, из которого получаем целочисленное значение
	* @param row номер строки
	* @param value параметр для получения целочисленного значения
	*/
	bool get_row_value_int(const std::string& table, const std::string& col, const int row,
		int& value);
 
	/**
	* Получить значение-текст из определённой ячейки в таблице.
	* @param table имя таблицы
	* @param col столбец, из которого получаем строковое значение
	* @param row номер строки
	* @param value параметр для получения строкового значения
	*/
	bool get_row_value_txt(const std::string& table, const std::string& col, const int row,
		std::string& value);

	/**
	* Получить все значения-числа из указанного столбца.
	* @param table имя таблицы
	* @param col столбец, из которого получаем числовые значения
	* @param values вектор для получения целочисленных значений
	*/
	int get_column_values_int(const std::string& table, const std::string& col, vector_int& values);
 
	/**
	* Получить все значения-строки из указанного столбца.
	* @param table имя таблицы
	* @param col столбец, из которого получаем строковые значения
	* @param values вектор для получения строковых значений
	*/
	int get_column_values_txt(const std::string& table, const std::string& col, vector_str& values);

	/**
	* Получить все значения-числа из столбца num_col и значения-строки из столбца txt_col.
	* @param table имя таблицы
	* @param num_col столбец, из которого получаем числовые значения
	* @param txt_col столбец, из которого получаем строковые значения
	* @param numbers вектор для получения целочисленных значений
	* @param strings вектор для получения строковых значений
	*/
	int get_pairs_int_txt(const std::string& table, const std::string& num_col, const std::string& txt_col,
		vector_int& numbers, vector_str& strings);

	/**
	* Ищем строку, содержащую переданное значение в указанном столбце.
	* Если находим, возвращаем значение-число из той же строки, но из второго указанного столбца.
	* @param table имя таблицы
	* @param find_col искомый столбец
	* @param target_col целевой столбец
	* @param find_value искомое значение - условие
	* @param get_value параметр для получения целочисленного значения
	*/
	bool get_corresp_col_value_int(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, int& get_value);

	/**
	* Узнать, сколько раз значение встречается в указанном столбце указанной таблицы.
	* @param table имя таблицы
	* @param find_col искомый столбец
	* @param find_value искомое значение
	*/
	int get_occurances_number(const std::string& table, const std::string& find_col, const std::string& find_value);

	/**
	* Перебираем все строки, содержащие переданное значение в указанном столбце.
	* Если находим, берём значение-число из той же строки, но из второго указанного столбца и добавляем его в вектор чисел.
	* @param table имя таблицы
	* @param find_col искомый столбец
	* @param target_col целевой столбец
	* @param find_value искомое значение - условие
	* @param values вектор для получения целочисленных значений
	*/
	int get_corresp_col_int_vector(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, vector_int& values);

	/**
	* Ищем строку, содержащую переданное значение в указанном столбце.
	* Если находим, возвращаем значение-текст из той же строки, но из второго указанного столбца.
	* @param table имя таблицы
	* @param find_col искомый столбец
	* @param target_col целевой столбец
	* @param find_value искомое значение - условие
	* @param get_value параметр для получения строкового значения
	*/
	bool get_corresp_col_value_txt(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, std::string& get_value);

	/**
	* Ищем строку, содержащую переданное значение в указанном столбце.
	* Собираем значения-числа из переданных столбцов в той же строке.
	* @param table имя таблицы
	* @param find_col искомый столбец
	* @param target_columns целевые столбцы
	* @param find_value искомое значение - условие
	* @param values вектор для получения целочисленных значений
	*/
	bool get_corresp_columns_int_values(const std::string& table,
		const std::string& find_col, const vector_str& target_columns,
		const std::string& find_value, vector_int& values);

	/**
	* Ищем строку, содержащую переданное значение в указанном столбце.
	* Собираем значения-строки из переданных столбцов в той же строке.
	* @param table имя таблицы
	* @param find_col искомый столбец
	* @param target_columns целевые столбцы
	* @param find_value искомое значение - условие
	* @param values вектор для получения строковых значений
	*/
	bool get_corresp_columns_txt_values(const std::string& table,
		const std::string& find_col, const vector_str& target_columns,
		const std::string& find_value, vector_str& values);

	/**
	* Обновить значение в ячейке в указанном столбце таблицы с соблюдением условия.
	* @param table имя таблицы
	* @param target_col столбец, в котором нужно изменить значение
	* @param new_value новое значение
	* @param clause_col имя столбца, в котором должно быть значение - условие
	* @param clause_val значение - условие для поиска нужной строки
	*/
	bool set_value_with_clause(const std::string& table,
		const std::string& target_col, const std::string& new_value,
		const std::string& clause_col, const std::string& clause_val);

	/**
	* Обновить значения в ячейках в указанных столбцах таблицы с соблюдением условия.
	* @param table имя таблицы
	* @param target_columns столбцы, в которых нужно изменить значения
	* @param values вектор с новыми значениями 
	* @param clause_col имена столбцов, в которых должны быть значения - условия 
	* @param clause_val значения - условия, для поиска нужных строк 
	*/
	bool set_several_values_with_clause(const std::string& table,
		const vector_str& target_columns, const vector_str& values,
		const std::string& clause_col, const std::string& clause_val);

	/**
	* Добавить в таблицу новую строку со значениями для всех столбцов.
	* @param table имя таблицы
	* @param values строка с набором значений для всех столбцов
	*/
	bool insert_row(const std::string& table, const std::string& values);

	/**
	* Добавить в таблицу несколько строк со значениями для всех столбцов.
	* @param table имя таблицы
	* @param rows вектор со строками, каждая из которых, это набор значений для всех столбцов.
	*/
	bool insert_rows(const std::string& table, const vector_str& rows);

	/**
	* Добавить в таблицу новую строку со значениями для указанных столбцов.
	* @param table имя таблицы
	* @param columns вектор имен столбцов
	* @param values строка с набором значений для столбцов
	*/
	bool insert_row_for_columns(const std::string& table, const vector_str& columns, const std::string& values);

	/**
	* Добавить в таблицу несколько строк со значениями для указанных столбцов.
	* @param table имя таблицы
	* @param columns вектор имен столбцов
	* @param rows вектор со строками, каждая из которых, это набор значений для столбцов.
	*/
	bool insert_rows_for_columns(const std::string& table, const vector_str& columns, const vector_str& rows);

	/**
	* Удалить из таблицы строку с указанием столбца и значения в нём.
	* @param table имя таблицы
	* @param column наименование столбца
	* @param value искомое значение
	*/
	bool delete_row(const std::string& table, const std::string& column, const std::string& value);

	/**
	* Удалить из таблицы строки с указанием столбца и значений в нём.
	* @param table имя таблицы
	* @param column наименование столбца
	* @param values вектор искомых значений
	*/
	bool delete_rows(const std::string& table, const std::string& column, const vector_str& values);

private:
	sqlite3* handle;
	sqlite3_stmt* stmt;

	const char* db_path; // Путь к файлу с базой данных.

	bool base_opened; // Флаг успешного открытия базы данных.

	std::mutex data_mutex; // Для защиты базы данных при параллельном доступе.	
};

using storage_unique = std::unique_ptr<DataStorage>;

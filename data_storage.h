#pragma once

#include <mutex>
#include <vector>
#include <string>
#include <memory>

#include "sqlite3.h"

using vector_str = std::vector<std::string>;
using vector_int = std::vector<int>;

/**
* @brief ����� - ��������� ������. ��������� ������, ����������� �������� ��������������� � �����.
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
	* ������������� ������� ��� ������� � ��.
	*/
	void lock_db() {
		data_mutex.lock();
	}
	
	/**
	* �������������� ������� ��� ������� � ��.
	*/
	void unlock_db() {
		data_mutex.unlock();
	}

	/**
	* ������� ���������� � ��.
	*/
	bool init_database();

	/**
	* ��������� ������� � ���� ������.
	* @param request ������ � ���� ������
	*/
	bool handle_request(const std::string& request);

	/**
	* ��������� ������ � ���� ������ � ��������� ������.
	* @param request ������ � ���� ������
	* @param err_ptr �������� ��� ��������� ������ ���������� �������
	*/
	bool execute_db_request(const std::string& request, char** err_ptr);

	/**
	* �������� ������������ �������� �������� �� ���������� �������.
	* @param table ��� �������
	* @param col �������, � ������� ���� ��������
	* @param max_value �������� ��� ��������� �������������� �������� 
	*/
	bool get_column_max_int_value(const std::string& table, const std::string& col, int& max_value);

	/**
	* ������, ���� �� ���������� ������� �������� � ��������� ������� �������.
	* @param table ��� �������
	* @param col �������, ������� ��������� �� ������� ��������
	* @param find_value ������� ��������  
	*/
	bool get_txt_value_presence(const std::string& table, const std::string& col, const std::string& find_value);

	/**
	* �������� ��������-����� �� ����������� ������ � �������.
	* @param table ��� �������
	* @param col �������, �� �������� �������� ������������� ��������
	* @param row ����� ������
	* @param value �������� ��� ��������� �������������� ��������
	*/
	bool get_row_value_int(const std::string& table, const std::string& col, const int row,
		int& value);
 
	/**
	* �������� ��������-����� �� ����������� ������ � �������.
	* @param table ��� �������
	* @param col �������, �� �������� �������� ��������� ��������
	* @param row ����� ������
	* @param value �������� ��� ��������� ���������� ��������
	*/
	bool get_row_value_txt(const std::string& table, const std::string& col, const int row,
		std::string& value);

	/**
	* �������� ��� ��������-����� �� ���������� �������.
	* @param table ��� �������
	* @param col �������, �� �������� �������� �������� ��������
	* @param values ������ ��� ��������� ������������� ��������
	*/
	int get_column_values_int(const std::string& table, const std::string& col, vector_int& values);
 
	/**
	* �������� ��� ��������-������ �� ���������� �������.
	* @param table ��� �������
	* @param col �������, �� �������� �������� ��������� ��������
	* @param values ������ ��� ��������� ��������� ��������
	*/
	int get_column_values_txt(const std::string& table, const std::string& col, vector_str& values);

	/**
	* �������� ��� ��������-����� �� ������� num_col � ��������-������ �� ������� txt_col.
	* @param table ��� �������
	* @param num_col �������, �� �������� �������� �������� ��������
	* @param txt_col �������, �� �������� �������� ��������� ��������
	* @param numbers ������ ��� ��������� ������������� ��������
	* @param strings ������ ��� ��������� ��������� ��������
	*/
	int get_pairs_int_txt(const std::string& table, const std::string& num_col, const std::string& txt_col,
		vector_int& numbers, vector_str& strings);

	/**
	* ���� ������, ���������� ���������� �������� � ��������� �������.
	* ���� �������, ���������� ��������-����� �� ��� �� ������, �� �� ������� ���������� �������.
	* @param table ��� �������
	* @param find_col ������� �������
	* @param target_col ������� �������
	* @param find_value ������� �������� - �������
	* @param get_value �������� ��� ��������� �������������� ��������
	*/
	bool get_corresp_col_value_int(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, int& get_value);

	/**
	* ������, ������� ��� �������� ����������� � ��������� ������� ��������� �������.
	* @param table ��� �������
	* @param find_col ������� �������
	* @param find_value ������� ��������
	*/
	int get_occurances_number(const std::string& table, const std::string& find_col, const std::string& find_value);

	/**
	* ���������� ��� ������, ���������� ���������� �������� � ��������� �������.
	* ���� �������, ���� ��������-����� �� ��� �� ������, �� �� ������� ���������� ������� � ��������� ��� � ������ �����.
	* @param table ��� �������
	* @param find_col ������� �������
	* @param target_col ������� �������
	* @param find_value ������� �������� - �������
	* @param values ������ ��� ��������� ������������� ��������
	*/
	int get_corresp_col_int_vector(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, vector_int& values);

	/**
	* ���� ������, ���������� ���������� �������� � ��������� �������.
	* ���� �������, ���������� ��������-����� �� ��� �� ������, �� �� ������� ���������� �������.
	* @param table ��� �������
	* @param find_col ������� �������
	* @param target_col ������� �������
	* @param find_value ������� �������� - �������
	* @param get_value �������� ��� ��������� ���������� ��������
	*/
	bool get_corresp_col_value_txt(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, std::string& get_value);

	/**
	* ���� ������, ���������� ���������� �������� � ��������� �������.
	* �������� ��������-����� �� ���������� �������� � ��� �� ������.
	* @param table ��� �������
	* @param find_col ������� �������
	* @param target_columns ������� �������
	* @param find_value ������� �������� - �������
	* @param values ������ ��� ��������� ������������� ��������
	*/
	bool get_corresp_columns_int_values(const std::string& table,
		const std::string& find_col, const vector_str& target_columns,
		const std::string& find_value, vector_int& values);

	/**
	* ���� ������, ���������� ���������� �������� � ��������� �������.
	* �������� ��������-������ �� ���������� �������� � ��� �� ������.
	* @param table ��� �������
	* @param find_col ������� �������
	* @param target_columns ������� �������
	* @param find_value ������� �������� - �������
	* @param values ������ ��� ��������� ��������� ��������
	*/
	bool get_corresp_columns_txt_values(const std::string& table,
		const std::string& find_col, const vector_str& target_columns,
		const std::string& find_value, vector_str& values);

	/**
	* �������� �������� � ������ � ��������� ������� ������� � ����������� �������.
	* @param table ��� �������
	* @param target_col �������, � ������� ����� �������� ��������
	* @param new_value ����� ��������
	* @param clause_col ��� �������, � ������� ������ ���� �������� - �������
	* @param clause_val �������� - ������� ��� ������ ������ ������
	*/
	bool set_value_with_clause(const std::string& table,
		const std::string& target_col, const std::string& new_value,
		const std::string& clause_col, const std::string& clause_val);

	/**
	* �������� �������� � ������� � ��������� �������� ������� � ����������� �������.
	* @param table ��� �������
	* @param target_columns �������, � ������� ����� �������� ��������
	* @param values ������ � ������ ���������� 
	* @param clause_col ����� ��������, � ������� ������ ���� �������� - ������� 
	* @param clause_val �������� - �������, ��� ������ ������ ����� 
	*/
	bool set_several_values_with_clause(const std::string& table,
		const vector_str& target_columns, const vector_str& values,
		const std::string& clause_col, const std::string& clause_val);

	/**
	* �������� � ������� ����� ������ �� ���������� ��� ���� ��������.
	* @param table ��� �������
	* @param values ������ � ������� �������� ��� ���� ��������
	*/
	bool insert_row(const std::string& table, const std::string& values);

	/**
	* �������� � ������� ��������� ����� �� ���������� ��� ���� ��������.
	* @param table ��� �������
	* @param rows ������ �� ��������, ������ �� �������, ��� ����� �������� ��� ���� ��������.
	*/
	bool insert_rows(const std::string& table, const vector_str& rows);

	/**
	* �������� � ������� ����� ������ �� ���������� ��� ��������� ��������.
	* @param table ��� �������
	* @param columns ������ ���� ��������
	* @param values ������ � ������� �������� ��� ��������
	*/
	bool insert_row_for_columns(const std::string& table, const vector_str& columns, const std::string& values);

	/**
	* �������� � ������� ��������� ����� �� ���������� ��� ��������� ��������.
	* @param table ��� �������
	* @param columns ������ ���� ��������
	* @param rows ������ �� ��������, ������ �� �������, ��� ����� �������� ��� ��������.
	*/
	bool insert_rows_for_columns(const std::string& table, const vector_str& columns, const vector_str& rows);

	/**
	* ������� �� ������� ������ � ��������� ������� � �������� � ��.
	* @param table ��� �������
	* @param column ������������ �������
	* @param value ������� ��������
	*/
	bool delete_row(const std::string& table, const std::string& column, const std::string& value);

	/**
	* ������� �� ������� ������ � ��������� ������� � �������� � ��.
	* @param table ��� �������
	* @param column ������������ �������
	* @param values ������ ������� ��������
	*/
	bool delete_rows(const std::string& table, const std::string& column, const vector_str& values);

private:
	sqlite3* handle;
	sqlite3_stmt* stmt;

	const char* db_path; // ���� � ����� � ����� ������.

	bool base_opened; // ���� ��������� �������� ���� ������.

	std::mutex data_mutex; // ��� ������ ���� ������ ��� ������������ �������.	
};

using storage_unique = std::unique_ptr<DataStorage>;

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

	// �������� ����� � ����� ������.
	bool init_database();

	// ��������� ������� � ���� ������.
	bool handle_request(const std::string& request);

	// ��������� ������ � ���� ������ � ��������� ������.
	bool execute_db_request(const std::string& request, char** err_ptr);

	// �������� ������������ �������� �������� �� ���������� �������.
	bool get_column_max_int_value(const std::string& table, const std::string& col, int& max_value);

	// ������, ���� �� ���������� ������� �������� � ��������� ������� �������.
	bool get_txt_value_presence(const std::string& table, const std::string& col, const std::string& find_value);

	// �������� ��������-����� �� ����������� ������ � �������.
	bool get_row_value_int(const std::string& table, const std::string& col, const int row,
		int& value);

	// �������� ��������-����� �� ����������� ������ � �������.
	bool get_row_value_txt(const std::string& table, const std::string& col, const int row,
		std::string& value);

	// ���� ������, ���������� ���������� �������� � ��������� �������.
	// ���� �������, ���������� ��������-����� �� ��� �� ������, �� �� ������� ���������� �������.
	bool get_corresp_col_value_int(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, int& get_value);

	// ������, ������� ��� �������� ����������� � ��������� ������� ��������� �������.
	int get_occurances_number(const std::string& table, const std::string& find_col, const std::string& find_value);
		

	// ���������� ��� ������, ���������� ���������� �������� � ��������� �������.
	// ���� �������, ���� ��������-����� �� ��� �� ������, �� �� ������� ���������� ������� � ��������� ��� � ������ �����.
	void get_corresp_col_int_vector(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, vector_int& values);


	// ���� ������, ���������� ���������� �������� � ��������� �������.
	// ���� �������, ���������� ��������-����� �� ��� �� ������, �� �� ������� ���������� �������.
	bool get_corresp_col_value_txt(const std::string& table,
		const std::string& find_col, const std::string& target_col,
		const std::string& find_value, std::string& get_value);

	// ���� ������, ���������� ���������� �������� � ��������� �������.
	// �������� �������� �� ���������� �������� � ��� �� ������.
	bool get_corresp_columns_txt_values(const std::string& table,
		const std::string& find_col, const vector_str& target_columns,
		const std::string& find_value, vector_str& values);


	// �������� �������� � ������� ������� � ����������� �������.
	bool set_value_with_clause(const std::string& table,
		const std::string& target_col, const std::string& new_value,
		const std::string& clause_col, const std::string& clause_val);

	// �������� � ������� ����� ������ �� ���������� ��� ���� ��������.
	bool insert_row(const std::string& table, const std::string& values);

	// �������� � ������� ��������� ����� ����� �� ���������� ��� ���� ��������.
	bool insert_rows(const std::string& table, const vector_str& rows);

	// �������� � ������� ����� ������ �� ���������� ��� ��������� ��������.
	bool insert_row_for_columns(const std::string& table, const vector_str& columns, const std::string& values);

	// �������� � ������� ��������� ����� ����� �� ���������� ��� ��������� ��������.
	bool insert_rows_for_columns(const std::string& table, const vector_str& columns, const vector_str& rows);

	// ������� �� ������� ������ � ��������� ������� � �������� � ��.
	bool delete_row(const std::string& table, const std::string& column, const std::string& value);

	// ������� �� ������� ������ � ��������� ������� � �������� � ��.
	bool delete_rows(const std::string& table, const std::string& column, const vector_str& values);


private:
	sqlite3* handle;
	sqlite3_stmt* stmt;

	const char* db_path; // ���� � ����� � ����� ������.

	bool base_opened; // ���� ��������� �������� ���� ������.

	std::mutex data_mutex; // ��� ������ ���� ������ ��� ������������ �������.	
};

using storage_unique = std::unique_ptr<DataStorage>;
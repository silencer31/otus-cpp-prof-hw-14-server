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

	// �������� ����� � ����� ������.
	bool init_database();

	// ��������� ������� � ���� ������.
	bool handle_request(const std::string& request);

	// �������� ���� �����/������.
	bool check_login(const std::string& username, const std::string& password);

private:
	sqlite3* handle;
	sqlite3_stmt* stmt;

	const char* db_path; // ���� � ����� � ����� ������.

	bool base_opened; // ���� ��������� �������� ���� ������.

	std::mutex data_mutex; // ��� ������ ���� ������ ��� ������������ �������.	
};

using storage_shared = std::shared_ptr<DataStorage>;
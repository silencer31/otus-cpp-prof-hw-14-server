#pragma once

#include "data_storage.h"

class RequestManager {
public:
	RequestManager() = delete;

	explicit RequestManager(const char* file_path)
		: data_storage_ptr(std::make_unique<DataStorage>(file_path))
	{}

	~RequestManager() = default;

	// �������� ���� �����/������.
	bool check_password(const int user_id, const std::string& password);

	// �������� user_unique_id �� ������ ������������.
	int get_user_id_by_login(const std::string& username);

	// �������� user_type_number �� user_unique_id.
	int get_user_type_by_user_id(const int user_id);

	// ��������� �������� ���� ������������
	bool get_user_type_name_by_user_id(const int user_id, std::string& type_name);

private: // methods
	// ������ ������ ������������ �� user_unique_id.
	bool get_password_by_user_id(const int user_id, std::string& password);




private: // data
	const storage_unique data_storage_ptr;
};

using request_manager_shared = std::shared_ptr<RequestManager>;
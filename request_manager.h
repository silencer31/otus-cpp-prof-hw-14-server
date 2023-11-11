#pragma once

#include "data_storage.h"

enum class UserType {
	Unknown = 0,
	Administrator = 1,
	Operator = 2,
	User = 3
};

enum class TaskStatus {
	Unknown = 0,
	NotAppointed = 1,
	Appointed = 2,
	InProgress = 3,
	Rejected = 4,
	Completed = 5
};

class RequestManager {
public:
	RequestManager() = delete;

	explicit RequestManager(const char* file_path)
		: data_storage_ptr(std::make_unique<DataStorage>(file_path))
	{}

	~RequestManager() = default;

	void lock_access() {
		data_storage_ptr->lock_db();
	}

	void free_access() {
		data_storage_ptr->lock_db();
	}

// ***** �������� ������ ������, ��������� � �������������� *****

	// ������, ���� �� ����� ����� � ����.
	bool login_present(const std::string& login);

	// �������� ������.
	bool check_password(const int user_id, const std::string& password);

	// ������ ���������� id ������������.
	int get_user_id_max_number();

	// �������� user_unique_id �� ������ ������������. ���������� ������������� ������������ � ����.
	int get_user_id_by_login(const std::string& username);

	// �������� user_type_number �� user_unique_id. �����, ��������������� ���� ������������.
	int get_user_type_by_user_id(const int user_id);

	// ��������� �������� ���� ������������. 
	bool get_user_type_name_by_user_id(const int user_id, std::string& type_name);

	// �������� ��� ������������ �� �����.
	UserType get_user_type_from_int(const int user_type);

	// �������� ��� ������������ �� user_unique_id.
	bool get_fio_by_user_id(const int user_id, vector_str& fio);

	// �������� ������ ���� id �������������.
	int get_user_id_list(vector_int& id_list);

	// �������� ������ ������� ����� ������������ � ���������. 
	int get_type_description_lists(vector_int& numbers, vector_str& descriptions);

// ***** �������� ������ ������, ��������� � �������� *****

	// ������ ���������� id �����.
	int get_task_id_max_number();

	// ������ status_type_number �� task_unique_id. �����, ��������������� ������� ������.
	int get_status_type_by_task_id(const int task_id);

	// �������� ������ ������ �� �����.
	TaskStatus get_status_type_from_int(const int status_type);

	// ��������� �������� ���������/������� ������.
	bool get_status_name_by_task_id(const int task_id, std::string& status_name);

	// �������� �������� ��������������� ����� ��� ������ user_unique_id. ������ ����� ������ ������������. 
	int get_task_ids_by_user_id(const int user_id, vector_int& task_ids);

	// �������� ������ ��������� ������. id ������������ � ������. ���� ����������, ��������, ��������.
	bool get_task_data_by_task_id(const int task_id, vector_int& user_and_status, vector_str& time_name_description);

	// �������� ������ ���� id �����.
	int get_task_id_list(vector_int& id_list);

	// �������� ������ ������� �������� ����� � ���������. 
	int get_status_description_lists(vector_int& numbers, vector_str& descriptions);

// ***** ��������� � ���� ������ *****
	// �������� ������ ������������.
	bool add_user(UserType user_type, const std::string& login, const std::string& password,
		const std::string& sname, const std::string& fname, const std::string& patr);

	// �������� ����� ������.
	bool add_task(const int user_id, const std::string& deadline,
		const std::string& name, const std::string& description);

// ***** ������� �� ���� ������ *****
	// ������� ������������.
	bool del_user(const int user_id);

	// ������� ������.
	bool del_task(const int task_id);

// ***** �������� �������� � ���� *****
	// ���������� ����� ������.
	bool set_password(const int user_id, const std::string& password);

	// �������� ��� ������������.
	bool set_user_type(const int user_id, UserType user_type);

	// �������� ������ ������.
	bool set_task_status(const int task_id, TaskStatus task_status);

private: // methods
	// ������ ������ ������������ �� user_unique_id.
	bool get_password_by_user_id(const int user_id, std::string& password);

private: // data
	const storage_unique data_storage_ptr;
};

using request_manager_shared = std::shared_ptr<RequestManager>;
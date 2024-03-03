#pragma once

#include "data_storage.h"

/**
* @brief ������������ - ��������� ���� �������������.
*/
enum class UserType {
	Unknown = 0,
	Administrator = 1,
	Operator = 2,
	User = 3
};

/**
* @brief ������������ - ��������� ������� �����.
*/
enum class TaskStatus {
	Unknown = 0,
	NotAppointed = 1,
	Appointed = 2,
	InProgress = 3,
	Rejected = 4,
	Completed = 5
};

/**
* @brief ����� - �������� �������� � ���� ������.
*/
class RequestManager {
public:
	RequestManager() = delete;

	explicit RequestManager(const char* file_path)
		: data_storage_ptr(std::make_unique<DataStorage>(file_path))
	{}

	~RequestManager() = default;

	/**
	* �������� �������� �������� � ���� ������.
	*/
	void lock_access() {
		data_storage_ptr->lock_db();
	}

	/**
	* ���������� �������� �������� � ���� ������.
	*/
	void free_access() {
		data_storage_ptr->unlock_db();
	}

// ***** �������� ������ ������, ��������� � �������������� *****

	/**
	* ������, ���� �� ����� ����� � ����.
	* @param login ����� ������������.
	*/
	bool login_present(const std::string& login);
 
	/**
	* �������� ������.
	* @param user_id ���������� id ������������.
	* @param password ������ ��� ��������.
	*/
	bool check_password(const int user_id, const std::string& password);

	/**
	* ������ ����� - ���������� id ������������ � ����.
	*/
	int get_user_id_max_number();

	/**
	* �������� ���������� id ������������ �� ������ ������������.
	* @param username ����� ������������.
	*/
	int get_user_id_by_login(const std::string& username);

	/**
	* �������� �����, ��������������� ���� ������������ �� id ������������.
	* @param user_id ���������� id ������������.
	*/
	int get_user_type_by_user_id(const int user_id);

	/**
	* ������ ����� �� id ������������.
	* @param user_id ���������� id ������������.
	* @param username �������� ��� ������ �������� - ������.
	*/
	bool get_login_by_user_id(const int user_id, std::string& username);

	/**
	* �������� ��������� �������� ���� ������������ �� id ������������. 
	* @param user_id ���������� id ������������.
	* @param type_name �������� ��� ������ �������� - �������� ���� ������������.
	*/
	bool get_user_type_name_by_user_id(const int user_id, std::string& type_name);

	/**
	* �������� ��� ������������ �� �����.
	* @param user_type ����� - ����� ���� ������������.
	*/
	UserType get_user_type_from_int(const int user_type);

	/**
	* �������� ��� ������������ �� user_unique_id.
	* @param user_id ���������� id ������������.
	* @param fio �������� ��� ������ �������� - ��� ������������.
	*/
	bool get_fio_by_user_id(const int user_id, vector_str& fio);

	/**
	* �������� ������ ���� id �������������.
	* @param id_list �������� ��� ������ �������� - ���� id �������������.
	*/
	int get_user_id_list(vector_int& id_list);

	/**
	* �������� ������ ������� ����� ������������ � ������ � ��������� ���������.
	* @param numbers �������� ��� ������ �������� - ������� ����� ������������.
	* @param descriptions �������� ��� ������ �������� - ������ � ��������� ���������.
	*/
	int get_type_description_lists(vector_int& numbers, vector_str& descriptions);

// ***** �������� ������ ������, ��������� � �������� *****
 
	/**
	* ������ ����� - ���������� id ����� � ����.
	*/
	int get_task_id_max_number();

	/**
	* ������ �����, ��������������� ������� ������ �� id ������.
	* @param task_id ���������� id ������.
	*/
	int get_status_type_by_task_id(const int task_id);

	/**
	* �������� ������ ������ �� �����.
	* @param status_type �����, ��������������� ������� ������.
	*/
	TaskStatus get_status_type_from_int(const int status_type);

	/**
	* �������� ��������� �������� ������� ������.
	* @param task_id ���������� id ������.
	* @param status_name �������� ��� ������ �������� - ���������� �������� ������� ������.
	*/
	bool get_status_name_by_task_id(const int task_id, std::string& status_name);

	/**
	* �������� id ����������� ������ �� id ������.
	* @param task_id ���������� id ������.
	*/
	int get_task_user_id_by_task_id(const int task_id);

	/**
	* �������� ������ �� id �����, ������� ��������� �� ������������ � ���������� user_id.
	* @param user_id ���������� id ������������.
	* @param task_ids �������� ��� ������ �������� - ������ id �����.
	*/
	int get_task_ids_by_user_id(const int user_id, vector_int& task_ids);

	/**
	* �������� ������ ��������� ������. id ������������ � ������. ���� ����������, ��������, ��������.
	* @param task_id ���������� id ������.
	* @param user_and_status �������� ��� ������ �������� - ����������� � �������� �������.
	* @param time_name_description �������� ��� ������ �������� - ���� ����������, �������� � �������� ������.
	*/
	bool get_task_data_by_task_id(const int task_id, vector_int& user_and_status, vector_str& time_name_description);

	/**
	* �������� ������ ���� id �����.
	* @param �������� ��� ������ �������� - ������ id ���� �����.
	*/
	int get_task_id_list(vector_int& id_list);

	/**
	* �������� ������ ������� �������� ����� � ���������.
	* @param numbers �������� ��� ������ �������� - ������ �������� �����.
	* @param descriptions �������� ��� ������ �������� - ��������� �������� �������� �����.
	*/
	int get_status_description_lists(vector_int& numbers, vector_str& descriptions);

// ***** ��������� � ���� ������ *****
 
	/**
	* �������� ������ ������������.
	* @param
	*/
	int add_user(UserType user_type, const std::string& login, const std::string& password,
		const std::string& sname, const std::string& fname, const std::string& patr);

	/**
	* �������� ����� ������.
	* @param user_id ���������� id ������������.
	* @param deadline ���� ���������� ������.
	* @param name �������� ������.
	* @param description �������� ������.
	*/
	int add_task(const int user_id, const std::string& deadline,
		const std::string& name, const std::string& description);

// ***** ������� �� ���� ������ *****

	/**
	* ������� ������������.
	* @param user_id ���������� id ������������.
	*/
	bool del_user(const int user_id);

	/**
	* ������� ������.
	* @param task_id ���������� id ������.
	*/
	bool del_task(const int task_id);

// ***** �������� �������� � ���� *****

	/**
	* ���������� ����� ������.
	* @param user_id ���������� id ������������.
	* @param password ����� ������.
	*/
	bool set_password(const int user_id, const std::string& password);
 
	/**
	* �������� ��� ������������.
	* @param user_id ���������� id ������������.
	* @param user_type ����� ��� ������������.
	*/
	bool set_user_type(const int user_id, UserType user_type);

	/**
	* �������� ������ ������.
	* @param task_id ���������� id ������.
	* @param task_status ����� ������ ������.
	*/
	bool set_task_status(const int task_id, TaskStatus task_status);

	/**
	* ���������� ����������� � ��������� ������ "appointed".
	* @param task_id ���������� id ������.
	* @param user_id ���������� id ������������ - ������ �����������.
	*/
	bool set_task_appointed(const int task_id, const int user_id);

	/**
	* �������� ����������� ������.
	* @param task_id ���������� id ������.
	*/
	bool set_task_not_appointed(const int task_id);

	/**
	* �������� ����������� ������.
	* @param task_id ���������� id ������.
	* @param user_id ���������� id ������������ - ������ �����������.
	*/
	bool set_task_user(const int task_id, const int user_id);

	/**
	* �������� deadline ������.
	* @param task_id ���������� id ������.
	* @param dl_value ����� �������� ����� ��������� ������.
	*/
	bool set_task_deadline(const int task_id, const std::string& dl_value);

private: // methods
 
	/**
	* ������ ������ ������������ �� user_unique_id.
	* @param user_id ���������� id ������������.
	* @param password �������� ��� ��������� �������� ������.
	*/
	bool get_password_by_user_id(const int user_id, std::string& password);

private: // data
	const storage_unique data_storage_ptr;
};

using request_manager_shared = std::shared_ptr<RequestManager>;
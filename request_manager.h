#pragma once

#include "data_storage.h"

class RequestManager {
public:
	RequestManager() = delete;

	explicit RequestManager(const char* file_path)
		: data_storage_ptr(std::make_unique<DataStorage>(file_path))
	{}

	~RequestManager() = default;

	// Проверка пары логин/пароль.
	bool check_password(const int user_id, const std::string& password);

	// Получить user_unique_id по логину пользователя.
	int get_user_id_by_login(const std::string& username);

	// Получить user_type_number по user_unique_id.
	int get_user_type_by_user_id(const int user_id);

	// Текстовое название типа пользователя
	bool get_user_type_name_by_user_id(const int user_id, std::string& type_name);

private: // methods
	// Узнать пароль пользователя по user_unique_id.
	bool get_password_by_user_id(const int user_id, std::string& password);




private: // data
	const storage_unique data_storage_ptr;
};

using request_manager_shared = std::shared_ptr<RequestManager>;
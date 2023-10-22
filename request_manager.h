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
	bool check_login(const std::string& username, const std::string& password, int& user_type_number);

private:
	const storage_unique data_storage_ptr;
};

using request_manager_shared = std::shared_ptr<RequestManager>;
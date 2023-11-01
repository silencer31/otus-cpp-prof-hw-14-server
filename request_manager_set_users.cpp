#include "request_manager.h"

#include <string>

// Установить новый пароль.
bool RequestManager::set_password(const int user_id, const std::string& password)
{
	return data_storage_ptr->set_value_with_clause("Security", "password", "'" + password + "'",
		"user_unique_id", std::to_string(user_id));
}

// Изменить тип пользователя.
bool RequestManager::set_user_type(const int user_id, UserType user_type)
{
	return data_storage_ptr->set_value_with_clause("Security", "user_type_number", std::to_string(static_cast<int>(user_type)),
		"user_unique_id", std::to_string(user_id));
}
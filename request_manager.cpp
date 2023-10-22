#include "request_manager.h"

#include <string>

// Проверка пары логин/пароль.
bool RequestManager::check_login(const std::string& username, const std::string& password, int& user_type_number)
{
    if (username.empty() || password.empty()) {
        return false;
    }

    int num_value;

    // Узнаем user_unique_id в таблице Users по логину пользователя.
    if (!data_storage_ptr->get_corresp_col_value_int("Users",
                                                     "user_unique_login", "user_unique_id",
                                                     username, num_value)) {
        return false;
    }
    
    std::string str_value;

    // Узнаем password в таблице Security по user_unique_id 
    if (!data_storage_ptr->get_corresp_col_value_txt("Security",
                                                     "user_unique_id", "password",
                                                     std::to_string(num_value), str_value)) {
        return false;
    }

    // Сравниваем переданный пароль и пароль в базе.
    if (password != str_value) {
        return false;
    }

    // Узнаем user_type_number в таблице Security по логину user_unique_id.
    if (!data_storage_ptr->get_corresp_col_value_int("Security",
                                                     "user_unique_id", "user_type_number",
                                                     std::to_string(num_value), num_value)) {
        return false;
    }

    user_type_number = num_value;

    return true;
}
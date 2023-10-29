#include "request_manager.h"

// Узнать, есть ли такой логин в базе.
bool RequestManager::check_login(const std::string& login)
{
    return data_storage_ptr->get_txt_value_presence("Users", "user_unique_login", login);
}

// Проверка пары логин/пароль.
bool RequestManager::check_password(const int user_id, const std::string& password)
{
    std::string pass_value;

    if (!get_password_by_user_id(user_id, pass_value)) {
        return false;
    }

    return password == pass_value;
}
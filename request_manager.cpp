#include "request_manager.h"

#include <string>

// Проверка пары логин/пароль.
bool RequestManager::check_password(const int user_id, const std::string& password)
{
    std::string pass_value;

    if (!get_password_by_user_id(user_id, pass_value)) {
        return false;
    }

    return password == pass_value;
}
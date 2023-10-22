#include "request_manager.h"

// Проверка пары логин/пароль.
bool RequestManager::check_login(const std::string& username, const std::string& password)
{
    if (username.empty() || password.empty()) {
        return false;
    }

    int value;


    bool result = data_storage_ptr->get_corresp_col_value_int("Users", "user_unique_login", "user_unique_id", "user_test", value);

    if (result) {
        std::cout << "res " << value << std::endl;
    }
    else {
        std::cout << "err" << std::endl;
    }

    return result;
}
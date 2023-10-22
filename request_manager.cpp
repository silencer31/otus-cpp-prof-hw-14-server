#include "request_manager.h"

#include <string>

// �������� ���� �����/������.
bool RequestManager::check_login(const std::string& username, const std::string& password, int& user_type_number)
{
    if (username.empty() || password.empty()) {
        return false;
    }

    int num_value;

    // ������ user_unique_id � ������� Users �� ������ ������������.
    if (!data_storage_ptr->get_corresp_col_value_int("Users",
                                                     "user_unique_login", "user_unique_id",
                                                     username, num_value)) {
        return false;
    }
    
    std::string str_value;

    // ������ password � ������� Security �� user_unique_id 
    if (!data_storage_ptr->get_corresp_col_value_txt("Security",
                                                     "user_unique_id", "password",
                                                     std::to_string(num_value), str_value)) {
        return false;
    }

    // ���������� ���������� ������ � ������ � ����.
    if (password != str_value) {
        return false;
    }

    // ������ user_type_number � ������� Security �� ������ user_unique_id.
    if (!data_storage_ptr->get_corresp_col_value_int("Security",
                                                     "user_unique_id", "user_type_number",
                                                     std::to_string(num_value), num_value)) {
        return false;
    }

    user_type_number = num_value;

    return true;
}
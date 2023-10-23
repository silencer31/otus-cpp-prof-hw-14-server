#include "request_manager.h"

// �������� user_unique_id �� ������ ������������.
int RequestManager::get_user_id_by_login(const std::string& username)
{
    int num_value;

    // ������ user_unique_id � ������� Users �� ������ ������������.
    if ( data_storage_ptr->get_corresp_col_value_int("Users", "user_unique_login", "user_unique_id", username, num_value)) {
        return num_value;
    }

    return -1;
}

// �������� user_type_number �� user_unique_id.
int RequestManager::get_user_type_by_user_id(const int user_id)
{
    int num_value;

    // ������ user_type_number � ������� Security �� ������ user_unique_id.
    if (data_storage_ptr->get_corresp_col_value_int("Security", "user_unique_id", "user_type_number", std::to_string(user_id), num_value)) {
        return num_value;
    }

    return -1;
}

// ��������� �������� ���� ������������
bool RequestManager::get_user_type_name_by_user_id(const int user_id, std::string& type_name)
{
    // ������ user_type_number �� user_unique_id
    int user_type_value = get_user_type_by_user_id(user_id);

    if (user_type_value < 0) {
        return false;
    }

    // ������ user_type_description � ������� UserTypes �� user_type_number. 
    if (data_storage_ptr->get_corresp_col_value_txt("Security", "user_type_number", "user_type_description", std::to_string(user_type_value), type_name)) {
        return true;
    }

    return false;
}


bool RequestManager::get_password_by_user_id(const int user_id, std::string& password)
{
    // ������ password � ������� Security �� user_unique_id 
    if ( data_storage_ptr->get_corresp_col_value_txt("Security", "user_unique_id", "password", std::to_string(user_id), password)) {
        return true;
    }

    return false;
}
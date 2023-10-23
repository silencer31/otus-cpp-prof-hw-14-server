#include "request_manager.h"

#include <string>

// Получить user_unique_id по логину пользователя.
int RequestManager::get_user_id_by_login(const std::string& username)
{
    int num_value;

    // Узнаем user_unique_id в таблице Users по логину пользователя.
    if ( data_storage_ptr->get_corresp_col_value_int("Users", "user_unique_login", "user_unique_id", username, num_value)) {
        return num_value;
    }

    return -1;
}

// Получить user_type_number по user_unique_id.
int RequestManager::get_user_type_by_user_id(const int user_id)
{
    int num_value;

    // Узнаем user_type_number в таблице Security по логину user_unique_id.
    if (data_storage_ptr->get_corresp_col_value_int("Security", "user_unique_id", "user_type_number", std::to_string(user_id), num_value)) {
        return num_value;
    }

    return -1;
}

// Текстовое название типа пользователя
bool RequestManager::get_user_type_name_by_user_id(const int user_id, std::string& type_name)
{
    // Узнаем user_type_number по user_unique_id
    int user_type_value = get_user_type_by_user_id(user_id);

    if (user_type_value < 0) {
        return false;
    }

    // Узнаем user_type_description в таблице UserTypes по user_type_number. 
    if (data_storage_ptr->get_corresp_col_value_txt("Security", "user_type_number", "user_type_description", std::to_string(user_type_value), type_name)) {
        return true;
    }

    return false;
}

// Получить ФИО пользователя по user_unique_id.
bool RequestManager::get_fio_by_user_id(const int user_id, std::vector<std::string> fio)
{
    std::string str_value;

    // Узнаем second_name (фамилию) в таблице Users по user_unique_id 
    if (!data_storage_ptr->get_corresp_col_value_txt("Users", "user_unique_id", "second_name", std::to_string(user_id), str_value)) {
        return false;
    }

    fio.push_back(str_value);

    // Узнаем first_name (имя) в таблице Users по user_unique_id 
    if (!data_storage_ptr->get_corresp_col_value_txt("Users", "user_unique_id", "first_name", std::to_string(user_id), str_value)) {
        return false;
    }

    fio.push_back(str_value);

    // Узнаем patronymic (отчество) в таблице Users по user_unique_id 
    if (!data_storage_ptr->get_corresp_col_value_txt("Users", "user_unique_id", "patronymic", std::to_string(user_id), str_value)) {
        return false;
    }

    fio.push_back(str_value);

    return true;
}

// Узнать пароль пользователя по user_unique_id.
bool RequestManager::get_password_by_user_id(const int user_id, std::string& password)
{
    // Узнаем password в таблице Security по user_unique_id 
    if ( data_storage_ptr->get_corresp_col_value_txt("Security", "user_unique_id", "password", std::to_string(user_id), password)) {
        return true;
    }

    return false;
}
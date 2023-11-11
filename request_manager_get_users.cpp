#include "request_manager.h"

#include <string>

// Узнать наибольший id пользователя.
int RequestManager::get_user_id_max_number()
{
    int id_value;

    if (data_storage_ptr->get_column_max_int_value("Users", "user_unique_id", id_value)) {
        return id_value;
    }

    return -1;
}

// Получить user_unique_id по логину пользователя.
int RequestManager::get_user_id_by_login(const std::string& username)
{
    int num_value;

    // Узнаем user_unique_id в таблице Users по логину пользователя.
    if ( data_storage_ptr->get_corresp_col_value_int("Users", "user_unique_login", "user_unique_id",
        username, num_value)) {
        return num_value;
    }

    return -1;
}

// Получить user_type_number по user_unique_id.
int RequestManager::get_user_type_by_user_id(const int user_id)
{
    int num_value;

    // Узнаем user_type_number в таблице Security по логину user_unique_id.
    if (data_storage_ptr->get_corresp_col_value_int("Security", "user_unique_id", "user_type_number",
        std::to_string(user_id), num_value)) {
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
    return data_storage_ptr->get_corresp_col_value_txt("Security", "user_type_number", "user_type_description",
        std::to_string(user_type_value), type_name);
}

// Получить тип пользователя из числа.
UserType RequestManager::get_user_type_from_int(const int user_type)
{
    switch (user_type) {
    case 0:
        return UserType::Unknown;
    case 1:
        return UserType::Administrator;
    case 2:
        return UserType::Operator;
    case 3:
        return UserType::User;
    default:
        return UserType::Unknown;
    }
}

// Получить ФИО пользователя по user_unique_id.
bool RequestManager::get_fio_by_user_id(const int user_id, vector_str& fio)
{
    const vector_str target_columns{ "second_name", "first_name", "patronymic" };

    return data_storage_ptr->get_corresp_columns_txt_values("Users", "user_unique_id", target_columns,
        std::to_string(user_id), fio);
}

// Получить список всех id пользователей.
int RequestManager::get_user_id_list(vector_int& id_list)
{
    return data_storage_ptr->get_column_values_int("Users", "user_unique_id", id_list);
}

// Получить список номеров типов пользователя с описанием. 
int RequestManager::get_type_description_lists(vector_int& numbers, vector_str& descriptions)
{
    return data_storage_ptr->get_pairs_int_txt("UserTypes", "user_type_number", "user_type_description",
        numbers, descriptions);
}

// Узнать пароль пользователя по user_unique_id.
bool RequestManager::get_password_by_user_id(const int user_id, std::string& password)
{
    // Узнаем password в таблице Security по user_unique_id 
    return data_storage_ptr->get_corresp_col_value_txt("Security", "user_unique_id", "password",
        std::to_string(user_id), password);
}
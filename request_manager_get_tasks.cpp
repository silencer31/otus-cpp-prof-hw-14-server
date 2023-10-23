#include "request_manager.h"

#include <string>

// ������ task_unique_id �� user_unique_id.
int RequestManager::get_task_id_by_user_id(const int user_id)
{
    int num_value;

    // ������ task_unique_id � ������� Tasks �� user_unique_id.
    if (data_storage_ptr->get_corresp_col_value_int("Tasks", "user_unique_id", "task_unique_id",std::to_string(user_id), num_value)) {
        return num_value;
    }

    return -1;
}

// ������ status_type_number �� task_unique_id.
int RequestManager::get_status_type_by_task_id(const int task_id)
{
    int num_value;

    // ������ task_unique_id � ������� Tasks �� user_unique_id.
    if (data_storage_ptr->get_corresp_col_value_int("Tasks", "task_unique_id", "status_type_number", std::to_string(task_id), num_value)) {
        return num_value;
    }

    return -1;
}

// ��������� �������� ��������� ������.
bool RequestManager::get_status_name_by_task_id(const int task_id, std::string& status_name)
{
    // ������ status_type_number �� task_unique_id
    int status_type_value = get_status_type_by_task_id(task_id);

    if (status_type_value < 0) {
        return false;
    }

    // ������ status_type_description � ������� TaskStatuses �� status_type_number. 
    if (data_storage_ptr->get_corresp_col_value_txt("TaskStatuses", "status_type_number", "status_type_description", std::to_string(status_type_value), status_name)) {
        return true;
    }

    return false;
}
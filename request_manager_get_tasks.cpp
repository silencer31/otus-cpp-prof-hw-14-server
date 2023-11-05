#include "request_manager.h"

#include <string>

// ������ ���������� id �����.
int RequestManager::get_task_id_max_number()
{
    int id_value;

    if (data_storage_ptr->get_column_max_int_value("Tasks", "task_unique_id", id_value)) {
        return id_value;
    }

    return -1;
}

// ������ status_type_number �� task_unique_id. �����, ��������������� ��������� ������.
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
    return data_storage_ptr->get_corresp_col_value_txt("TaskStatuses", "status_type_number", "status_type_description",
                                                       std::to_string(status_type_value), status_name);
}

// �������� �������� ��������������� ����� ��� ������ user_unique_id. ������ ����� ������ ������������.
bool RequestManager::get_task_ids_by_user_id(const int user_id, vector_int& task_ids)
{
    // ������ ���-�� ��������� - ���������� �������� user_unique_id.
    int items_number = data_storage_ptr->get_occurances_number("Tasks", "user_unique_id", std::to_string(user_id));

    if (items_number == 0) { return false; }

    // ����������� ����� ��� ��������.
    task_ids.reserve(items_number);

    // ��������� ������ ���������� task_unique_id.
    data_storage_ptr->get_corresp_col_int_vector("Tasks", "user_unique_id", "task_unique_id",
                                                 std::to_string(user_id), task_ids);

    return !task_ids.empty();
}

// �������� ������ ��������� ������. ���� ����������, ��������, ��������.
bool RequestManager::get_task_data_by_task_id(const int task_id, vector_str& task_data)
{
    const vector_str target_columns{ "deadline", "task_name", "task_description" };
        
    return data_storage_ptr->get_corresp_columns_txt_values("Tasks", "task_unique_id", target_columns,
                                                            std::to_string(task_id), task_data);
}

// �������� ������ ���� id �����.
int RequestManager::get_task_id_list(vector_int& id_list)
{
    return data_storage_ptr->get_column_values_int("Tasks", "task_unique_id", id_list);
}
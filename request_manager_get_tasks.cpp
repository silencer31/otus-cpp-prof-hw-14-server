#include "request_manager.h"

#include <string>

// ������ ���������� id �����.
int RequestManager::get_task_id_max_number()
{
    int id_value = 0;

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

// �������� ������ ������ �� �����.
TaskStatus RequestManager::get_status_type_from_int(const int status_type)
{
    switch (status_type) {
    case 0:
        return TaskStatus::Unknown;
    case 1:
        return TaskStatus::NotAppointed;
    case 2:
        return TaskStatus::Appointed;
    case 3:
        return TaskStatus::InProgress;
    case 4:
        return TaskStatus::Rejected;
    case 5:
        return TaskStatus::Completed;
    default:
        return TaskStatus::Unknown;
    }
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

// �������� id �������� ������������ ������.
int RequestManager::get_task_user_id_by_task_id(const int task_id)
{
    int num_value;

    // ������ task_unique_id � ������� Tasks �� user_unique_id.
    if (data_storage_ptr->get_corresp_col_value_int("Tasks", "task_unique_id", "user_unique_id", std::to_string(task_id), num_value)) {
        return num_value;
    }

    return -1;
}

// �������� �������� ��������������� ����� ��� ������ user_unique_id. ������ ����� ������ ������������.
int RequestManager::get_task_ids_by_user_id(const int user_id, vector_int& task_ids)
{
    // ������ ���-�� ��������� - ���������� �������� user_unique_id.
    int items_number = data_storage_ptr->get_occurances_number("Tasks", "user_unique_id", std::to_string(user_id));

    if (items_number == 0) { return 0; }

    // ����������� ����� ��� ��������.
    task_ids.reserve(items_number);

    // ��������� ������ ���������� task_unique_id.
    return data_storage_ptr->get_corresp_col_int_vector("Tasks", "user_unique_id", "task_unique_id",
                                                 std::to_string(user_id), task_ids);
}

// �������� ������ ��������� ������. id ������������ � ������. ���� ����������, ��������, ��������.
bool RequestManager::get_task_data_by_task_id(const int task_id, vector_int& user_and_status, vector_str& time_name_description)
{
    const vector_str int_columns{ "user_unique_id", "status_type_number" };

    if (!data_storage_ptr->get_corresp_columns_int_values("Tasks", "task_unique_id", int_columns,
        std::to_string(task_id), user_and_status)) {
        return false;
    }

    const vector_str txt_columns{ "deadline", "task_name", "task_description" };
        
    return data_storage_ptr->get_corresp_columns_txt_values("Tasks", "task_unique_id", txt_columns,
        std::to_string(task_id), time_name_description);
}

// �������� ������ ���� id �����.
int RequestManager::get_task_id_list(vector_int& id_list)
{
    return data_storage_ptr->get_column_values_int("Tasks", "task_unique_id", id_list);
}

// �������� ������ ������� �������� ����� � ���������. 
int RequestManager::get_status_description_lists(vector_int& numbers, vector_str& descriptions)
{
    return data_storage_ptr->get_pairs_int_txt("TaskStatuses", "status_type_number", "status_type_description",
        numbers, descriptions);
}
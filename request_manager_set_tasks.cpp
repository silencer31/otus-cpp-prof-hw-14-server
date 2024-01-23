#include "request_manager.h"

#include <string>

// Изменить статус задачи.
bool RequestManager::set_task_status(const int task_id, TaskStatus task_status)
{
	return data_storage_ptr->set_value_with_clause("Tasks",
			"status_type_number", std::to_string(static_cast<int>(task_status)),
			"task_unique_id", std::to_string(task_id));		 
}

// Установить исполнителя и назначить статус "appointed".
bool RequestManager::set_task_appointed(const int task_id, const int user_id)
{
	vector_str target_columns{ "user_unique_id", "status_type_number" };
	vector_str values{std::to_string(user_id), "2"};

	return data_storage_ptr->set_several_values_with_clause("Tasks", target_columns, values,
		"task_unique_id", std::to_string(task_id));
}

// Сбросить исполнителя задачи.
bool RequestManager::set_task_not_appointed(const int task_id)
{
	vector_str target_columns{"user_unique_id", "status_type_number"};
	vector_str values{"0", "1"};

	return data_storage_ptr->set_several_values_with_clause("Tasks", target_columns, values,
		"task_unique_id", std::to_string(task_id));
}

// Изменить исполнителя задачи.
bool RequestManager::set_task_user(const int task_id, const int user_id)
{
	return data_storage_ptr->set_value_with_clause("Tasks",
			"user_unique_id", std::to_string(user_id),
			"task_unique_id", std::to_string(task_id));
}

// Изменить deadline задачи.
bool RequestManager::set_task_deadline(const int task_id, const std::string& dl_value)
{
	return data_storage_ptr->set_value_with_clause("Tasks",
			"deadline", "'" + dl_value + "'",
			"task_unique_id", std::to_string(task_id));
}


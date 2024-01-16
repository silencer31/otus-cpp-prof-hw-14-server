#include "request_manager.h"

#include <string>

// Изменить статус задачи.
bool RequestManager::set_task_status(const int task_id, TaskStatus task_status)
{
	return data_storage_ptr->set_value_with_clause("Tasks", "status_type_number", std::to_string(static_cast<int>(task_status)),
		"task_unique_id", std::to_string(task_id));
}

// Изменить исполнителя задачи.
bool RequestManager::set_task_user(const int task_id, const int user_id)
{
	return data_storage_ptr->set_value_with_clause("Tasks", "user_unique_id", std::to_string(user_id),
		"task_unique_id", std::to_string(task_id));
}

// Изменить deadline задачи.
bool RequestManager::set_task_deadline(const int task_id, const std::string& deadline)
{
	return data_storage_ptr->set_value_with_clause("Tasks", "deadline", deadline,
		"task_unique_id", std::to_string(task_id));
}


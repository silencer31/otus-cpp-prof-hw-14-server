#include "request_manager.h"

#include <string>

// Изменить статус задачи.
bool RequestManager::set_task_status(const int task_id, TaskStatus task_status)
{
	return data_storage_ptr->set_value_with_clause("Tasks", "status_type_number", std::to_string(static_cast<int>(task_status)),
		"task_unique_id", std::to_string(task_id));
}


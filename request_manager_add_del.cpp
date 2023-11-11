#include "request_manager.h"

#include <string>

// Добавить нового пользователя.
bool RequestManager::add_user(UserType user_type, const std::string& login, const std::string& password,
	const std::string& sname, const std::string& fname, const std::string& patr)
{
	int user_id = get_user_id_max_number();

	user_id = (user_id < 0) ? 1 : (user_id + 1);

	const std::string u_1_val = "'" + login + "'";
	const std::string u_2_val = "'" + fname + "'";
	const std::string u_3_val = "'" + sname + "'";
	const std::string u_4_val = "'" + patr + "'";

	// Значения для столбцов таблицы Users
	const std::string u_values = "(" + std::to_string(user_id) + ","
		+ u_1_val + "," + u_2_val + "," + u_3_val + "," + u_4_val + ")";
		
	// Добавляем новую строку со всеми значениями в Users.
	if (!data_storage_ptr->insert_row("Users", u_values)) {
		return false;
	}

	// Значения для столбцов таблицы Security
	const std::string s_values = "(" + std::to_string(user_id) + "," + std::to_string(static_cast<int>(user_type)) + ","
		+ "'" + password + "'" +  ")";

	// Добавляем новую строку со всеми значениями в Security.
	return data_storage_ptr->insert_row("Security", s_values);
}

// Добавить новую задачу.
bool RequestManager::add_task(const int user_id, const std::string& deadline,
	const std::string& name, const std::string& description)
{
	int task_id = get_task_id_max_number();

	task_id = (task_id < 0) ? 1 : (task_id + 1);

	const std::string t_4_val = "'" + deadline + "'";
	const std::string t_5_val = "'" + name + "'";
	const std::string t_6_val = "'" + description + "'";

	// Значения для столбцов таблицы Security
	const std::string t_values = "(" + std::to_string(task_id) + "," + std::to_string(user_id) + ",2,"
		+ t_4_val + "," + t_5_val + "," + t_6_val + ")";

	// Добавляем новую строку со всеми значениями в Security.
	return data_storage_ptr->insert_row("Tasks", t_values);
}

// Удалить пользователя.
bool RequestManager::del_user(const int user_id)
{
	if (!data_storage_ptr->delete_row("Users", "user_unique_id", std::to_string(user_id))) {
		return false;
	}

	return data_storage_ptr->delete_row("Security", "user_unique_id", std::to_string(user_id));
}

// Удалить задачу.
bool RequestManager::del_task(const int task_id)
{
	return data_storage_ptr->delete_row("Taks", "task_unique_id", std::to_string(task_id));
}
#include "task_server.h"

// Получить ФИО пользователя по user id.
void ClientSession::get_fullname()
{
	if (!client_request.contains("user_id")) {
		server_reply["parameter"] = "user_id";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request["user_id"].is_number_integer()) {
		server_reply["parameter"] = "user_id";
		reply_error(RequestError::BadValue);
		return;
	}
		
	int user_id = static_cast<int>(client_request["user_id"]);
	
	request_manager_ptr->lock_access(); // Пытаемся получить доступ к базе.

	// Проверяем, есть ли в базе пользователь с таким id.
	if (request_manager_ptr->get_user_type_by_user_id(user_id) < 0) {
		request_manager_ptr->free_access(); // Освобождаем доступ к базе.
		server_reply["parameter"] = "user_id";
		server_reply["details"] = "Provided user id is not found in data base";
		reply_error(RequestError::BadValue);
		return;
	}

	vector_str fio;
	fio.reserve(3);

	
	bool result = request_manager_ptr->get_fio_by_user_id(user_id, fio);
	request_manager_ptr->free_access(); // Освобождаем доступ к базе.

	server_reply["result"] = result;

	if (result) {
		server_reply["second"] = fio.at(0);
		server_reply["first"] = fio.at(1);
		server_reply["patronymic"] = fio.at(2);
	}

	reply_request(CommandType::Get);
}

// Получить список всех id пользователей.
void ClientSession::get_userlist()
{
	// Только администратор может запросить список id всех пользователей.
	if (logged_user_type != UserType::Administrator) {
		server_reply["details"] = "Insufficient access level";
		reply_error(RequestError::NoPermission);
		return;
	}

	vector_int id_list;
	id_list.reserve(20);

	request_manager_ptr->lock_access(); // Пытаемся получить доступ к базе.
	int size = request_manager_ptr->get_user_id_list(id_list);
	request_manager_ptr->free_access(); // Освобождаем доступ к базе.

	server_reply["result"] = (size > 0);

	if (size == 0) {
		reply_request(CommandType::Get);
		return;
	}
	
	id_list.shrink_to_fit();
	
	server_reply["id_list"] = id_list;

	reply_request(CommandType::Get);
}

// Если указан user_id, получить список его задач, а если не указан, то список всех задач. 
void ClientSession::get_tasklist()
{
	int user_id = -1;

	// Если передан id пользователя, возвращаем список задач только для него.
	if (client_request.contains("user_id")) {
		if (client_request["user_id"].is_number_integer()) {
			user_id = static_cast<int>(client_request["user_id"]);
		}
		else {
			server_reply["parameter"] = "user_id";
			reply_error(RequestError::BadValue);
			return;
		}
	}

	request_manager_ptr->lock_access(); // Пытаемся получить доступ к базе.

	// Проверяем, есть ли в базе пользователь с таким id.
	if ((user_id > 0) && (request_manager_ptr->get_user_type_by_user_id(user_id) < 0)) {
		request_manager_ptr->free_access(); // Освобождаем доступ к базе.
		server_reply["parameter"] = "user_id";
		server_reply["details"] = "Provided user id is not found in data base";
		reply_error(RequestError::BadValue);
		return;
	}

	vector_int id_list;
	id_list.reserve(30);

	int size = ((user_id >= 0)
		? request_manager_ptr->get_task_id_list(id_list)
		: request_manager_ptr->get_task_ids_by_user_id(user_id, id_list));

	request_manager_ptr->free_access(); // Освобождаем доступ к базе.

	server_reply["result"] = (size > 0);

	if (size == 0) {
		reply_request(CommandType::Get);
		return;
	}
	
	id_list.shrink_to_fit();
	
	server_reply["id_list"] = id_list;

	reply_request(CommandType::Get);
}

// Получить список возможных типов пользователя и список с их описанием. 
void ClientSession::get_typelist()
{
	vector_int type_list;
	vector_str description_list;

	type_list.reserve(3);
	description_list.reserve(3);

	request_manager_ptr->lock_access(); // Пытаемся получить доступ к базе.
	int size = request_manager_ptr->get_type_description_lists(type_list, description_list);
	request_manager_ptr->free_access(); // Освобождаем доступ к базе.

	server_reply["result"] = (size > 0);

	if (size == 0) {
		reply_request(CommandType::Get);
		return;
	}

	type_list.shrink_to_fit();
	description_list.shrink_to_fit();

	server_reply["numbers"] = type_list;
	server_reply["description"] = description_list;

	reply_request(CommandType::Get);
}

// Получить список возможных статусов задачи и список с их описанием.
void ClientSession::get_statuslist()
{
	vector_int status_list;
	vector_str description_list;

	status_list.reserve(5);
	description_list.reserve(5);

	request_manager_ptr->lock_access(); // Пытаемся получить доступ к базе.
	int size = request_manager_ptr->get_status_description_lists(status_list, description_list);
	request_manager_ptr->free_access(); // Освобождаем доступ к базе.

	server_reply["result"] = (size > 0);

	if (size == 0) {
		reply_request(CommandType::Get);
		return;
	}

	status_list.shrink_to_fit();
	description_list.shrink_to_fit();

	server_reply["numbers"] = status_list;
	server_reply["description"] = description_list;

	reply_request(CommandType::Get);
}

// Получить данные задачи по task id.
void ClientSession::get_taskdata()
{
	if (!client_request.contains("task_id")) {
		server_reply["parameter"] = "task_id";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request["task_id"].is_number_integer()) {
		server_reply["parameter"] = "task_id";
		reply_error(RequestError::BadValue);
		return;
	}

	int task_id = static_cast<int>(client_request["task_id"]);

	vector_int user_and_status;
	vector_str time_name_description;

	user_and_status.reserve(2);
	time_name_description.reserve(3);

	request_manager_ptr->lock_access(); // Пытаемся получить доступ к базе.
	bool result = request_manager_ptr->get_task_data_by_task_id(task_id, user_and_status, time_name_description);
	request_manager_ptr->free_access(); // Освобождаем доступ к базе.

	server_reply["result"] = result;

	if (!result) {
		server_reply["details"] = "An error occured while getting task data";
		reply_request(CommandType::Get);
		return;
	}

	server_reply["user_id"] = user_and_status.at(0);
	server_reply["status"] = user_and_status.at(1);

	server_reply["deadline"] = time_name_description.at(0);
	server_reply["name"] = time_name_description.at(1);
	server_reply["description"] = time_name_description.at(2);

	reply_request(CommandType::Get);
}
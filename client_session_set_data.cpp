#include "task_server.h"

void ClientSession::set_password()
{
	if (!client_request.contains("user_id")) {
		server_reply["parameter"] = "user_id";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request["user_id"].is_number_integer()) {
		server_reply["parameter"] = "user_id";
		server_reply["details"] = "user id value is not integer";
		reply_error(RequestError::BadValue);
		return;
	}

	// Проверка наличия в запросе поля password.
	if (!client_request.contains("password")) {
		server_reply["parameter"] = "password";
		reply_error(RequestError::NoParameter);
		return;
	}

	int user_id = static_cast<int>(client_request["user_id"]);

	// Администратор может менять пароль любого пользователя, а остальные только свой пароль.
	if ((user_id != logged_user_id) && (logged_user_type != UserType::Administrator)) {
		reply_error(RequestError::NoPermission);
		return;
	}

	request_manager_ptr->lock_access();
	bool result = request_manager_ptr->set_password(user_id, client_request["password"]);
	request_manager_ptr->free_access();

	server_reply["result"] = result;

	server_reply["details"] = (result ? "Password successfully changed" : "An error occurred while setting new password");

	reply_request(CommandType::Set);
}

void ClientSession::set_usertype()
{
	if (!client_request.contains("user_id")) {
		server_reply["parameter"] = "user_id";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request["user_id"].is_number_integer()) {
		server_reply["parameter"] = "user_id";
		server_reply["details"] = "user id value is not integer";
		reply_error(RequestError::BadValue);
		return;
	}

	if (!client_request.contains("user_type")) {
		server_reply["parameter"] = "user_type";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request["user_type"].is_number_integer()) {
		server_reply["parameter"] = "user_type";
		server_reply["details"] = "user type value is not integer";
		reply_error(RequestError::BadValue);
		return;
	}

	// Только администратор может менять тип пользователя.
	if (logged_user_type != UserType::Administrator) {
		reply_error(RequestError::NoPermission);
		return;
	}

	UserType user_type = request_manager_ptr->get_user_type_from_int(static_cast<int>(client_request["user_type"]));

	if (user_type == UserType::Unknown) {
		server_reply["parameter"] = "user_type";
		server_reply["details"] = "Unable to get UserType for provided user_id";
		reply_error(RequestError::BadValue);
		return;
	}

	int user_id = static_cast<int>(client_request["user_id"]);

	// Проверяем совпадение с текущим типом у целевого пользователя.
	int cur_utype_val = request_manager_ptr->get_user_type_by_user_id(user_id);
	if (user_type == request_manager_ptr->get_user_type_from_int(cur_utype_val)) {
		server_reply["details"] = "User already has the same type";
		reply_error(RequestError::NoPermission);
		return;
	}

	request_manager_ptr->lock_access();
	bool result = request_manager_ptr->set_user_type(user_id, user_type);
	request_manager_ptr->free_access();

	if (result && (user_id == logged_user_id)) {
		logged_user_type = user_type; // Администратор понизил свой тип пользователя.
	}

	server_reply["result"] = result;

	server_reply["details"] = (result ? "User type successfully changed" : "An error occurred while setting new user type");

	reply_request(CommandType::Set);
}

void ClientSession::set_taskstatus()
{
	if (!client_request.contains("task_id")) {
		server_reply["parameter"] = "task_id";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request["task_id"].is_number_integer()) {
		server_reply["parameter"] = "task_id";
		server_reply["details"] = "task id value is not integer";
		reply_error(RequestError::BadValue);
		return;
	}

	if (!client_request.contains("status")) {
		server_reply["parameter"] = "status";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request["status"].is_number_integer()) {
		server_reply["parameter"] = "status";
		server_reply["details"] = "status value is not integer";
		reply_error(RequestError::BadValue);
		return;
	}

	TaskStatus task_status = request_manager_ptr->get_status_type_from_int(static_cast<int>(client_request["status"]));

	if (task_status == TaskStatus::Unknown) {
		server_reply["parameter"] = "status";
		server_reply["details"] = "Unable to get TaskStatus for provided task_id";
		reply_error(RequestError::BadValue);
		return;
	}

	int task_id = static_cast<int>(client_request["task_id"]);

	int cur_ts_value = request_manager_ptr->get_status_type_by_task_id(task_id);
	if (task_status == request_manager_ptr->get_status_type_from_int(cur_ts_value)) {
		server_reply["details"] = "Task already has the same status";
		reply_error(RequestError::NoPermission);
		return;
	}
	
	request_manager_ptr->lock_access();
	bool result = request_manager_ptr->set_task_status(task_id, task_status);
	request_manager_ptr->free_access();

	server_reply["result"] = result;

	server_reply["details"] = (result ? "Task status successfully changed" : "An error occurred while setting new task status");

	reply_request(CommandType::Set);
}

void ClientSession::set_taskdata()
{


	reply_request(CommandType::Set);
}
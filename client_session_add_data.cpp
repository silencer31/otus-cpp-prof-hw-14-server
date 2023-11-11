#include "task_server.h"

void ClientSession::add_user()
{
	if (!client_request.contains("username")) {
		server_reply["parameter"] = "username";
		reply_error(RequestError::NoParameter);
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

	if (!client_request.contains("second")) {
		server_reply["parameter"] = "second";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request.contains("first")) {
		server_reply["parameter"] = "first";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request.contains("patronymic")) {
		server_reply["parameter"] = "patronymic";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request.contains("password")) {
		server_reply["parameter"] = "password";
		reply_error(RequestError::NoParameter);
		return;
	}

	// Только администратор может добавлять нового пользователя.
	if (logged_user_type != UserType::Administrator) {
		reply_error(RequestError::NoPermission);
		return;
	}

	// Проверяем, есть ли уже такой логин в базе.
	if (login_present(client_request["username"])) {
		server_reply["parameter"] = "username";
		server_reply["details"] = "Username is already in use";
		reply_error(RequestError::BadValue);
		return;
	}

	UserType user_type = request_manager_ptr->get_user_type_from_int(static_cast<int>(client_request["user_type"]));

	if (user_type == UserType::Unknown) {
		server_reply["parameter"] = "user_type";
		server_reply["details"] = "Unable to convert provided value to UserType";
		reply_error(RequestError::BadValue);
		return;
	}

	request_manager_ptr->lock_access();
	bool result = request_manager_ptr->add_user(user_type, client_request["username"], client_request["password"],
		client_request["second"], client_request["first"], client_request["patronymic"]);
	request_manager_ptr->free_access();

	server_reply["result"] = result;

	server_reply["details"] = (result ? "User successfully added" : "An error occurred while adding new task");

	reply_request(CommandType::Add);
}

void ClientSession::add_task()
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

	if (!client_request.contains("deadline")) {
		server_reply["parameter"] = "deadline";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request.contains("name")) {
		server_reply["parameter"] = "name";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request.contains("description")) {
		server_reply["parameter"] = "description";
		reply_error(RequestError::NoParameter);
		return;
	}

	int user_id = static_cast<int>(client_request["user_id"]);

	request_manager_ptr->lock_access();
	bool result = request_manager_ptr->add_task(user_id, client_request["deadline"], client_request["name"], client_request["description"]);
	request_manager_ptr->free_access();

	server_reply["result"] = result;

	server_reply["details"] = (result ? "Task successfully added" : "An error occurred while adding new task");

	reply_request(CommandType::Add);
}
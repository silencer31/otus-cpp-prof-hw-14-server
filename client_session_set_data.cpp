#include "task_server.h"

// ���������� ����� ������ � ������������.
void ClientSession::set_password()
{
	// �������� ������� � ������� ���� user_id.
	if (!client_request.contains("user_id")) {
		server_reply["parameter"] = "user_id";
		reply_error(RequestError::NoParameter);
		return;
	}

	// ���� user_id ������ ���� ������.
	if (!client_request["user_id"].is_number_integer()) {
		server_reply["parameter"] = "user_id";
		server_reply["details"] = "user id value is not integer";
		reply_error(RequestError::BadValue);
		return;
	}

	// �������� ������� � ������� ���� password.
	if (!client_request.contains("password")) {
		server_reply["parameter"] = "password";
		reply_error(RequestError::NoParameter);
		return;
	}

	int user_id = static_cast<int>(client_request["user_id"]);

	// ������������� ����� ������ ������ ������ ������������, � ��������� ������ ���� ������.
	if ((user_id != logged_user_id) && (logged_user_type != UserType::Administrator)) {
		server_reply["details"] = "You are not allowed to change user password";
		reply_error(RequestError::NoPermission);
		return;
	}

	request_manager_ptr->lock_access(); // �������� �������� ������ � ����.

	// ���������, ���� �� � ���� ������������ � ����� id.
	if (request_manager_ptr->get_user_type_by_user_id(user_id) < 0) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "user_id";
		server_reply["details"] = "Provided user id is not found in data base";
		reply_error(RequestError::BadValue);
		return;
	}

	bool result = request_manager_ptr->set_password(user_id, client_request["password"]);
	request_manager_ptr->free_access(); // ����������� ������ � ����.

	server_reply["result"] = result;

	if (!result) {
		server_reply["details"] = "An error occurred while setting new password";
	}

	reply_request(CommandType::Set);
}

// �������� ��� ������������ � ����.
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

	// ������ ������������� ����� ������ ��� ������������.
	if (logged_user_type != UserType::Administrator) {
		server_reply["details"] = "You are not allowed to change user type";
		reply_error(RequestError::NoPermission);
		return;
	}

	int user_id = static_cast<int>(client_request["user_id"]);
	int new_user_type_val = static_cast<int>(client_request["user_type"]);

	request_manager_ptr->lock_access(); // �������� �������� ������ � ����.

	// ���������, ���� �� � ���� ������������ � ����� id.
	if (request_manager_ptr->get_user_type_by_user_id(user_id) < 0) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "user_id";
		server_reply["details"] = "Provided user id is not found in data base";
		reply_error(RequestError::BadValue);
		return;
	}

	// ��������� ���������� � ������� ����� � �������� ������������.
	if (new_user_type_val == request_manager_ptr->get_user_type_by_user_id(user_id)) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["details"] = "User already has the same type";
		reply_error(RequestError::NoPermission);
		return;
	}

	UserType user_type = request_manager_ptr->get_user_type_from_int(new_user_type_val);

	if (user_type == UserType::Unknown) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "user_type";
		server_reply["details"] = "Unable to get user type for provided user id";
		reply_error(RequestError::BadValue);
		return;
	}
	
	bool result = request_manager_ptr->set_user_type(user_id, user_type);
	request_manager_ptr->free_access(); // ����������� ������ � ����.

	if (result && (user_id == logged_user_id)) {
		logged_user_type = user_type; // ������������� ������� ���� ��� ������������.
	}

	server_reply["result"] = result;

	if (!result) {
		server_reply["details"] = "An error occurred while setting new user type";
	}

	reply_request(CommandType::Set);
}

// ���������� ����� ������ ������ �� task id.
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
	
	int task_id = static_cast<int>(client_request["task_id"]);
	int new_task_status_val = static_cast<int>(client_request["status"]);

	request_manager_ptr->lock_access(); // �������� �������� ������ � ����.

	// ������� ������ ��������� ������.
	int cur_task_status_val = request_manager_ptr->get_status_type_by_task_id(task_id);

	// ���������, ��� �� ������� ��������� id ������.
	if (cur_task_status_val < 0) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "task_id";
		server_reply["details"] = "Provided task id is not found in data base";
		reply_error(RequestError::BadValue);
		return;
	}

	// ��������� ���������� ������ ������� ������ � ��������.
	if (new_task_status_val == cur_task_status_val) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["details"] = "Task already has the same status";
		reply_error(RequestError::NoPermission);
		return;
	}
		
	TaskStatus new_task_status = request_manager_ptr->get_status_type_from_int(new_task_status_val);
	
	// ��������� ������������ �������� ������ ������� ������.
	if (new_task_status == TaskStatus::Unknown) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "status";
		server_reply["details"] = "Provided task status value is not correct";
		reply_error(RequestError::BadValue);
		return;
	}
	
	bool result = request_manager_ptr->set_task_status(task_id, new_task_status);
	request_manager_ptr->free_access();

	server_reply["result"] = result;

	if (!result) {
		server_reply["details"] = "An error occurred while setting new status for task";
	}

	reply_request(CommandType::Set);
}

// ��������� �� ������ ������� ������������.
void ClientSession::set_taskuser()
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

	int task_id = static_cast<int>(client_request["task_id"]);
	int new_task_user_id = static_cast<int>(client_request["user_id"]);

	request_manager_ptr->lock_access(); // �������� �������� ������ � ����.

	// ���������, ��� �� ������� ��������� id ������.
	if (request_manager_ptr->get_status_type_by_task_id(task_id) < 0) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "task_id";
		server_reply["details"] = "Provided task id is not found in data base";
		reply_error(RequestError::BadValue);
		return;
	}

	// �������, ��� �� ��� �������� ������������ �� ������ ������.
	int cur_task_user_id = request_manager_ptr->get_task_user_id_by_task_id(task_id);

	// ���� ��� ��������, ��������� ���������� � ����� �������������.
	if ((cur_task_user_id > 0) && (new_task_user_id)) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["details"] = "The same user has alredy been appointed";
		reply_error(RequestError::NoPermission);
		return;
	}
	
	// ���������, ��� �� ������� ��������� ����� id ������������.
	if (request_manager_ptr->get_user_type_by_user_id(new_task_user_id) < 0) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "user_id";
		server_reply["details"] = "Provided user id is not found in data base";
		reply_error(RequestError::BadValue);
		return;
	}

	bool result = request_manager_ptr->set_task_user(task_id, new_task_user_id);
	
	request_manager_ptr->free_access(); // ����������� ������ � ����.

	server_reply["result"] = result;

	if (!result) {
		server_reply["details"] = "An error occurred while setting new task user";
	}

	reply_request(CommandType::Set);
}

// ���������� ����� �������� deadline � ������
void ClientSession::set_taskdeadline()
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

	const int task_id = static_cast<int>(client_request["task_id"]);

	request_manager_ptr->lock_access(); // �������� �������� ������ � ����.

	// ���������, ��� �� ������� ��������� id ������.
	if (request_manager_ptr->get_status_type_by_task_id(task_id) < 0) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "task_id";
		server_reply["details"] = "Provided task id is not found in data base";
		reply_error(RequestError::BadValue);
		return;
	}

	bool result = request_manager_ptr->set_task_deadline(task_id, client_request["deadline"]);

	request_manager_ptr->free_access(); // ����������� ������ � ����.

	server_reply["result"] = result;

	if (!result) {
		server_reply["details"] = "An error occurred while setting new deadline";
	}

	reply_request(CommandType::Set);
}
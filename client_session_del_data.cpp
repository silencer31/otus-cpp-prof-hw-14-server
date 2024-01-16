#include "task_server.h"

// ������� ������������ �� ����.
void ClientSession::del_user()
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

	// ������ ������������� ����� ������� ������������.
	if (logged_user_type != UserType::Administrator) {
		server_reply["details"] = "Insufficient access level";
		reply_error(RequestError::NoPermission);
		return;
	}

	request_manager_ptr->lock_access(); // �������� �������� ������ � ����.

	int user_id = static_cast<int>(client_request["user_id"]);

	// ������ ������� ������ ����.
	if (logged_user_id == user_id) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "user_id";
		server_reply["details"] = "Self deletion is prohibited";
		reply_error(RequestError::NoPermission);
		return;
	}

	// ���������, ���� �� � ���� ������������ � ����� id.
	if (request_manager_ptr->get_user_type_by_user_id(user_id) < 0) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "user_id";
		server_reply["details"] = "Provided user id is not found in data base";
		reply_error(RequestError::BadValue);
		return;
	}
	
	// ������� ������ �����, ������� �������� ������ ������������.
	vector_int ids;
	[[maybe_unused]] const int number = request_manager_ptr->get_task_ids_by_user_id(user_id, ids);
	
	bool result = false;

	// ������ ������ ������ ������ "Not appointed" � ��������� user_id = 0.
	for (auto iter = ids.cbegin(); iter != ids.cend(); ++iter) {
		result = request_manager_ptr->set_task_status(*iter, TaskStatus::NotAppointed);

		if (!result) {
			server_reply["details"] = "Error while changing task status to not appointed";
			break;
		}

		result = request_manager_ptr->set_task_user(*iter, 0);

		if (!result) {
			server_reply["details"] = "Error while changing changing user id for task";
			break;
		}
	}

	if (!result) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["result"] = result;
		reply_request(CommandType::Del);
		return;
	}

	// ������� ������������ �� ����.
	result = request_manager_ptr->del_user(user_id);

	request_manager_ptr->free_access(); // ����������� ������ � ����.
	
	if (!result) {
		server_reply["details"] = "An error occurred while deletion of the user";
	}

	server_reply["result"] = result;

	reply_request(CommandType::Del);
}

// ������� ������ �� ����.
void ClientSession::del_task()
{
	// ������� ������ ����� ������ ������������� ��� �������� ���� ������.
	if (logged_user_type == UserType::User) {
		server_reply["details"] = "Insufficient access level";
		reply_error(RequestError::NoPermission);
		return;
	}

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

	int task_id = static_cast<int>(client_request["task_id"]);

	request_manager_ptr->lock_access(); // �������� �������� ������ � ����.
	
	// ���������, ���� �� � ���� ������ � ����� id.
	if (request_manager_ptr->get_status_type_by_task_id(task_id) < 0) {
		request_manager_ptr->free_access(); // ����������� ������ � ����.
		server_reply["parameter"] = "task_id";
		server_reply["details"] = "Provided task id is not found in data base";
		reply_error(RequestError::BadValue);
		return;
	}

	// ������� ������ �� ����.
	bool result = request_manager_ptr->del_task(task_id);
	
	request_manager_ptr->free_access(); // ����������� ������ � ����.

	server_reply["result"] = result;

	if (!result) {
		server_reply["details"] = "An error occurred while deletion of the task";
	}

	reply_request(CommandType::Del);
}
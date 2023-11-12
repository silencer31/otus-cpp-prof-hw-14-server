//#include "client_session.h"
#include "task_server.h"

#include <iostream>

// ��������� ������� �� �������.
void ClientSession::handle_request()
{
	// ���������� ��� �������/�������.
	CommandType comm_type = task_server_ptr->command_type(client_request["command"]);

	// ��� �������� ������ � ������� ������������ ������ ���� ��������� .
	if ( (comm_type > CommandType::Login) && (logged_user_type != UserType::Unknown)) {
		reply_error(RequestError::NotLogged);
		return;
	}

	// ��� ������� get, add, edit � del ������ ����� ���� type.
	if (comm_type >= CommandType::Get && !client_request.contains("type")) {
		server_reply["parameter"] = "type";
		reply_error(RequestError::NoParameter);
		return;
	}

	switch (comm_type) {
	case CommandType::Unknown:
		reply_error(RequestError::UnknownCommand);
		break;
	case CommandType::Test:
		reply_request(CommandType::Test);
		break;
	case CommandType::Login:
		handle_login();
		break;
	case CommandType::Closedown: // ���������� ������� ������ � ��������.
		handle_closedown();
		break;
	case CommandType::Shutdown:
		handle_shutdown(); // �������� ������� � ������������� ���������� ������.
		break;
	case CommandType::Get:
		handle_get();
		break;
	case CommandType::Add:
		handle_add();
		break;
	case CommandType::Set:
		handle_set();
		break;
	case CommandType::Del:
		handle_del();
		break;
	default:
		break;
	}	
}

// ��������� ������� �� ���������� ������.
void ClientSession::handle_closedown()
{
	shutdown_session_flag = true;
	task_server_ptr->close_session(session_id); // �������� ������� � ���������� ������.

	reply_request(CommandType::Closedown);
}

// ��������� ������� �� ���������� �������.
void ClientSession::handle_shutdown()
{
	// ������ ������������� ����� ��������� ������.
	if (logged_user_type != UserType::Administrator) {
		reply_error(RequestError::NoPermission);
		return;
	}

	shutdown_session_flag = true;
	task_server_ptr->shutdown_server(session_id);

	reply_request(CommandType::Shutdown);
}

// ��������� ������� �������� ���� �����/������.
void ClientSession::handle_login()
{
	// �������� ��� �� ��� ��������� ������������ � ������ ������ ������.
	if (logged_user_type != UserType::Unknown) {
		reply_error(RequestError::AlreadyLogged);
		return;
	}

	// �������� ������� � ������� ���� username.
	if (!client_request.contains("username")) {
		server_reply["parameter"] = "username";
		reply_error(RequestError::NoParameter);
		return;
	}

	// �������� ������� � ������� ���� password.
	if (!client_request.contains("password")) {
		server_reply["parameter"] = "password";
		reply_error(RequestError::NoParameter);
		return;
	}

	// ���������, ���� �� ����� ����� � ����.
	if (!request_manager_ptr->login_present(client_request["username"])) {
		server_reply["details"] = "User is not registered";
		reply_request(CommandType::Login);
		return;
	}

	// ������� �������� user id �� ���� �� ������.
	logged_user_id = request_manager_ptr->get_user_id_by_login(client_request["username"]);

	if (logged_user_id < 0) {
		server_reply["details"] = "Unable to get user id by username";
		reply_request(CommandType::Login);
		return;
	}

	// ��������� ������ ������������.
	if (!request_manager_ptr->check_password(logged_user_id, client_request["password"])) {
		server_reply["details"] = "Password is not correct";
		reply_request(CommandType::Login);
		return;
	}

	// ������� ������ ��� ������������.
	int user_type_number = request_manager_ptr->get_user_type_by_user_id(logged_user_id);

	if (user_type_number < 0) {
		logged_user_id = 0;
		server_reply["details"] = "Unable to get user type by user id";
		reply_request(CommandType::Login);
		return;
	}
	
	// ���� �� ������ �������, ���������� ������������� ������������ ������.
	logged_user_type = request_manager_ptr->get_user_type_from_int(user_type_number);
		
	server_reply["user_id"] = logged_user_id;
	server_reply["user_type"] = user_type_number;
	server_reply["details"] = "Successfully logged in";

	reply_request(CommandType::Login);
}

// ��������� ������� �� ��������� ������.
void ClientSession::handle_get()
{
	// ���������� ����� ������ ����� �� ����.
	GetRequest request_type = task_server_ptr->get_request_type(client_request["type"]);

	switch (request_type) {
	case GetRequest::Unknown:
		server_reply["parameter"] = "type";
		server_reply["details"] = "Unknown get request";
		reply_error(RequestError::BadValue);
		break;
	case GetRequest::Fullname:
		get_fullname();
		break;
	case GetRequest::UserList:
		get_userlist();
		break;
	case GetRequest::TaskList:
		get_tasklist();
		break;
	case GetRequest::StatusList:
		get_statuslist();
		break;
	case GetRequest::TypeList:
		get_typelist();
		break;
	case GetRequest::TaskData:
		get_taskdata();
		break;
	default:
		break;
	}
}

// ��������� ������� �� ���������� ������.
void ClientSession::handle_add()
{
	// ���������� ����� ������ ����� ��������� � ����.
	AddRequest request_type = task_server_ptr->add_request_type(client_request["type"]);

	switch (request_type) {
	case AddRequest::Unknown:
		server_reply["parameter"] = "type";
		server_reply["details"] = "Unknown add request";
		reply_error(RequestError::BadValue);
		break;
	case AddRequest::User:
		add_user();
		break;
	case AddRequest::Task:
		add_task();
		break;
	default:
		break;
	}
}

// ��������� ������� �� ��������� ������.
void ClientSession::handle_set()
{
	// ���������� ����� ������ ����� �������� � ����
	SetRequest request_type = task_server_ptr->edit_request_type(client_request["type"]);

	switch (request_type) {
	case SetRequest::Unknown:
		server_reply["parameter"] = "type";
		server_reply["details"] = "Unknown edit request";
		reply_error(RequestError::BadValue);
		break;
	case SetRequest::Password:
		set_password();
		break;
	case SetRequest::UserType:
		set_usertype();
		break;
	case SetRequest::TaskStatus:
		set_taskstatus();
		break;
	case SetRequest::TaskUser:
		set_taskuser();
		break;
	default:
		break;
	}
}

// ��������� ������� �� �������� ������.
void ClientSession::handle_del()
{
	// ���������� ����� ������ ����� �������� � ����
	DelRequest request_type = task_server_ptr->del_request_type(client_request["type"]);

	switch (request_type) {
	case DelRequest::Unknown:
		server_reply["parameter"] = "type";
		server_reply["details"] = "Unknown del request";
		reply_error(RequestError::BadValue);
		break;
	case DelRequest::User:
		del_user();
		break;
	case DelRequest::Task:
		del_task();
		break;
	default:
		break;
	}
}
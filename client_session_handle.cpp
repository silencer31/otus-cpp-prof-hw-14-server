#include "client_session.h"
#include "task_server.h"

#include <iostream>

// ��������� ������� �� �������.
void ClientSession::handle_request()
{
	server_reply.clear(); // ������� ���������� �����.

	// ���������� ��� �������/�������.
	switch (task_server_ptr->get_command_type(client_request["command"])) {
	case CommandType::Unknown:
		reply_error(RequestError::UnknownCommand);
		break;
	case CommandType::Test:
		reply_request(CommandType::Test);
		break;
	case CommandType::Closedown: // ���������� ������� ������ � ��������.
		handle_closedown();
		break;
	case CommandType::Shutdown:
		handle_shutdown(); // �������� ������� � ������������� ���������� ������.
		break;
	case CommandType::Login:
		handle_login();
		break;
	case CommandType::Get:
		handle_get();
		break;
	case CommandType::Add:
		handle_add();
		break;
	case CommandType::Edit:
		handle_edit();
		break;
	case CommandType::Delete:
		handle_delete();
		break;
	default:
		break;
	}	
}

// ��������� ������� �� ���������� ������.
void ClientSession::handle_closedown()
{
	// ���������, ��� �� ��������� ������������.
	if (!user_logged) {
		server_reply["details"] = "not logged in";
		reply_request(CommandType::Closedown);
		return;
	}

	shutdown_session_flag = true;
	task_server_ptr->close_session(session_id); // �������� ������� � ���������� ������.

	reply_request(CommandType::Closedown);
}

// ��������� ������� �� ���������� �������.
void ClientSession::handle_shutdown()
{
	// ���������, ��� �� ��������� ������������.
	if (!user_logged) {
		server_reply["details"] = "not logged in";
		reply_request(CommandType::Closedown);
		return;
	}

	// ������ ������������� ����� ��������� ������.
	if (logged_user_type != UserType::Administrator) {
		server_reply["details"] = "operation is not permitted";
		reply_request(CommandType::Closedown);
		return;
	}

	shutdown_session_flag = true;
	task_server_ptr->shutdown_server(session_id);

	reply_request(CommandType::Shutdown);
}

// ��������� ������� �������� ���� �����/������.
void ClientSession::handle_login()
{
	// �������� ������� ����������� ���������� � �������.
	if (!client_request.contains("username") || !client_request.contains("password")) {
		reply_error(RequestError::BadParameters);
		return;
	}

	// ���������, ���� �� ����� ����� � ����.
	if (!request_manager_ptr->check_login(client_request["username"])) {
		server_reply["details"] = "username not found";
		reply_request(CommandType::Login);
		return;
	}

	// ������� �������� user id �� ���� �� ������.
	logged_user_id = request_manager_ptr->get_user_id_by_login(client_request["username"]);

	if (logged_user_id < 0) {
		server_reply["details"] = "unable to get user id by username";
		reply_request(CommandType::Login);
		return;
	}

	// ��������� ������ ������������.
	if (!request_manager_ptr->check_password(logged_user_id, client_request["password"])) {
		server_reply["details"] = "password is not correct";
		reply_request(CommandType::Login);
		return;
	}

	// ������� ������ ��� ������������.
	int user_type_number = request_manager_ptr->get_user_type_by_user_id(logged_user_id);

	if (user_type_number < 0) {
		logged_user_id = 0;
		server_reply["details"] = "unable to get user type by user id";
		reply_request(CommandType::Login);
		return;
	}
	
	// ���� �� ������ �������, ���������� ������������� ������������ ������.
	logged_user_type = static_cast<UserType>(user_type_number);
	user_logged = true;

	server_reply["details"] = "successfully logged in";
	server_reply["user_type"] = user_type_number;

	reply_request(CommandType::Login);
}

// ��������� ������� �� ��������� ������.
void ClientSession::handle_get()
{
	// ���������� ����� ������ ����� �� ����
}

// ��������� ������� �� ���������� ������.
void ClientSession::handle_add()
{

}

// ��������� ������� �� ��������� ������.
void ClientSession::handle_edit()
{
	// ���������� ����� ������ ����� �������� � ����
}

// ��������� ������� �� �������� ������.
void ClientSession::handle_delete()
{

}
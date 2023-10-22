#include "client_session.h"
#include "task_server.h"

#include <iostream>

// ��������� ������� �� �������.
void ClientSession::handle_request()
{
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
	case CommandType::GetData:
		handle_getdata();
		break;
	case CommandType::EditData:
		handle_editdata();
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
	shutdown_session_flag = true;
	task_server_ptr->shutdown_server(session_id);

	reply_request(CommandType::Shutdown);
}

// ��������� ������� �������� ���� �����/������.
void ClientSession::handle_login()
{
	if (!client_request.contains("username") || !client_request.contains("password")) {
		reply_error(RequestError::BadParameters);
		return;
	}

	bool res = request_manager_ptr->check_login(client_request["username"], client_request["password"]);

	reply_request(CommandType::Login);
}

// ��������� ������� �� ��������� ������.
void ClientSession::handle_getdata()
{
	// ���������� ����� ������ ����� �� ����
}

// ��������� ������� �� ��������� ������.
void ClientSession::handle_editdata()
{
	// ���������� ����� ������ ����� �������� � ����
}
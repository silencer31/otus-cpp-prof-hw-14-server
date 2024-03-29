//#include "client_session.h"
#include "task_server.h"

// ����� ������� �� ������.
void ClientSession::reply_request(CommandType command_type)
{
	server_reply["error"] = false;

	switch (command_type) {
	case CommandType::Unknown:
		break;
	case CommandType::Test:
		server_reply["command"] = "test";
		server_reply["result"] = !shutdown_session_flag; // ������ �� ��������� �� ����������.
		break;
	case CommandType::Common:
		server_reply["command"] = "common";
		break;
	case CommandType::Login:
		server_reply["command"] = "login";
		server_reply["result"] = (logged_user_type != UserType::Unknown);
		break;
	case CommandType::Closedown:
		server_reply["command"] = "closedown";
		server_reply["result"] = shutdown_session_flag;
		break;
	case CommandType::Shutdown:
		server_reply["command"] = "shutdown";
		server_reply["result"] = shutdown_session_flag;
		break;
	case CommandType::Get:
		server_reply["command"] = "get";
		break;
	case CommandType::Add:
		server_reply["command"] = "add";
		break;
	case CommandType::Set:
		server_reply["command"] = "set";
		break;
	case CommandType::Del:
		server_reply["command"] = "del";
		break;
	default:
		break;
	}

	// ���������� json � ���� ������.
	do_write(server_reply.dump());
}
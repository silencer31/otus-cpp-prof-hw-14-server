#include "client_session.h"
#include "task_server.h"

// Ответ клиенту на запрос.
void ClientSession::reply_request(CommandType command_type)
{
	server_reply["error"] = false;

	switch (command_type) {
	case CommandType::Unknown:
		break;
	case CommandType::Test:
		server_reply["command"] = "test";
		server_reply["result"] = !shutdown_session_flag; // Сессия не находится на завершении.
		break;
	case CommandType::Closedown:
		server_reply["command"] = "closedown";
		server_reply["result"] = shutdown_session_flag;
		break;
	case CommandType::Shutdown:
		server_reply["command"] = "shutdown";
		server_reply["result"] = shutdown_session_flag;
		break;
	case CommandType::Login:
		server_reply["command"] = "login";
		break;
	case CommandType::GetData:
		server_reply["command"] = "getdata";
		break;
	case CommandType::EditData:
		server_reply["command"] = "editdata";
		break;
	default:
		break;
	}

	// Отправляем json в виде строки.
	do_write(server_reply.dump());
}
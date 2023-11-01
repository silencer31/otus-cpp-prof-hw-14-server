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
		server_reply["result"] = user_logged;
		break;
	case CommandType::Get:
		server_reply["command"] = "get";
		break;
	case CommandType::Add:
		server_reply["command"] = "add";
		break;
	case CommandType::Edit:
		server_reply["command"] = "edit";
		break;
	case CommandType::Delete:
		server_reply["command"] = "delete";
		break;
	default:
		break;
	}

	// Отправляем json в виде строки.
	do_write(server_reply.dump());
}
//#include "client_session.h"
#include "task_server.h"

// Сообщить клиенту об ошибке в запросе.
void ClientSession::reply_error(RequestError request_error)
{	
	server_reply["error"] = true;
	
	if (request_error > RequestError::NoCommand) {
		server_reply["command"] = client_request["command"];
	}

	switch (request_error) {
	case RequestError::ParseError:
		server_reply["errtext"] = "parse_error";
		break;
	case RequestError::IsNull:
		server_reply["errtext"] = "is_null";
		break;
	case RequestError::NotObject:
		server_reply["errtext"] = "not_object";
		break;
	case RequestError::NoCommand:
		server_reply["errtext"] = "command_not_found";
		break;
	case RequestError::UnknownCommand:
		server_reply["errtext"] = "unknown_command";
		break;
	case RequestError::NoParameter:
		server_reply["errtext"] = "not_found";
		break;
	case RequestError::BadValue:
		server_reply["errtext"] = "bad_value";
		break;
	case RequestError::NotLogged:
		server_reply["errtext"] = "not_logged";
		break;
	case RequestError::AlreadyLogged:
		server_reply["errtext"] = "already_logged";
		break;
	case RequestError::NoPermission:
		server_reply["errtext"] = "no_permission";
		break;
	default:
		server_reply["errtext"] = "unknown";
		break;
	}

	// Отправляем json в виде строки.
	do_write(server_reply.dump());
}
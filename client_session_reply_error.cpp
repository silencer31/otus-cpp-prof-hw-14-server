//#include "client_session.h"
#include "task_server.h"

// Сообщить клиенту об ошибке в запросе.
void ClientSession::reply_error(RequestError request_error)
{	
	server_reply["error"] = true;
	server_reply["result"] = false;

	if (request_error > RequestError::NoCommand) {
		server_reply["command"] = client_request["command"];
	}

	switch (request_error) {
	case RequestError::ParseError:
		server_reply["errtext"] = "json parse error";
		break;
	case RequestError::IsNull:
		server_reply["errtext"] = "json is null";
		break;
	case RequestError::NotObject:
		server_reply["errtext"] = "json is not object";
		break;
	case RequestError::NoCommand:
		server_reply["errtext"] = "command field not found";
		break;
	case RequestError::UnknownCommand:
		server_reply["errtext"] = "unknown command";
		break;
	case RequestError::NoParameter:
		server_reply["errtext"] = "parameter not found";
		break;
	case RequestError::BadValue:
		server_reply["errtext"] = "parameter bad value";
		break;
	case RequestError::NotLogged:
		server_reply["errtext"] = "user not logged";
		break;
	case RequestError::AlreadyLogged:
		server_reply["errtext"] = "user already logged";
		break;
	case RequestError::NoPermission:
		server_reply["errtext"] = "no permission to perform request";
		break;
	default:
		server_reply["errtext"] = "unknown request error";
		break;
	}

	// Отправляем json в виде строки.
	do_write(server_reply.dump());
}
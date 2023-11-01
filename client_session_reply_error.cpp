#include "client_session.h"
#include "task_server.h"

// Сообщить клиенту об ошибке в запросе.
void ClientSession::reply_error(RequestError request_error)
{
	json reply;
	
	reply["error"] = true;
	
	switch (request_error) {
	case RequestError::ParseError:
		reply["details"] = "json parse error";
		break;
	case RequestError::IsNull:
		reply["details"] = "json data is null";
		break;
	case RequestError::WrongType:
		reply["details"] = "json is not object";
		break;
	case RequestError::NoCommand:
		reply["details"] = "command field not found";
		break;
	case RequestError::UnknownCommand:
		reply["details"] = "unknown command : " + client_request["command"];
		break;
	case RequestError::BadParameters:
		reply["details"] = "bad parameters : " + client_request["command"];
		break;
	default:
		reply["details"] = "unknown error";
		break;
	}

	// Отправляем json в виде строки.
	do_write(reply.dump());
}
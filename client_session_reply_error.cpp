#include "client_session.h"
#include "task_server.h"

// Сообщить клиенту об ошибке в запросе.
void ClientSession::reply_error(RequestError req_error, CommandType command_type)
{
	json reply;
	reply["error"] = true;

	json reply;

	switch (command_type) {
	case CommandType::Unknown:
		reply["command"] = last_command_str;
		break;
	case CommandType::Test:
		reply["command"] = "test";
		break;
	case CommandType::Closedown:
		reply["command"] = "closedown";
		break;
	case CommandType::Shutdown:
		reply["command"] = "shutdown";
		break;
	case CommandType::Login:
		reply["command"] = "login";
		break;
	case CommandType::GetData:
		reply["command"] = "getdata";
		break;
	case CommandType::EditData:
		reply["command"] = "editdata";
		break;
	default:
		reply["command"] = last_command_str;
		break;
	}

	switch (req_error) {
	case RequestError::ParseError:
		reply["description"] = "json parse error";
		break;
	case RequestError::IsNull:
		reply["description"] = "json data is null";
		break;
	case RequestError::WrongType:
		reply["description"] = "json is not object";
		break;
	case RequestError::NoCommand:
		reply["description"] = "unknown command";
		break;
	case RequestError::UnknownCommand:
		reply["description"] = "unknown command";
		break;
	case RequestError::BadParameters:
		reply["description"] = "bad parameters";
		break;
	default:
		reply["description"] = "unknown error";
		break;
	}

	// Отправляем json в виде строки.
	do_write(reply.dump());
}
#include "client_session.h"
#include "task_server.h"

#include <iostream>

// Сообщить клиенту об ошибке в запросе.
void ClientSession::reply_error(RequestError req_error)
{
	json reply;
	reply["reply"] = "error";

	switch (req_error) {
	case RequestError::ParseError:
		reply["info"] = "json parse error";
		break;
	case RequestError::IsNull:
		reply["info"] = "json data is null";
		break;
	case RequestError::UnknownCommand:
		reply["info"] = "unknown command";
		break;
	case RequestError::BadParameters:
		reply["info"] = "bad parameters";
		break;
	default:
		reply["info"] = "unknown error";
		break;
	}

	// Отправляем json в виде строки.
	do_write( reply.dump() );
}

// Ответ клиенту на запрос.
void ClientSession::reply_request(CommandType command_type, bool result)
{
	// Подготовка ответа.
	//json reply = R"( { "reply" : "ack" })"_json;
	// или так
	json reply;
	reply["reply"] = "reply";
	reply["result"] = result;

	switch (command_type) {
	case CommandType::Unknown:
		break;
	case CommandType::Test:
		reply["command"] = "test";
		break;
	case CommandType::Shutdown:
		reply["command"] = "shutdown";
		break;
	case CommandType::Login:
		reply["command"] = "login";
		break;
	case CommandType::GetData:
		break;
	case CommandType::EditData:
		break;
	default:
		break;

	}

	// Отправляем json в виде строки.
	do_write( reply.dump() );
}
#include "client_session.h"

// Сообщить клиенту об ошибке в запросе.
void ClientSession::reply_error(RequestError req_error)
{
	json reply;
	reply["reply"] = "error";

	switch (req_error) {
	case RequestError::ParseError:
		reply["type"] = "json parse error";
		break;
	case RequestError::ParseError:
		reply["type"] = "json data is null";
		break;
	case RequestError::UnknownCommand:
		reply["type"] = "unknown command";
		break;
	case RequestError::BadParameters:
		reply["type"] = "bad parameters";
		break;
	default:
		reply["type"] = "unknown error";
		break;
	}

	// Отправляем json в виде строки.
	do_write( reply.dump() );
}

// Ответ клиенту на запрос.
void ClientSession::reply_request(CommandType req_type)
{
	// Подготовка ответа.
	json reply = R"( { "reply" : "ack" })"_json;
	// или так
	json reply_new;
	reply_new["reply"] = "ack";

	// Отправляем json в виде строки.
	do_write( reply.dump() );
}
#include "client_session.h"
#include "task_server.h"

#include <iostream>

// �������� ������� �� ������ � �������.
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

	// ���������� json � ���� ������.
	do_write( reply.dump() );
}

// ����� ������� �� ������.
void ClientSession::reply_request(CommandType command_type)
{
	// ���������� ������.
	//json reply = R"( { "reply" : "ack" })"_json;
	// ��� ���
	json reply;
	reply["reply"] = "reply";

	switch (command_type) {
	case CommandType::Unknown:
		break;
	case CommandType::Test:
		reply["command"] = "test";
		reply["result"] = "ack";
		break;
	case CommandType::Shutdown:
		reply["command"] = "shutdown";
		reply["result"] = "ack";
		break;
	case CommandType::Login:
		break;
	case CommandType::GetData:
		break;
	case CommandType::EditData:
		break;
	default:
		break;

	}

	// ���������� json � ���� ������.
	do_write( reply.dump() );
}
#include "client_session.h"
#include "task_server.h"

// �������� ������� �� ������ � �������.
void ClientSession::reply_error(RequestError req_error, CommandType command_type)
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
	case RequestError::NoCommand:
		reply["info"] = "unknown command";
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
void ClientSession::reply_request(CommandType command_type, bool result)
{
	// ���������� ������.
	//json reply = R"( { "reply" : "ack" })"_json;
	// ��� ���
	json reply;

	switch (command_type) {
	case CommandType::Unknown:
		break;
	case CommandType::Test:
		reply["command"] = "test";
		reply["result"] = result;
		break;
	case CommandType::Closedown:
		reply["command"] = "closedown";
		reply["result"] = result;
		break;
	case CommandType::Shutdown:
		reply["command"] = "shutdown";
		reply["result"] = result;
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
		break;
	}

	// ���������� json � ���� ������.
	do_write( reply.dump() );
}
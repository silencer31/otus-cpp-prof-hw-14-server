#include "client_session.h"
#include "task_server.h"

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
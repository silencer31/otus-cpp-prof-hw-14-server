#include "client_session.h"

// �������� ������� �� ������ � �������.
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

	// ���������� json � ���� ������.
	do_write( reply.dump() );
}

// ����� ������� �� ������.
void ClientSession::reply_request(CommandType req_type)
{
	// ���������� ������.
	json reply = R"( { "reply" : "ack" })"_json;
	// ��� ���
	json reply_new;
	reply_new["reply"] = "ack";

	// ���������� json � ���� ������.
	do_write( reply.dump() );
}
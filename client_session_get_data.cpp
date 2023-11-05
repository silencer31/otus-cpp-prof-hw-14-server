#include "task_server.h"

void ClientSession::get_fullname()
{
	if (!client_request.contains("user_id")) {
		server_reply["parameter"] = "user_id";
		reply_error(RequestError::NoParameter);
		return;
	}

	if (!client_request["user_id"].is_number_integer()) {
		server_reply["parameter"] = "user_id";
		reply_error(RequestError::BadValue);
		return;
	}

	vector_str fio;
	int user_id = static_cast<int>(client_request["user_id"]);
	
	bool result = request_manager_ptr->get_fio_by_user_id(user_id, fio);
	
	server_reply["result"] = result;

	if (!result) {
		reply_request(CommandType::Get);
		return;
	}

	server_reply["second"] = fio.at(0);
	server_reply["first"] = fio.at(1);
	server_reply["patronymic"] = fio.at(2);

	reply_request(CommandType::Get);
}

void ClientSession::get_userlist()
{
	vector_int id_list;
	id_list.reserve(10);

	int size = request_manager_ptr->get_user_id_list(id_list);

	server_reply["result"] = (size > 0);

	if (size == 0) {
		reply_request(CommandType::Get);
		return;
	}

	id_list.shrink_to_fit();

	json j_values = json::array(id_list);

	reply_request["values"] = j_values;

	reply_request(CommandType::Get);
}

void ClientSession::get_tasklist()
{

	server_reply["result"] = result;

	if (!result) {
		reply_request(CommandType::Get);
		return;
	}

	reply_request(CommandType::Get);
}

void ClientSession::get_statuslist()
{

	server_reply["result"] = result;

	if (!result) {
		reply_request(CommandType::Get);
		return;
	}

	reply_request(CommandType::Get);
}

void ClientSession::get_typelist()
{

	server_reply["result"] = result;

	if (!result) {
		reply_request(CommandType::Get);
		return;
	}

	reply_request(CommandType::Get);
}

void ClientSession::get_taskdata()
{

	server_reply["result"] = result;

	if (!result) {
		reply_request(CommandType::Get);
		return;
	}

	reply_request(CommandType::Get);
}
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
	
	request_manager_ptr->lock_access();
	bool result = request_manager_ptr->get_fio_by_user_id(user_id, fio);
	request_manager_ptr->free_access();

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
	id_list.reserve(20);

	request_manager_ptr->lock_access();
	int size = request_manager_ptr->get_user_id_list(id_list);
	request_manager_ptr->free_access();

	server_reply["result"] = (size > 0);

	if (size == 0) {
		reply_request(CommandType::Get);
		return;
	}

	id_list.shrink_to_fit();

	json j_values = json::array(id_list);

	reply_request["id_list"] = j_values;

	reply_request(CommandType::Get);
}

void ClientSession::get_tasklist()
{
	vector_int id_list;
	id_list.reserve(30);

	request_manager_ptr->lock_access();
	int size = request_manager_ptr->get_task_id_list(id_list);
	request_manager_ptr->free_access();

	server_reply["result"] = (size > 0);

	if (size == 0) {
		reply_request(CommandType::Get);
		return;
	}

	id_list.shrink_to_fit();

	json j_values = json::array(id_list);

	reply_request["id_list"] = j_values;

	reply_request(CommandType::Get);
}

void ClientSession::get_typelist()
{
	vector_int type_list;
	vector_str description_list;

	type_list.reserve(3);
	description_list.reserve(3);

	request_manager_ptr->lock_access();
	int size = request_manager_ptr->get_type_description_lists(type_list, description_list);
	request_manager_ptr->free_access();

	server_reply["result"] = (size > 0);

	if (size == 0) {
		reply_request(CommandType::Get);
		return;
	}

	type_list.shrink_to_fit();
	description_list.shrink_to_fit();

	reply_request["numbers"] = json::array(type_list);
	reply_request["description"] = json::array(description_list);

	reply_request(CommandType::Get);
}

void ClientSession::get_statuslist()
{
	vector_int status_list;
	vector_str description_list;

	status_list.reserve(5);
	description_list.reserve(5);

	request_manager_ptr->lock_access();
	int size = request_manager_ptr->get_status_description_lists(status_list, description_list);
	request_manager_ptr->free_access();

	server_reply["result"] = (size > 0);

	if (size == 0) {
		reply_request(CommandType::Get);
		return;
	}

	status_list.shrink_to_fit();
	description_list.shrink_to_fit();

	reply_request["numbers"] = json::array(status_list);
	reply_request["description"] = json::array(description_list);

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
#pragma once

#include "client_session.h"

#define SESSIONS_MAX_NUMBER 5

using boost::asio::ip::tcp;

using session_map = std::map<int, session_shared>; // ��� ��������� ������ �������������.

using command_map = std::map<std::string, CommandType>; // ��� ��������� �������� ����� �������� �������.

using common_map = std::map<std::string, CommonRequest>; // ��� ��������� �������� �������� ��������� ����� ������.

using get_req_map = std::map<std::string, GetRequest>; // ��� ��������� �������� �������� ��������� ���������� ������.
using add_req_map = std::map<std::string, AddRequest>; // ��� ��������� �������� �������� ���������� ������ � ����.
using set_req_map = std::map<std::string, SetRequest>; // ��� ��������� �������� �������� ��������� ������ � ����.
using del_req_map = std::map<std::string, DelRequest>; // ��� ��������� �������� �������� �������� ������ �� ����

class TaskServer : public std::enable_shared_from_this<TaskServer>
{
public:
	TaskServer() = delete;

	TaskServer(boost::asio::io_context& io_context, unsigned short port, const char* file_path);
	
	void shutdown_server(int session_id); // �������� ������� �� ���������� �������.

	void close_session(int session_id); // �������� ������.

	// �������� ��� ������� � ������� �� ������� ��� Unknown, ���� ����� ������� ���.
	CommandType command_type(const std::string& comm) {
		return ((commands.find(comm) == commands.end()) ? CommandType::Unknown : commands[comm]);
	}

	// �������� ��� common �������.
	CommonRequest common_request_type(const std::string& comm) {
		return ((common_requests.find(comm) == common_requests.end()) ? CommonRequest::Unknown : common_requests[comm]);
	}

	// �������� ��� get �������.
	GetRequest get_request_type(const std::string& comm) {
		return ((get_requests.find(comm) == get_requests.end()) ? GetRequest::Unknown : get_requests[comm]);
	}

	// �������� ��� add �������.
	AddRequest add_request_type(const std::string& comm) {
		return ((add_requests.find(comm) == add_requests.end()) ? AddRequest::Unknown : add_requests[comm]);
	}

	// �������� ��� set �������.
	SetRequest set_request_type(const std::string& comm) {
		return ((set_requests.find(comm) == set_requests.end()) ? SetRequest::Unknown : set_requests[comm]);
	}

	// �������� ��� del �������.
	DelRequest del_request_type(const std::string& comm) {
		return ((del_requests.find(comm) == del_requests.end()) ? DelRequest::Unknown : del_requests[comm]);
	}

private: // methods
	void do_accept();
	
private: // data
	tcp::acceptor acceptor_;
	
	const request_manager_shared request_manager_ptr;

	// ������� �� �������, ������� ����� ������������ ������.
	command_map commands;
	
	common_map common_requests;
	
	get_req_map get_requests;
	add_req_map add_requests;
	set_req_map set_requests;
	del_req_map del_requests;

	session_map sessions; // ��������� ������.
	int session_number;   // ���-�� ������.

	bool shutdown_flag; // ����, ��� ����� ��������� ������ �������.			
};

using task_server_shared = std::shared_ptr<TaskServer>;
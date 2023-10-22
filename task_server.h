#pragma once

#include "client_session.h"

#define SESSIONS_MAX_NUMBER 5

using boost::asio::ip::tcp;

using session_map = std::map<int, session_shared>;

using command_map = std::map<std::string, CommandType>;


class TaskServer : public std::enable_shared_from_this<TaskServer>
{
public:
	TaskServer() = delete;

	TaskServer(boost::asio::io_context& io_context, unsigned short port, const char* file_path);
	
	void shutdown_server(int session_id); // �������� ������� �� ���������� �������.

	void close_session(int session_id); // �������� ������.

	// �������� ��� ������� � ������� �� ������� ��� Unknown, ���� ����� ������� ���.
	CommandType get_command_type(const std::string& comm) {
		return ((commands.find(comm) == commands.end()) ? CommandType::Unknown : commands[comm]);
	}

private: // methods
	void do_accept();
	
private: // data
	tcp::acceptor acceptor_;
	
	const request_manager_shared request_manager_ptr;

	// ������� �� �������, ������� ����� ������������ ������.
	command_map commands; // ��������, ����� ��������� ��� � ����� ClientSession � ������� ��������� ����������.

	session_map sessions; // ��������� ������.
	int session_number;   // ���-�� ������.

	bool shutdown_flag; // ����, ��� ����� ��������� ������ �������.			
};

using task_server_shared = std::shared_ptr<TaskServer>;
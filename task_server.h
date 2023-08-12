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

	TaskServer(boost::asio::io_context& io_context, unsigned short port, const storage_shared& dstp);
	
	void exit_received(int session_id); // �������� ������� �� ���������� �������.

	// �������� ��� ������� � ������� �� ������� ��� Unknown, ���� ����� ������� ���.
	CommandType get_command_type(const std::string& comm) {
		return ((commands.find(comm) == commands.end()) ? CommandType::Unknown : commands[comm]);
	}

private: // methods
	void do_accept();
	
private: // data
	tcp::acceptor acceptor_;
	
	int session_number;   // ���-�� ������.

	storage_shared data_storage_ptr;

	session_map sessions; // ��������� ������.
	command_map commands; // ������� �� �������, ������� ����� ������������ ������.

	bool shutdown_flag; // ����, ��� ����� ��������� ������ �������.	
};

using task_server_shared = std::shared_ptr<TaskServer>;
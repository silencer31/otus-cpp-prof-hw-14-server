#pragma once

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

#include <thread>
#include <memory>
#include <map>

#include "data_storage.h"

using boost::asio::ip::tcp;

class TaskServer;

using task_server_shared = std::shared_ptr<TaskServer>;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
	
	ClientSession(
		const task_server_shared& ts_ptr,
		const storage_shared& dstp,
		tcp::socket socket, int s_id)
		: socket_(std::move(socket))
		, session_id(s_id)
		, task_server_ptr(ts_ptr)
		, data_storage_ptr(dstp)
		, shutdown_flag(false)
	{}

	~ClientSession() {
		shutdown();
	}

	void start() {
		clear_data_read();
		do_read();
	}

	void shutdown(); // ���������� ������.

private: // methods

	void do_read();
	
	void do_write(std::size_t length);

	// �������� ������� � ��������� ������� exit.
	void report_exit_received();

	// ������� ������ ��� ������ ������ �� ����.
	void clear_data_read();

	// ���������� ������ ��� �������� �� ����.
	void prepare_data_send(const std::string& data);

private: // data
	tcp::socket socket_;
	int session_id;

	const task_server_shared task_server_ptr; // ��� ����� � ��������, ��������� ������ ������.

	const storage_shared data_storage_ptr; // ��� ����� � ���������� ������.
	
	enum { max_length = 1024 };

	char data_read[max_length]; // ��� ��������� ������ �� ����.
	char data_send[max_length];	// ��� �������� ������.

	bool shutdown_flag; // ����, ��� ����� ��������� ������ � ������ �������.
};

using session_shared = std::shared_ptr<ClientSession>;
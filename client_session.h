#pragma once

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

#include <thread>
#include <memory>
#include <map>

using boost::asio::ip::tcp;

class TaskServer;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
	
	ClientSession(tcp::socket socket, int s_id)
		: socket_(std::move(socket)), session_id(s_id), shutdown_flag(false)
	{}

	~ClientSession() {
		shutdown();
	}

	void start() {
		clear_data_read();
		do_read();
	}

	void set_server(TaskServer* ts_ptr);

	void shutdown();

private: // methods

	void do_read();
	
	void do_write(std::size_t length);

	// Сообщить серверу о получении команды exit.
	void report_exit_received();

	void clear_data_read();

	void prepare_data_send(const std::string& data);

private: // data
	tcp::socket socket_;
	int session_id;

	TaskServer *task_server_ptr;

	enum { max_length = 1024 };

	char data_read[max_length]; // Для получения данных из сети.
	char data_send[max_length];	// Для отправки данных.

	bool shutdown_flag;
};
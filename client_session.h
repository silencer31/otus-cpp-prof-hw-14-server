#pragma once

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

#include <thread>
#include <memory>
#include <map>

#include "data_storage.h"

using storage_ptr = std::shared_ptr<DataStorage>;

using boost::asio::ip::tcp;

class TaskServer;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
	
	ClientSession(tcp::socket socket, int s_id, const storage_ptr dstp)
		: socket_(std::move(socket)), session_id(s_id), data_storage_ptr(dstp), shutdown_flag(false)
	{}

	~ClientSession() {
		shutdown();
	}

	void start() {
		clear_data_read();
		do_read();
	}

	void set_server(TaskServer* ts_ptr);

	void shutdown(); // Выключение сессии.

private: // methods

	void do_read();
	
	void do_write(std::size_t length);

	// Сообщить серверу о получении команды exit.
	void report_exit_received();

	// Очистка буфера для приема данных по сети.
	void clear_data_read();

	// Подготовка данных для отправки по сети.
	void prepare_data_send(const std::string& data);

private: // data
	tcp::socket socket_;
	int session_id;

	const storage_ptr data_storage_ptr; // Для связи с хранилищем данных.

	TaskServer *task_server_ptr; // Для связи с сервером, создавшим данную сессию.

	enum { max_length = 1024 };

	char data_read[max_length]; // Для получения данных из сети.
	char data_send[max_length];	// Для отправки данных.

	bool shutdown_flag; // Флаг, что нужно завершать сессию и работу сервера.
};
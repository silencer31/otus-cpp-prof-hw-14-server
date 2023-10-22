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
	
	void shutdown_server(int session_id); // Получена команда на выключение сервера.

	void close_session(int session_id); // Закрытие сессии.

	// Получаем тип команды в запросе от клиента или Unknown, если такой команды нет.
	CommandType get_command_type(const std::string& comm) {
		return ((commands.find(comm) == commands.end()) ? CommandType::Unknown : commands[comm]);
	}

private: // methods
	void do_accept();
	
private: // data
	tcp::acceptor acceptor_;
	
	const request_manager_shared request_manager_ptr;

	// Команды от клиента, которые готов обрабатывать сервер.
	command_map commands; // Возможно, лучше перенести это в класс ClientSession и сделать статичным параметром.

	session_map sessions; // Коллекция сессий.
	int session_number;   // Кол-во сессий.

	bool shutdown_flag; // Флаг, что нужно завершать работу сервера.			
};

using task_server_shared = std::shared_ptr<TaskServer>;
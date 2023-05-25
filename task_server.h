#pragma once

#include "client_session.h"

#define SESSIONS_MAX_NUMBER 5

using boost::asio::ip::tcp;
using session_ptr = std::shared_ptr<ClientSession>;
using session_map = std::map<int, session_ptr>;


class TaskServer
{
public:
	TaskServer(boost::asio::io_context& io_context, unsigned short port, const storage_ptr dstp)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
		data_storage_ptr(dstp),
		session_number(0),
		shutdown_flag(false)
	{
		do_accept();
	}

	void exit_received(int session_id); // Получена команда на выключение сервера.

private: // methods
	void do_accept();
	
private: // data
	tcp::acceptor acceptor_;
	
	int session_number;   // Кол-во сессий.
	session_map sessions; // Коллекция сессий.

	bool shutdown_flag; // Флаг, что нужно завершать работу сервера.

	const storage_ptr data_storage_ptr;
};
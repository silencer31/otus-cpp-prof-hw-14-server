#pragma once

#include "client_session.h"

using boost::asio::ip::tcp;
using session_ptr = std::shared_ptr<ClientSession>;
using session_map = std::map<int, session_ptr>;


class TaskServer
{
public:
	TaskServer(boost::asio::io_context& io_context, unsigned short port, const storage_ptr dstp)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
		data_storage(dstp),
		session_number(0),
		shutdown_flag(false)
	{
		if (data_storage->init_database()) {
			do_accept();
		}
	}

	void exit_received(int session_id); // Получена команда на выключение сервера.

private: // methods
	void do_accept();
	
private: // data
	tcp::acceptor acceptor_;
	
	int session_number;
	session_map sessions; // Коллекция сессий.

	bool shutdown_flag;

	const storage_ptr data_storage;
};
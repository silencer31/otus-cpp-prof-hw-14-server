#pragma once

#include "client_session.h"

using boost::asio::ip::tcp;
using session_ptr = std::shared_ptr<ClientSession>;
using session_map = std::map<int, session_ptr>;

class TaskServer
{
public:
	TaskServer(boost::asio::io_context& io_context, unsigned short port)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
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

	//boost::shared_ptr<tcp::socket> socket_ptr;
	
	int session_number;
	session_map sessions; // Коллекция сессий.

	bool shutdown_flag;
};
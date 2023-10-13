#include "task_server.h"

#include <iostream>

TaskServer::TaskServer(boost::asio::io_context& io_context, unsigned short port, const storage_shared& dstp)
	: acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
	, data_storage_ptr(dstp)
	, session_number(0)
	, shutdown_flag(false)
{
	commands["test"]	 = CommandType::Test;
	commands["shutdown"] = CommandType::Shutdown;
	commands["login"]	 = CommandType::Login;
	commands["getdata"]  = CommandType::GetData;
	commands["editdata"] = CommandType::EditData;

	do_accept();
}

void TaskServer::do_accept()
{
	acceptor_.async_accept( // Если будет входящее соединение, выполнить следующую функцию.
		[this](boost::system::error_code errcode, tcp::socket socket)
		{
			if (shutdown_flag) {
				std::cout << "\n -- connection rejected. Server is shutting down!" << std::endl;
				return;
			}

			// Проверяем, сколько активных сессий уже ведётся
			if (sessions.size() == SESSIONS_MAX_NUMBER) {
				std::cout << "\n -- connection rejected. Too many sessions!" << std::endl;
				do_accept(); // Переключаемся на ожидание следующего соединения.
				return;
			}

			// Смотрим была ли ошибка boost.
			if (errcode) {
				std::cout << "\n -- connection rejected. Boost system error: " << errcode.message() << std::endl;
				do_accept(); // Переключаемся на ожидание следующего соединения.
				return;
			}

			std::cout << "\n -- connection accepted" << std::endl;
			
			// Создаем клиентскую сессию и запускаем прием данных.
			session_shared session_ptr = std::make_shared<ClientSession>(
				shared_from_this(),
				data_storage_ptr,
				std::move(socket),
				session_number // Каждая сессия знает свой номер.
			);

			// Добавляем новую сессию в коллекцию сессий сервера.
			sessions.insert(std::pair{ session_number, session_ptr });
				
			session_number++;

			// Начало работы сессии.
			session_ptr->start();
			
			// Переключаемся на ожидание следующего соединения.
			do_accept();
		}
	);

	std::cout << "\nwaiting for new connection" << std::endl;
}

// Получена команда на выключение сервера.
void TaskServer::exit_received(int session_id)
{
	std::cout << "Server. Exit request received from session: " << session_id  << std::endl;
	
	shutdown_flag = true;

	// В цикле завершаем все сессии.
	for (const auto& [id, session_ptr] : sessions) {
		if (id != session_id) { // Пропускаем сессию, от которой пришел сигнал на завершение.
			session_ptr->shutdown();
		}
	}

	// Отключаем прием новых соединений.
	acceptor_.cancel();
	//acceptor_.close();
}
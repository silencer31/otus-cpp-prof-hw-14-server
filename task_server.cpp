#include "task_server.h"

#include <iostream>

void TaskServer::do_accept()
{
	acceptor_.async_accept( // Если будет входящее соединение, выполнить следующую функцию.
		[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (shutdown_flag) {
				return;
			}

			std::cout << "\n -- connection rejected" << std::endl;

			// Проверяем, сколько активных сессий уже ведётся
			if (sessions.size() == SESSIONS_MAX_NUMBER) {
				do_accept(); // Переключаемся на ожидание следующего соединения.
				return;
			}

			std::cout << "\n -- connection accepted" << std::endl;

			if (!ec)
			{
				// Создаем клиентскую сессию и запускаем прием данных.
				//std::make_shared<ClientSession>(std::move(socket))->start();
				session_ptr s_ptr = std::make_shared<ClientSession>(std::move(socket), session_number, data_storage_ptr);
				s_ptr->set_server(this);

				sessions.insert(std::pair{ session_number, s_ptr });
				
				session_number++;

				//std::cout << "sessions number: " << sessions.size() << std::endl;

				s_ptr->start();
			}

			do_accept(); // Переключаемся на ожидание следующего соединения.
		}
	);

	std::cout << "\nwait new acception" << std::endl;
}

// Получена команда на выключение сервера.
void TaskServer::exit_received(int session_id)
{
	std::cout << "Server. Exit received " << session_id  << std::endl;
	
	shutdown_flag = true;

	// В цикле завершаем все сессии.
	for (const auto& [key, value] : sessions) {
		//value->shutdown();
		if (key != session_id) { // Пропускаем сессию, от которой пришел сигнал на завершение.
			value->shutdown();
		}
	}

	acceptor_.cancel();
	//acceptor_.close();
}
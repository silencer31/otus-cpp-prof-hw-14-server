#include "task_server.h"

#include <iostream>

void TaskServer::do_accept()
{
	acceptor_.async_accept( // ���� ����� �������� ����������, ��������� ��������� �������.
		[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (shutdown_flag) {
				return;
			}

			std::cout << "\n -- connection rejected" << std::endl;

			// ���������, ������� �������� ������ ��� ������
			if (sessions.size() == SESSIONS_MAX_NUMBER) {
				do_accept(); // ������������� �� �������� ���������� ����������.
				return;
			}

			std::cout << "\n -- connection accepted" << std::endl;

			if (!ec)
			{
				// ������� ���������� ������ � ��������� ����� ������.
				//std::make_shared<ClientSession>(std::move(socket))->start();
				session_ptr s_ptr = std::make_shared<ClientSession>(
					shared_from_this(),
					data_storage_ptr,
					std::move(socket),
					session_number
				);

				sessions.insert(std::pair{ session_number, s_ptr });
				
				session_number++;

				//std::cout << "sessions number: " << sessions.size() << std::endl;

				s_ptr->start();
			}

			do_accept(); // ������������� �� �������� ���������� ����������.
		}
	);

	std::cout << "\nwait new acception" << std::endl;
}

// �������� ������� �� ���������� �������.
void TaskServer::exit_received(int session_id)
{
	std::cout << "Server. Exit received " << session_id  << std::endl;
	
	shutdown_flag = true;

	// � ����� ��������� ��� ������.
	for (const auto& [key, value] : sessions) {
		//value->shutdown();
		if (key != session_id) { // ���������� ������, �� ������� ������ ������ �� ����������.
			value->shutdown();
		}
	}

	acceptor_.cancel();
	//acceptor_.close();
}
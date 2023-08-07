#include "task_server.h"

#include <iostream>

void TaskServer::do_accept()
{
	acceptor_.async_accept( // ���� ����� �������� ����������, ��������� ��������� �������.
		[this](boost::system::error_code errcode, tcp::socket socket)
		{
			if (shutdown_flag) {
				std::cout << "\n -- connection rejected. Server is shutting down!" << std::endl;
				return;
			}

			// ���������, ������� �������� ������ ��� ������
			if (sessions.size() == SESSIONS_MAX_NUMBER) {
				std::cout << "\n -- connection rejected. Too many sessions!" << std::endl;
				do_accept(); // ������������� �� �������� ���������� ����������.
				return;
			}

			// ������� ���� �� ������ boost.
			if (errcode) {
				std::cout << "\n -- connection rejected. Boost system error: " << errcode.message() << std::endl;
				do_accept(); // ������������� �� �������� ���������� ����������.
				return;
			}

			std::cout << "\n -- connection accepted" << std::endl;
			
			// ������� ���������� ������ � ��������� ����� ������.
			session_shared session_ptr = std::make_shared<ClientSession>(
				shared_from_this(),
				data_storage_ptr,
				std::move(socket),
				session_number // ������ ������ ����� ���� �����.
			);

			// ��������� ����� ������ � ��������� ������ �������.
			sessions.insert(std::pair{ session_number, session_ptr });
				
			session_number++;

			// ������ ������ ������.
			session_ptr->start();
			
			// ������������� �� �������� ���������� ����������.
			do_accept();
		}
	);

	std::cout << "\nwaiting for new connection" << std::endl;
}

// �������� ������� �� ���������� �������.
void TaskServer::exit_received(int session_id)
{
	std::cout << "Server. Exit request received from session: " << session_id  << std::endl;
	
	shutdown_flag = true;

	// � ����� ��������� ��� ������.
	for (const auto& [id, session_ptr] : sessions) {
		if (id != session_id) { // ���������� ������, �� ������� ������ ������ �� ����������.
			session_ptr->shutdown();
		}
	}

	// ��������� ����� ����� ����������.
	acceptor_.cancel();
	//acceptor_.close();
}
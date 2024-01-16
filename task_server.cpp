#include "task_server.h"

#include <iostream>

TaskServer::TaskServer(boost::asio::io_context& io_context, unsigned short port, const char* file_path)
	: acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
	, request_manager_ptr(std::make_shared<RequestManager>(file_path))
{
	commands["test"]	  = CommandType::Test;
	commands["closedown"] = CommandType::Closedown;
	commands["shutdown"]  = CommandType::Shutdown;
	commands["login"]	  = CommandType::Login;
	commands["get"]		  = CommandType::Get;
	commands["add"]		  = CommandType::Add;
	commands["set"]		  = CommandType::Set;
	commands["del"]		  = CommandType::Del;

	get_requests["username"]	= GetRequest::Username;
	get_requests["fullname"]	= GetRequest::Fullname;
	get_requests["userlist"]	= GetRequest::UserList;
	get_requests["tasklist"]	= GetRequest::TaskList;
	get_requests["statuslist"]	= GetRequest::StatusList;
	get_requests["typelist"]	= GetRequest::TypeList;
	get_requests["taskdata"]	= GetRequest::TaskData;
		
	add_requests["user"] = AddRequest::User;
	add_requests["task"] = AddRequest::Task;
	
	set_requests["password"] = SetRequest::Password;
	set_requests["usertype"] = SetRequest::UserType;
	set_requests["taskstatus"] = SetRequest::TaskStatus;
	set_requests["taskuser"] = SetRequest::TaskUser;
	set_requests["deadline"] = SetRequest::TaskDeadline;
		
	del_requests["user"] = DelRequest::User;
	del_requests["task"] = DelRequest::Task;

	session_number = 0;

	shutdown_flag = false;

	do_accept();
}

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
				request_manager_ptr,
				std::move(socket),
				session_number // ������ ������ ����� ���� �����.
			);

			// ��������� ����� ������ � ��������� ������ �������.
			sessions[session_number] = session_ptr;
				
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
void TaskServer::shutdown_server(int session_id)
{
	std::cout << "Task server: Exit request received from session: " << session_id  << std::endl;
	
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

// �������� ������.
void TaskServer::close_session(int session_id)
{
	std::cout << "Task server: Session will be closed: " << session_id << std::endl;

	sessions[session_id]->shutdown();
	sessions.erase(session_id);
	session_number--;
}
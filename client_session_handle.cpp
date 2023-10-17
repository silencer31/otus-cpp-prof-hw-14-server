#include "client_session.h"
#include "task_server.h"

#include <iostream>

// Обработка запроса от клиента.
void ClientSession::handle_request(const json& jdata)
{
	// 
	if (jdata.is_object()) {
		std::cout << "jdata is object" << std::endl;
	}
	else {
		std::cout << "jdata is NOT object" << std::endl;
	}

	// 
	if ( !jdata.contains("command")) {
		reply_error(RequestError::NoCommand, CommandType::Unknown);
		return;
	}

	last_command_str = jdata["command"];

	// Определяем тип команды/запроса.
	CommandType command_type = task_server_ptr->get_command_type(last_command_str);

	switch (command_type) {
	case CommandType::Unknown:
		std::cout << "Unknown command has been received" << std::endl;
		reply_error(RequestError::UnknownCommand, CommandType::Unknown);
		break;
	case CommandType::Test:
		std::cout << "Test request has been received" << std::endl;
		reply_request(CommandType::Test, !shutdown_session_flag);
		break;
	case CommandType::Closedown:
		std::cout << "Closedown command received!" << std::endl;
		reply_request(CommandType::Closedown, true);
		handle_closedown();
		break;
	case CommandType::Shutdown:
		std::cout << "Shutdown command received!" << std::endl;
		reply_request(CommandType::Shutdown, true);
		handle_shutdown(); // Сообщаем серверу о необходимости завершения работы.
		break;
	case CommandType::Login:
		std::cout << "Handle login from client" << std::endl;
		handle_login(jdata);
		break;
	case CommandType::GetData:
		std::cout << "Request to get data" << std::endl;
		handle_getdata(jdata);
		break;
	case CommandType::EditData:
		std::cout << "Request to edit data" << std::endl;
		handle_editdata(jdata);
		break;
	default:
		break;
	}	
}

// Обработка запроса на завершение сессии.
void ClientSession::handle_closedown()
{
	shutdown_session_flag = true;
	task_server_ptr->close_session(session_id); // Сообщаем серверу о завершении сессии.
}

// Обработка запроса на выключение сервера.
void ClientSession::handle_shutdown()
{
	shutdown_session_flag = true;
	task_server_ptr->shutdown_server(session_id);
}

// Обработка запроса проверки пары логин/пароль.
void ClientSession::handle_login(const json& jdata)
{
	if (!jdata.contains("username") || !jdata.contains("password")) {
		reply_error(RequestError::BadParameters);
		return;
	}

	reply_request(CommandType::Login, data_storage_ptr->check_login(username, password));
}

// Обработка запроса на получение данных.
void ClientSession::handle_getdata(const json& jdata)
{

}

// Обработка запроса на изменение данных.
void ClientSession::handle_editdata(const json& jdata)
{

}
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

	// 
	if ( !jdata.contains("command")) {
		reply_error(RequestError::BadParameters);
		return;
	}

	// Определяем тип команды/запроса.
	CommandType command_type = task_server_ptr->get_command_type(jdata["command"]);

	switch (command_type) {
	case CommandType::Unknown:
		std::cout << "Unknown command has been received" << std::endl;
		reply_error(RequestError::UnknownCommand);
		break;
	case CommandType::Test:
		std::cout << "Test request has been received" << std::endl;
		reply_request(CommandType::Test, !shutdown_flag);
		break;
	case CommandType::Shutdown:
		std::cout << "Shutdown command received!" << std::endl;
		reply_request(CommandType::Shutdown, true);
		handle_shutdown(); // Сообщаем серверу о необходимости завершения работы.
		break;
	case CommandType::Login:
		std::cout << "Handle login from client" << std::endl;
		if (!jdata.contains("username") || !jdata.contains("password")) {
			reply_error(RequestError::BadParameters);
			return;
		}
		handle_login( jdata["username"], jdata["password"]);
		break;
	case CommandType::GetData:
		std::cout << "Request to get data" << std::endl;
		break;
	case CommandType::EditData:
		std::cout << "Request to edit data" << std::endl;
		break;
	default:
		break;
	}

	

}

// Обработка запроса на выключение сервера.
void ClientSession::handle_shutdown()
{
	shutdown_flag = true;
	task_server_ptr->exit_received(session_id);
}

// Обработка запроса проверки пары логин/пароль.
void ClientSession::handle_login(const std::string& username, const std::string& password)
{
	reply_request(CommandType::Login, data_storage_ptr->check_login(username, password));
}
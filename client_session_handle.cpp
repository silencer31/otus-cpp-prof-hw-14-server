#include "client_session.h"
#include "task_server.h"

#include <iostream>

// Обработка запроса от клиента.
void ClientSession::handle_request(const json& jdata)
{
	// 
	if (jdata.is_object()) {

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
		std::cout << "test request has been received" << std::endl;
		reply_request(CommandType::Test);
		break;
	case CommandType::Shutdown:
		std::cout << "Shutdown command received!" << std::endl;
		reply_request(CommandType::Shutdown);
		handle_shutdown(); // Сообщаем серверу о необходимости завершения работы.
		break;
	case CommandType::Login:
		std::cout << "Handle login from client" << std::endl;
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

void ClientSession::handle_shutdown()
{
	shutdown_flag = true;
	task_server_ptr->exit_received(session_id);
}
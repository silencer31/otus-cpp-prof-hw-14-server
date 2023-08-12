#include "client_session.h"

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
		reply_error(RequestError::UnknownCommand);
		break;
	case CommandType::Test:
		reply_request(CommandType::Test);
		break;
	case CommandType::Shutdown:
		std::cout << "Shutdown command received!" << std::endl;
		shutdown_flag = true;
		report_exit_received(); // Сообщаем серверу о необходимости завершения работы.
		break;
	case CommandType::Login:
		break;
	case CommandType::GetData:
		break;
	case CommandType::EditData:
		break;
	default:
		break;
	}

	

}
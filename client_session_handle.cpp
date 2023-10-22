#include "client_session.h"
#include "task_server.h"

#include <iostream>

// Обработка запроса от клиента.
void ClientSession::handle_request()
{
	// Определяем тип команды/запроса.
	switch (task_server_ptr->get_command_type(client_request["command"])) {
	case CommandType::Unknown:
		reply_error(RequestError::UnknownCommand, CommandType::Unknown);
		break;
	case CommandType::Test:
		reply_request(CommandType::Test);
		break;
	case CommandType::Closedown: // Завершение текущей сессии с клиентом.
		handle_closedown();
		break;
	case CommandType::Shutdown:
		handle_shutdown(); // Сообщаем серверу о необходимости завершения работы.
		break;
	case CommandType::Login:
		handle_login(jdata);
		break;
	case CommandType::GetData:
		handle_getdata(jdata);
		break;
	case CommandType::EditData:
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

	reply_request(CommandType::Closedown);
}

// Обработка запроса на выключение сервера.
void ClientSession::handle_shutdown()
{
	shutdown_session_flag = true;
	task_server_ptr->shutdown_server(session_id);

	reply_request(CommandType::Shutdown);
}

// Обработка запроса проверки пары логин/пароль.
void ClientSession::handle_login()
{
	if (!client_request.contains("username") || !client_request.contains("password")) {
		reply_error(RequestError::BadParameters);
		return;
	}

	data_storage_ptr->check_login(username, password);

	reply_request(CommandType::Login);
}

// Обработка запроса на получение данных.
void ClientSession::handle_getdata()
{
	// Определяем какие данные нужны из базы
}

// Обработка запроса на изменение данных.
void ClientSession::handle_editdata()
{
	// Определяем какие данные нужно изменить в базе
}
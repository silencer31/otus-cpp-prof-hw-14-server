#include "client_session.h"
#include "task_server.h"

#include <iostream>

// Обработка запроса от клиента.
void ClientSession::handle_request()
{
	server_reply.clear(); // Очищаем предыдущий ответ.

	// Определяем тип команды/запроса.
	switch (task_server_ptr->get_command_type(client_request["command"])) {
	case CommandType::Unknown:
		reply_error(RequestError::UnknownCommand);
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
		handle_login();
		break;
	case CommandType::Get:
		handle_get();
		break;
	case CommandType::Add:
		handle_add();
		break;
	case CommandType::Edit:
		handle_edit();
		break;
	case CommandType::Delete:
		handle_delete();
		break;
	default:
		break;
	}	
}

// Обработка запроса на завершение сессии.
void ClientSession::handle_closedown()
{
	// Проверяем, был ли залогинен пользователь.
	if (!user_logged) {
		server_reply["details"] = "not logged in";
		reply_request(CommandType::Closedown);
		return;
	}

	shutdown_session_flag = true;
	task_server_ptr->close_session(session_id); // Сообщаем серверу о завершении сессии.

	reply_request(CommandType::Closedown);
}

// Обработка запроса на выключение сервера.
void ClientSession::handle_shutdown()
{
	// Проверяем, был ли залогинен пользователь.
	if (!user_logged) {
		server_reply["details"] = "not logged in";
		reply_request(CommandType::Closedown);
		return;
	}

	// Только администратор может выключить сервер.
	if (logged_user_type != UserType::Administrator) {
		server_reply["details"] = "operation is not permitted";
		reply_request(CommandType::Closedown);
		return;
	}

	shutdown_session_flag = true;
	task_server_ptr->shutdown_server(session_id);

	reply_request(CommandType::Shutdown);
}

// Обработка запроса проверки пары логин/пароль.
void ClientSession::handle_login()
{
	// Проверка наличия необходимых параметров в запросе.
	if (!client_request.contains("username") || !client_request.contains("password")) {
		reply_error(RequestError::BadParameters);
		return;
	}

	// Проверяем, есть ли такой логин в базе.
	if (!request_manager_ptr->check_login(client_request["username"])) {
		server_reply["details"] = "username not found";
		reply_request(CommandType::Login);
		return;
	}

	// Пробуем получить user id из базы по логину.
	logged_user_id = request_manager_ptr->get_user_id_by_login(client_request["username"]);

	if (logged_user_id < 0) {
		server_reply["details"] = "unable to get user id by username";
		reply_request(CommandType::Login);
		return;
	}

	// Проверяем пароль пользователя.
	if (!request_manager_ptr->check_password(logged_user_id, client_request["password"])) {
		server_reply["details"] = "password is not correct";
		reply_request(CommandType::Login);
		return;
	}

	// Пробуем узнать тип пользователя.
	int user_type_number = request_manager_ptr->get_user_type_by_user_id(logged_user_id);

	if (user_type_number < 0) {
		logged_user_id = 0;
		server_reply["details"] = "unable to get user type by user id";
		reply_request(CommandType::Login);
		return;
	}
	
	// Если всё прошло успешно, запоминаем залогиненного пользователя сессии.
	logged_user_type = static_cast<UserType>(user_type_number);
	user_logged = true;

	server_reply["details"] = "successfully logged in";
	server_reply["user_type"] = user_type_number;

	reply_request(CommandType::Login);
}

// Обработка запроса на получение данных.
void ClientSession::handle_get()
{
	// Определяем какие данные нужны из базы
}

// Обработка запроса на добавление данных.
void ClientSession::handle_add()
{

}

// Обработка запроса на изменение данных.
void ClientSession::handle_edit()
{
	// Определяем какие данные нужно изменить в базе
}

// Обработка запроса на удаление данных.
void ClientSession::handle_delete()
{

}
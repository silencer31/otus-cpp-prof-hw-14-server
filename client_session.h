#pragma once

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

#include <thread>
#include <memory>
#include <map>

#include "nlohmann/json.hpp"
#include "request_manager.h"


using boost::asio::ip::tcp;
using json = nlohmann::json;
using namespace nlohmann::literals;

// Виды ошибок в запросе от клиента.
enum class RequestError {
	ParseError,
	IsNull,
	WrongType, // Неверный тип данных в запросе.
	NoCommand, // Нельзя определить тип запроса
	UnknownCommand, // Неизвестный тип запроса
	BadParameters // Что-то не так с параметрами
};

// Виды команд от клиента.
enum class CommandType {
	Unknown,	// Неизвестный серверу тип команды.
	Test,		// Тестовый запрос для проверки связи.
	Closedown,	// Сессия будет завершена.
	Shutdown,	// Запрос на выключение сервера.
	Login,		// Логин пользователя в системе.
	Get,		// Получение данных из базы.
	Add,		// Добавление данных в базу.
	Edit,		// Изменение значения в базе.
	Delete		// Удаление данных из базы.
};

class TaskServer;

using task_server_shared = std::shared_ptr<TaskServer>;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
	
	ClientSession(
		const task_server_shared& ts_ptr,
		const request_manager_shared& rm_ptr,
		tcp::socket socket, int s_id)
		: socket_(std::move(socket))
		, session_id(s_id)
		, task_server_ptr(ts_ptr)
		, request_manager_ptr(rm_ptr)
		, logged_user_id(0)
		, logged_user_type(UserType::User)
		, user_logged(false)
		, shutdown_session_flag(false)
	{}

	~ClientSession() {
		shutdown();
	}

	void start() {
		clear_data_read();
		do_read();
	}

	void shutdown(); // Выключение сессии.

private: // methods

	void do_read();
	
	void do_write(const std::string& answer);

	// Очистка буфера для приема данных по сети.
	void clear_data_read();

	// Подготовка данных для отправки по сети.
	void prepare_data_send(const std::string& data);

	// Обработка запроса от клиента.
	void handle_request();

	// Обработка запроса на завершение сессии.
	void handle_closedown();

	// Обработка запроса на выключение сервера.
	void handle_shutdown();

	// Обработка запроса проверки пары логин/пароль.
	void handle_login();

	// Обработка запроса на получение данных.
	void handle_get();

	// Обработка запроса на добавление данных.
	void handle_add();

	// Обработка запроса на изменение данных.
	void handle_edit();

	// Обработка запроса на удаление данных.
	void handle_delete();

	// Сообщить клиенту об ошибке в запросе.
	void reply_error(RequestError request_error);

	// Ответ клиенту на запрос.
	void reply_request(CommandType command_type);

private: // data
	tcp::socket socket_;
	int session_id; // Идентификатор сессии.

	const task_server_shared task_server_ptr; // Для связи с сервером, создавшим данную сессию.

	const request_manager_shared request_manager_ptr; // Для отправки запросов к базе данных.
	
	enum { max_length = 1024 };

	char data_read[max_length]; // Для получения данных из сети.
	char data_send[max_length];	// Для отправки данных.

	int logged_user_id; // Идентификатор залогиненного пользователя. 
	UserType logged_user_type; // Тип залогиненного пользователя.
	bool user_logged; // Флаг, что пользователь был успешно залогинен. Иначе не даём изменить базу.

	bool shutdown_session_flag; // Флаг, что завершается работа сессии.

	json client_request; // Запрос, который находится в обработке.
	json server_reply;   // Ответ клиенту от сервера, если не было ошибки в запросе.
};

using session_shared = std::shared_ptr<ClientSession>;
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
	ParseError = 0,		// Невозможно распарсить запрос в json.
	IsNull = 1,			// Получен пустой запрос.
	NotObject = 2,		// Данные в запросе не являются json объектом.
	NoCommand = 3,		// Нельзя определить тип запроса.
	UnknownCommand = 4, // Неизвестный тип запроса.
	NoParameter = 5,	// Необходимый параметр не найден в запросе.
	BadValue = 6,		// Параметр имеет некорректное значение.
	NotLogged = 7,		// Запрос не может быть обработан, т.к. пользователь ещё не залогинен в сессии.
	NoPermission = 8	// У пользователя не хватает прав на выполнение данного запроса.
};

// Виды команд от клиента.
enum class CommandType {
	Unknown = 0,	// Неизвестный серверу тип команды.
	Test = 1,		// Тестовый запрос для проверки связи.
	Login = 2,		// Логин пользователя в системе.
	Closedown = 3,	// Сессия будет завершена.
	Shutdown = 4,	// Запрос на выключение сервера.
	Get = 5,		// Получение данных из базы.
	Add = 6,		// Добавление данных в базу.
	Set = 7,		// Изменение значения в базе.
	Del = 8			// Удаление данных из базы.
};

// Виды запросов на получение данных.
enum class GetRequest {
	Unknown = 0,	// Неизвестный тип запроса.
	Fullname = 1,	// Узнать ФИО пользователя.
	UserList = 2,	// Получить список id всех пользователей.
	TaskList = 3,	// Получить список id всех задач.
	StatusList = 4,	// Получить список статусов задач с описанием.
	TypeList = 5,	// Получить список типов пользователей с описанием.
	TaskData = 6	// Получить данные конкретной задачи.
};

// Виды запросов на добавление данных.
enum class AddRequest {
	Unknown = 0,	// Неизвестный тип запроса.
	User = 1,		// Добавить нового пользователя.
	Task = 2		// Добавить задачу.
};

// Виды запросов на изменение данных.
enum class SetRequest {
	Unknown = 0,	// Неизвестный тип запроса.
	Password = 1,	// Изменить пароль пользователя.
	UserType = 2,	// Изменить тип пользователя.
	TaskStatus = 3,	// Изменить статус задачи.
	TaskUser = 4	// Назначить пользователя на задачу.
};

// Виды запросов на удаление данных.
enum class DelRequest {
	Unknown = 0,	// Неизвестный тип запроса.
	User = 1,		// Удалить пользователя.
	Task = 2		// Удалить задачу.
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
	void handle_set();

	// Обработка запроса на удаление данных.
	void handle_del();

	// Сообщить клиенту об ошибке в запросе.
	void reply_error(RequestError request_error);

	// Ответ клиенту на запрос.
	void reply_request(CommandType command_type);

	void get_fullname();

	void get_userlist();

	void get_tasklist();

	void get_statuslist();

	void get_typelist();

	void get_taskdata();

	void add_user();

	void add_task();

	void set_password();

	void set_usertype();

	void set_taskstatus();

	void set_taskdata();

	void del_user();

	void del_task();

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
	json server_reply;   // Ответ клиенту от сервера.
};

using session_shared = std::shared_ptr<ClientSession>;
#pragma once

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

#include <thread>
#include <memory>
#include <map>

#include "nlohmann/json.hpp"
#include "data_storage.h"


using boost::asio::ip::tcp;
using json = nlohmann::json;
using namespace nlohmann::literals;

// Виды ошибок обработки запроса от клиента.
enum class RequestError {
	ParseError,
	IsNull,
	UnknownCommand,
	BadParameters
};

// Виды команд от клиента.
enum class CommandType {
	Unknown,  // Неизвестный серверу тип команды.
	Test,     // Тестовый запрос для проверки связи.
	Shutdown, // Запрос на выключение сервера.
	Login,
	GetData,
	EditData
};

class TaskServer;

using task_server_shared = std::shared_ptr<TaskServer>;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
	
	ClientSession(
		const task_server_shared& ts_ptr,
		const storage_shared& dstp,
		tcp::socket socket, int s_id)
		: socket_(std::move(socket))
		, session_id(s_id)
		, task_server_ptr(ts_ptr)
		, data_storage_ptr(dstp)
		, shutdown_flag(false)
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
	void handle_request(const json& jdata);

	// Обработка запроса на выключение сервера.
	void handle_shutdown();

	// Обработка запроса проверки пары логин/пароль.
	void handle_login(const std::string& username, const std::string& password);

	// Сообщить клиенту об ошибке в запросе.
	void reply_error(RequestError req_error);

	// Ответ клиенту на запрос.
	void reply_request(CommandType command_type, bool result);

private: // data
	tcp::socket socket_;
	int session_id;

	const task_server_shared task_server_ptr; // Для связи с сервером, создавшим данную сессию.

	const storage_shared data_storage_ptr; // Для связи с хранилищем данных.
	
	enum { max_length = 1024 };

	char data_read[max_length]; // Для получения данных из сети.
	char data_send[max_length];	// Для отправки данных.

	bool shutdown_flag; // Флаг, что нужно завершать сессию и работу сервера.
};

using session_shared = std::shared_ptr<ClientSession>;
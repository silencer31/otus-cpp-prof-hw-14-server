#pragma once

#include "client_session.h"

#define SESSIONS_MAX_NUMBER 5

using boost::asio::ip::tcp;

using session_map = std::map<int, session_shared>; // Для коллекции сессий пользователей.

using command_map = std::map<std::string, CommandType>; // Для коллекции основных видов запросов серверу.

using common_map = std::map<std::string, CommonRequest>; // Для коллекции подвидов запросов получения общих данных.

using get_req_map = std::map<std::string, GetRequest>; // Для коллекции подвидов запросов получения защищенных данных.
using add_req_map = std::map<std::string, AddRequest>; // Для коллекции подвидов запросов добавления данных в базу.
using set_req_map = std::map<std::string, SetRequest>; // Для коллекции подвидов запросов изменения данных в базе.
using del_req_map = std::map<std::string, DelRequest>; // Для коллекции подвидов запросов удаления данных из базы

/**
* @brief Класс - сервер обработки сетевых подключений и управления сессиями пользователей.
*/
class TaskServer : public std::enable_shared_from_this<TaskServer>
{
public:
	TaskServer() = delete;

	TaskServer(boost::asio::io_context& io_context, unsigned short port, const char* file_path);
	
	/**
	* Получена команда на выключение сервера.
	* @param session_id уникальный id сессии, из которой получена команда.
	*/
	void shutdown_server(int session_id);

	/**
	* Закрытие сессии.
	* @param session_id уникальный id сессии.
	*/
	void close_session(int session_id);

	/**
	* Распознаем тип команды по тексту в запросе от клиента или Unknown, если такой команды нет.
	* @param comm команда в виде строки.
	*/
	CommandType command_type(const std::string& comm) {
		return ((commands.find(comm) == commands.end()) ? CommandType::Unknown : commands[comm]);
	}

	/**
	* Распознаем тип запроса на получение общих (публичных) данных.
	* @param comm запрос в виде строки.
	*/
	CommonRequest common_request_type(const std::string& comm) {
		return ((common_requests.find(comm) == common_requests.end()) ? CommonRequest::Unknown : common_requests[comm]);
	}

	/**
	* Распознаем тип get запроса.
	* @param comm запрос в виде строки.
	*/
	GetRequest get_request_type(const std::string& comm) {
		return ((get_requests.find(comm) == get_requests.end()) ? GetRequest::Unknown : get_requests[comm]);
	}

	/**
	* Распознаем тип add запроса.
	* @param comm запрос в виде строки.
	*/
	AddRequest add_request_type(const std::string& comm) {
		return ((add_requests.find(comm) == add_requests.end()) ? AddRequest::Unknown : add_requests[comm]);
	}

	/**
	* Распознаем тип set запроса.
	* @param comm запрос в виде строки.
	*/
	SetRequest set_request_type(const std::string& comm) {
		return ((set_requests.find(comm) == set_requests.end()) ? SetRequest::Unknown : set_requests[comm]);
	}

	/**
	* Распознаем тип del запроса.
	* @param comm запрос в виде строки.
	*/
	DelRequest del_request_type(const std::string& comm) {
		return ((del_requests.find(comm) == del_requests.end()) ? DelRequest::Unknown : del_requests[comm]);
	}

private: // methods
	/**
	* Приступить к ожиданию новых подключений.
	*/
	void do_accept();
	
private: // data
	tcp::acceptor acceptor_;
	
	const request_manager_shared request_manager_ptr;

	// Команды от клиента, которые готов обрабатывать сервер.
	command_map commands;
	
	common_map common_requests;
	
	get_req_map get_requests;
	add_req_map add_requests;
	set_req_map set_requests;
	del_req_map del_requests;

	session_map sessions; // Коллекция сессий.
	int session_number;   // Кол-во сессий.

	bool shutdown_flag; // Флаг, что нужно завершать работу сервера.			
};

using task_server_shared = std::shared_ptr<TaskServer>;
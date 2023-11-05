//#include "client_session.h"
#include "task_server.h"

#include <iostream>

void ClientSession::do_read()
{
	auto self(shared_from_this());

	socket_.async_read_some( // У сокета вызываем async_read_some
		boost::asio::buffer(data_read, max_length),
		[this, self](boost::system::error_code errcode, std::size_t length)
		{
			if (errcode) {
				std::cout << " Session: " << session_id << ". Read data boost system error code: " << errcode.message() << std::endl;

				// Сообщить серверу о необходимости выключения сессии
				task_server_ptr->close_session(session_id);

				return;
			}

			server_reply.clear(); // Очищаем предыдущий ответ.

			std::cout << " Session: " << session_id << " Received: " << length << " " << data_read << std::endl;

			// Проверка, что данные пришли в формате json.
			if (!json::accept(data_read)) {
				std::cerr << "json parse error. Data: " << data_read << std::endl;
				clear_data_read();
				reply_error(RequestError::ParseError);
				return;
			}

			// Преобразуем char строку в json object.
			const json jdata = json::parse(data_read);

			// Очищаем буфер для следующего запроса от клиента.
			clear_data_read();

			// Проверка, что json не пустой. 
			if (jdata.is_null()) {
				reply_error(RequestError::IsNull);
				return;
			}

			// Проверка, является ли объектом.
			if (!jdata.is_object()) {
				reply_error(RequestError::NotObject);
				return;
			}

			// Наличие поля command обязательно.
			if (!jdata.contains("command")) {
				reply_error(RequestError::NoCommand);
				return;
			}

			client_request.clear();
			client_request = jdata;

			std::cout << "Received request contains command: " << client_request["command"] << std::endl;

			// Обработка запроса в виде json.
			handle_request();
		}
	);
}

void ClientSession::do_write(const std::string& answer)
{
	auto self(shared_from_this());

	prepare_data_send(answer);

	boost::asio::async_write( // Вызываем async_write, указывая сокет.
		socket_, boost::asio::buffer(data_send, answer.length()),
		[this, self](boost::system::error_code errcode, std::size_t /*length*/)
		{
			if (errcode) {
				std::cout << " Session: " << session_id << ". Write data boost system error: " << errcode.message() << std::endl;
			}
			
			if (shutdown_session_flag) { // Если получена команда на выключение, больше не пытаемся читать из сети.
				return;
			}

			do_read();
		}
	);
}

// Выключение сессии.
void ClientSession::shutdown()
{
	if (!socket_.is_open()) {
		return;
	}

	std::cout << "Shutdown process started. Session: " << session_id << std::endl;

	boost::system::error_code ignore;

	socket_.shutdown(
		boost::asio::ip::tcp::socket::shutdown_both,
		ignore
	);

	socket_.close(ignore);

	std::cout << "Shutdown finished. Session: " << session_id << std::endl;
}

// Очистка буфера для приема данных по сети.
void ClientSession::clear_data_read()
{
	for (int i = 0; i < max_length; ++i) {
		data_read[i] = 0;
	}
}

// Подготовка данных для отправки по сети.
void ClientSession::prepare_data_send(const std::string& data)
{
	for (int i = 0; i < max_length; ++i) {
		data_send[i] = 0;
	}

	for (int i = 0; i < data.length(); ++i) {
		data_send[i] = data.c_str()[i];
		if (i == (max_length - 1)) {
			break;
		}
	}
}
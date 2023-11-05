//#include "client_session.h"
#include "task_server.h"

#include <iostream>

void ClientSession::do_read()
{
	auto self(shared_from_this());

	socket_.async_read_some( // � ������ �������� async_read_some
		boost::asio::buffer(data_read, max_length),
		[this, self](boost::system::error_code errcode, std::size_t length)
		{
			if (errcode) {
				std::cout << " Session: " << session_id << ". Read data boost system error code: " << errcode.message() << std::endl;

				// �������� ������� � ������������� ���������� ������
				task_server_ptr->close_session(session_id);

				return;
			}

			server_reply.clear(); // ������� ���������� �����.

			std::cout << " Session: " << session_id << " Received: " << length << " " << data_read << std::endl;

			// ��������, ��� ������ ������ � ������� json.
			if (!json::accept(data_read)) {
				std::cerr << "json parse error. Data: " << data_read << std::endl;
				clear_data_read();
				reply_error(RequestError::ParseError);
				return;
			}

			// ����������� char ������ � json object.
			const json jdata = json::parse(data_read);

			// ������� ����� ��� ���������� ������� �� �������.
			clear_data_read();

			// ��������, ��� json �� ������. 
			if (jdata.is_null()) {
				reply_error(RequestError::IsNull);
				return;
			}

			// ��������, �������� �� ��������.
			if (!jdata.is_object()) {
				reply_error(RequestError::NotObject);
				return;
			}

			// ������� ���� command �����������.
			if (!jdata.contains("command")) {
				reply_error(RequestError::NoCommand);
				return;
			}

			client_request.clear();
			client_request = jdata;

			std::cout << "Received request contains command: " << client_request["command"] << std::endl;

			// ��������� ������� � ���� json.
			handle_request();
		}
	);
}

void ClientSession::do_write(const std::string& answer)
{
	auto self(shared_from_this());

	prepare_data_send(answer);

	boost::asio::async_write( // �������� async_write, �������� �����.
		socket_, boost::asio::buffer(data_send, answer.length()),
		[this, self](boost::system::error_code errcode, std::size_t /*length*/)
		{
			if (errcode) {
				std::cout << " Session: " << session_id << ". Write data boost system error: " << errcode.message() << std::endl;
			}
			
			if (shutdown_session_flag) { // ���� �������� ������� �� ����������, ������ �� �������� ������ �� ����.
				return;
			}

			do_read();
		}
	);
}

// ���������� ������.
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

// ������� ������ ��� ������ ������ �� ����.
void ClientSession::clear_data_read()
{
	for (int i = 0; i < max_length; ++i) {
		data_read[i] = 0;
	}
}

// ���������� ������ ��� �������� �� ����.
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
#include "client_session.h"
#include "task_server.h"

#include "nlohmann/json.hpp"

#include <iostream>

using json = nlohmann::json;
using namespace nlohmann::literals;

void ClientSession::report_exit_received()
{
	task_server_ptr->exit_received(session_id);
}

void ClientSession::do_read()
{
	auto self(shared_from_this());

	socket_.async_read_some( // � ������ �������� async_read_some
		boost::asio::buffer(data_read, max_length),
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{				
				std::cout << " Session: " << session_id << " Received: " << length << " " << data_read << std::endl;
				
				// ����������� char ������ � json object
				json jdata = json::parse(data_read);

				// ���������� ������
				json reply = R"( { "reply" : "ack" })"_json;
				// ��� ���
				json reply_new;
				reply_new["reply"] = "ack";
				
				if (jdata.is_null()) {

				}

				if (jdata.is_object()) {

				}

				if (jdata.contains("request")) {
					
				}

				// ���������� ��� �������/�������.

				if ( strcmp(data_read, "shutdown") == 0) {
					std::cout << "Shutdown command received!" << std::endl;
					shutdown_flag = true;
					report_exit_received(); // �������� ������� � ������������� ���������� ������.
				}

				// ����������� json � ������ ��� ��������.
				std::string answer = reply.dump();
				prepare_data_send(answer);
				
				do_write( answer.length() );
			}
		}
	);
}

void ClientSession::do_write(std::size_t length)
{
	auto self(shared_from_this());

	boost::asio::async_write( // �������� async_write, �������� �����
		socket_, boost::asio::buffer(data_send, length),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (shutdown_flag) { // ���� �������� ������� �� ����������, ������ �� �������� ������ �� ����.
				return;
			}

			if (!ec) {
				clear_data_read();
				do_read();
			}
		}
	);
}

// ���������� ������.
void ClientSession::shutdown()
{
	std::cout << "Shutdown process started. Session: " << session_id << std::endl;

	if (!socket_.is_open()) {
		return;
	}

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
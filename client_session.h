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

// ���� ������ � ������� �� �������.
enum class RequestError {
	ParseError,
	IsNull,
	WrongType, // �������� ��� ������ � �������.
	NoCommand, // ������ ���������� ��� �������
	UnknownCommand, // ����������� ��� �������
	BadParameters // ���-�� �� ��� � �����������
};

// ���� ������ �� �������.
enum class CommandType {
	Unknown,	// ����������� ������� ��� �������.
	Test,		// �������� ������ ��� �������� �����.
	Closedown,	// ������ ����� ���������.
	Shutdown,	// ������ �� ���������� �������.
	Login,		// ����� ������������ � �������.
	Get,		// ��������� ������ �� ����.
	Add,		// ���������� ������ � ����.
	Edit,		// ��������� �������� � ����.
	Delete		// �������� ������ �� ����.
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

	void shutdown(); // ���������� ������.

private: // methods

	void do_read();
	
	void do_write(const std::string& answer);

	// ������� ������ ��� ������ ������ �� ����.
	void clear_data_read();

	// ���������� ������ ��� �������� �� ����.
	void prepare_data_send(const std::string& data);

	// ��������� ������� �� �������.
	void handle_request();

	// ��������� ������� �� ���������� ������.
	void handle_closedown();

	// ��������� ������� �� ���������� �������.
	void handle_shutdown();

	// ��������� ������� �������� ���� �����/������.
	void handle_login();

	// ��������� ������� �� ��������� ������.
	void handle_get();

	// ��������� ������� �� ���������� ������.
	void handle_add();

	// ��������� ������� �� ��������� ������.
	void handle_edit();

	// ��������� ������� �� �������� ������.
	void handle_delete();

	// �������� ������� �� ������ � �������.
	void reply_error(RequestError request_error);

	// ����� ������� �� ������.
	void reply_request(CommandType command_type);

private: // data
	tcp::socket socket_;
	int session_id; // ������������� ������.

	const task_server_shared task_server_ptr; // ��� ����� � ��������, ��������� ������ ������.

	const request_manager_shared request_manager_ptr; // ��� �������� �������� � ���� ������.
	
	enum { max_length = 1024 };

	char data_read[max_length]; // ��� ��������� ������ �� ����.
	char data_send[max_length];	// ��� �������� ������.

	int logged_user_id; // ������������� ������������� ������������. 
	UserType logged_user_type; // ��� ������������� ������������.
	bool user_logged; // ����, ��� ������������ ��� ������� ���������. ����� �� ��� �������� ����.

	bool shutdown_session_flag; // ����, ��� ����������� ������ ������.

	json client_request; // ������, ������� ��������� � ���������.
	json server_reply;   // ����� ������� �� �������, ���� �� ���� ������ � �������.
};

using session_shared = std::shared_ptr<ClientSession>;
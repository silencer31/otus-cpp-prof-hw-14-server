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
	ParseError = 0,		// ���������� ���������� ������ � json.
	IsNull = 1,			// ������� ������ ������.
	NotObject = 2,		// ������ � ������� �� �������� json ��������.
	NoCommand = 3,		// ������ ���������� ��� �������.
	UnknownCommand = 4, // ����������� ��� �������.
	NoParameter = 5,	// ����������� �������� �� ������ � �������.
	BadValue = 6,		// �������� ����� ������������ ��������.
	NotLogged = 7,		// ������ �� ����� ���� ���������, �.�. ������������ ��� �� ��������� � ������.
	NoPermission = 8	// � ������������ �� ������� ���� �� ���������� ������� �������.
};

// ���� ������ �� �������.
enum class CommandType {
	Unknown = 0,	// ����������� ������� ��� �������.
	Test = 1,		// �������� ������ ��� �������� �����.
	Login = 2,		// ����� ������������ � �������.
	Closedown = 3,	// ������ ����� ���������.
	Shutdown = 4,	// ������ �� ���������� �������.
	Get = 5,		// ��������� ������ �� ����.
	Add = 6,		// ���������� ������ � ����.
	Set = 7,		// ��������� �������� � ����.
	Del = 8			// �������� ������ �� ����.
};

// ���� �������� �� ��������� ������.
enum class GetRequest {
	Unknown = 0,	// ����������� ��� �������.
	Fullname = 1,	// ������ ��� ������������.
	UserList = 2,	// �������� ������ id ���� �������������.
	TaskList = 3,	// �������� ������ id ���� �����.
	StatusList = 4,	// �������� ������ �������� ����� � ���������.
	TypeList = 5,	// �������� ������ ����� ������������� � ���������.
	TaskData = 6	// �������� ������ ���������� ������.
};

// ���� �������� �� ���������� ������.
enum class AddRequest {
	Unknown = 0,	// ����������� ��� �������.
	User = 1,		// �������� ������ ������������.
	Task = 2		// �������� ������.
};

// ���� �������� �� ��������� ������.
enum class SetRequest {
	Unknown = 0,	// ����������� ��� �������.
	Password = 1,	// �������� ������ ������������.
	UserType = 2,	// �������� ��� ������������.
	TaskStatus = 3,	// �������� ������ ������.
	TaskUser = 4	// ��������� ������������ �� ������.
};

// ���� �������� �� �������� ������.
enum class DelRequest {
	Unknown = 0,	// ����������� ��� �������.
	User = 1,		// ������� ������������.
	Task = 2		// ������� ������.
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
	void handle_set();

	// ��������� ������� �� �������� ������.
	void handle_del();

	// �������� ������� �� ������ � �������.
	void reply_error(RequestError request_error);

	// ����� ������� �� ������.
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
	json server_reply;   // ����� ������� �� �������.
};

using session_shared = std::shared_ptr<ClientSession>;
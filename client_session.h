#pragma once

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

#include <thread>
#include <memory>
#include <map>
#include <utility>

#ifdef WIN32
#include "nlohmann/json.hpp"
#else
#include <nlohmann/json.hpp>
#endif

#include "request_manager.h"


using boost::asio::ip::tcp;
using json = nlohmann::json;
//using namespace nlohmann::literals;


/**
* @brief ������������ - ���� ������ � ������� �� �������.
*/
enum class RequestError {
	ParseError = 0,		// ���������� ���������� ������ � json.
	IsNull = 1,			// ������� ������ ������.
	NotObject = 2,		// ������ � ������� �� �������� json ��������.
	NoCommand = 3,		// ������ ���������� ��� �������.
	UnknownCommand = 4, // ����������� ��� �������.
	NoParameter = 5,	// ����������� �������� �� ������ � �������.
	BadValue = 6,		// �������� ����� ������������ ��������.
	NotLogged = 7,		// ������ �� ����� ���� ���������, �.�. ������������ ��� �� ��������� � ������.
	AlreadyLogged = 8,	// ������ ������ ������������ � ������ ����� ������.
	NoPermission = 9	// � ������������ �� ������� ���� �� ���������� ������� �������.
};

/**
* @brief ������������ - ���� ������ �� �������.
*/
enum class CommandType {
	Unknown = 0,	// ����������� ������� ��� �������.
	Test = 1,		// �������� ������ ��� �������� �����.
	Common = 2,		// ������ ����� ������. ���� �������������, ������� �����.
	Login = 3,		// ����� ������������ � �������.
	Closedown = 4,	// ������ ����� ���������.
	Shutdown = 5,	// ������ �� ���������� �������.
	Get = 6,		// ��������� ������ �� ����.
	Add = 7,		// ���������� ������ � ����.
	Set = 8,		// ��������� �������� � ����.
	Del = 9			// �������� ������ �� ����.
};

/**
* @brief ������������ - ���� �������� �� ��������� ����� (��������) ������.
*/
enum class CommonRequest {
	Unknown = 0,	 // ����������� ��� �������.
	UserTypes = 1,	 // �������� ������ ����� ������������� � ���������.
	TaskStatuses = 2 // �������� ������ �������� ����� � ���������.
};

/**
* @brief ������������ - ���� �������� �� ��������� ������.
*/
enum class GetRequest {
	Unknown  = 0,	// ����������� ��� �������.
	Username = 1,	// ������ ����� � ��� ������������.
	Fullname = 2,	// ������ ��� ������������.
	UserList = 3,	// �������� ������ id ���� �������������.
	TaskList = 4,	// �������� ������ id ���� �����.
	TaskData = 5	// �������� ������ ���������� ������.
};

/**
* @brief ������������ - ���� �������� �� ���������� ������.
*/
enum class AddRequest {
	Unknown = 0,	// ����������� ��� �������.
	User = 1,		// �������� ������ ������������.
	Task = 2		// �������� ������.
};

/**
* @brief ������������ - ���� �������� �� ��������� ������.
*/
enum class SetRequest {
	Unknown = 0,	 // ����������� ��� �������.
	Password = 1,	 // �������� ������ ������������.
	UserType = 2,	 // �������� ��� ������������.
	TaskStatus = 3,	 // �������� ������ ������.
	TaskUser = 4,	 // ��������� ������������ �� ������.
	TaskDeadline = 5 // �������� deadline ������.
};
 
/**
* @brief ������������ - ���� �������� �� �������� ������.
*/
enum class DelRequest {
	Unknown = 0,	// ����������� ��� �������.
	User = 1,		// ������� ������������.
	Task = 2		// ������� ������.
};

class TaskServer;

using task_server_shared = std::shared_ptr<TaskServer>;

/**
* @brief ����� - ������ ������������. ��������� ������������� ������������ � ������� � ������ ��������� ������.
*/
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
		, logged_user_type(UserType::Unknown)
		, shutdown_session_flag(false)
	{}

	~ClientSession() {
		shutdown();
	}

	/**
	* ������ ������ ������������.
	*/
	void start() {
		clear_data_read();
		do_read();
	}

	/**
	* ���������� ������.
	*/
	void shutdown();

private: // methods

	/**
	* ����������� ������ ������ �� ������.
	*/
	void do_read();
	
	/**
	* ����������� ������ ������ � �����.
	*/
	void do_write(const std::string& answer);

	/**
	* ������� ������ ��� ������ ������ �� ����.
	*/
	void clear_data_read();

	/**
	* ���������� ������ ��� �������� �� ����.
	*/
	void prepare_data_send(const std::string& data);

	/**
	* ��������� ������� �� �������.
	*/
	void handle_request();

	/**
	* �������� ������� �� ��������� ����� ������.
	*/
	void handle_common();

	/**
	* ��������� ������� �������� ���� �����/������.
	*/
	void handle_login();

	/**
	* ��������� ������� �� ���������� ������.
	*/
	void handle_closedown();

	/**
	* ��������� ������� �� ���������� �������.
	*/
	void handle_shutdown();

	/**
	* ��������� ������� �� ��������� ������.
	*/
	void handle_get();

	/**
	* ��������� ������� �� ���������� ������.
	*/
	void handle_add();

	/**
	* ��������� ������� �� ��������� ������.
	*/
	void handle_set();

	/**
	* ��������� ������� �� �������� ������.
	*/
	void handle_del();

	/**
	* �������� ������� �� ������ � �������.
	*/
	void reply_error(RequestError request_error);

	/**
	* ����� ������� �� ������.
	*/
	void reply_request(CommandType command_type);

	/**
	* �������� ������ ��������� ����� ������������ � ������ � �� ���������.
	*/
	void get_user_types();

	/**
	* �������� ������ ��������� �������� ������ � ������ � �� ���������.
	*/
	void get_task_statuses();

	/**
	* �������� ��� ������������ �� user id.
	*/
	void get_fullname();

	/**
	* �������� ����� � ��� ������������ �� user id.
	*/
	void get_username();
 
	/**
	* �������� ������ ���� id �������������.
	*/
	void get_userlist();

	/**
	* ���� ������ user_id, �������� ������ ��� �����, � ���� �� ������, �� ������ ���� �����.
	*/
	void get_tasklist();

	/**
	* �������� ������ ������ �� task id.
	*/
	void get_taskdata();

	/**
	* �������� ������ ������������ � ����.
	*/
	void add_user();
 
	/**
	* �������� ����� ������ � ����.
	*/
	void add_task();
 
	/**
	* ������� ������������ �� ����.
	*/
	void del_user();

	/**
	* ������� ������ �� ����.
	*/
	void del_task();

	/**
	* ���������� ����� ������ � ������������.
	*/
	void set_password();

	/**
	* �������� ��� ������������ � ����.
	*/
	void set_usertype();

	/**
	* ���������� ����� ������ ������ �� task id.
	*/
	void set_taskstatus();

	/**
	* ��������� �� ������ ������� ������������.
	*/
	void set_taskuser();

	/**
	* ���������� ����� �������� deadline � ������.
	*/
	void set_taskdeadline();

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

	bool shutdown_session_flag; // ����, ��� ����������� ������ ������.

	json client_request; // ������, ������� ��������� � ���������.
	json server_reply;   // ����� ������� �� �������.
};

using session_shared = std::shared_ptr<ClientSession>;

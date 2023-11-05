#include "task_server.h"

void ClientSession::del_user()
{


	reply_request(CommandType::Del);
}

void ClientSession::del_task()
{


	reply_request(CommandType::Del);
}
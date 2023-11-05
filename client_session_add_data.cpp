#include "task_server.h"

void ClientSession::add_user()
{


	reply_request(CommandType::Add);
}

void ClientSession::add_task()
{


	reply_request(CommandType::Add);
}
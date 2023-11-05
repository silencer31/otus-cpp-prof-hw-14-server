#include "task_server.h"

void ClientSession::set_password()
{


	reply_request(CommandType::Set);
}

void ClientSession::set_usertype()
{


	reply_request(CommandType::Set);
}

void ClientSession::set_taskstatus()
{


	reply_request(CommandType::Set);
}

void ClientSession::set_taskdata()
{


	reply_request(CommandType::Set);
}
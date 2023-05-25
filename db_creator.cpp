#include <boost/filesystem/operations.hpp>

#include "sql_create_requests.h"
#include "sql_insert_requests.h"
#include "sql_select_requests.h"
#include "sql_execute_functions.h"

#include <iostream>

#ifdef WIN32
#define appname "db_creator.exe"
#else
#define appname "./db_creator"
#endif


int main(int argc, char* argv[])
{
    // ����� ������� ��� ����� ����� ���� ������.
    if (argc != 2) {
        std::cout << "Usage: " << appname << " <data base file name>" << std::endl;
        return 1;
    }

    std::string db_name{ argv[1] };
    if (!db_name.ends_with(".sqlite")) {
        db_name.append(".sqlite");
    }

    // ����� ���� ��� �� �� ������ ������������. ������ ���� �� ��������������.
    if (boost::filesystem::exists(db_name)) {
        std::cout << "Such file for data base already exists. It will NOT be rewritten" << std::endl;
        return 1;
    }
    
    std::cout << "New data base will be created in file: " << db_name << std::endl;
    std::cout << " --------- " << std::endl;

    sqlite3* handle = nullptr;
     
    // �������� ����� � ����� ������.
    if (SQLITE_ERROR == sqlite3_open_v2(db_name.c_str(), &handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {
        std::cout << "Database creation error!" << std::endl;
        return 1;
    }

    if (handle == nullptr) {
        std::cout << "Unexpected error with database handle!" << std::endl;
        return 1;
    }
 
    execute_simple_request(handle, "PRAGMA foreign_keys = ON");


    // - 1 - �������� ������� UserTypes
    // ��� ������� �������� �������� ����� ������������� � � ������ �� �������� �� ����� ������ �������.
    // ������ ������� - �����-����. ������ ������� - ��������� �������� ���� ������������.
    if (false == execute_request(handle, create_user_types, "UserTypes", true)) {
        sqlite3_close(handle);
        return 1;
    }

    // ���������� ���������� �������� � ������� UserTypes
    if (false == execute_request(handle, insert_into_user_types, "UserTypes", false)) {
        sqlite3_close(handle);
        return 1;
    }
        
    // - 2 - �������� ������� Users
    // ������ ������� �������� ���� �������������, ������� ������ � ���� ������.
    // �������� ������ ������� ����� ������ �������������.
    // ������ ������� - ���������� ������������� ������������. ������������ � ������� Security.
    // ������ ������� - �����, ������� ����� ������ ���� ��������.
    // ������ ������� - ���, ��������� - �������, ����� - ��������.
    if (false == execute_request(handle, create_users, "Users", true)) {
        sqlite3_close(handle);
        return 1;
    }

    // ������� � ������� Users ��������������, ��������� ���� ������ � ������������.
    if (false == execute_request(handle, insert_into_users, "Users", false)) {
        sqlite3_close(handle);
        return 1;
    }
   
    // - 3 - �������� ������� Security
    // ������ ������� �������� �� ������������ ������� � ������ � ����� ���� �������� ������ ���������������.
    // ������������ ��� �������� ��� �����/������ � ��� ��������� ���� ������������ ��� ���������� �������� ��� ����.
    // ������ ������� - ���������� ������������� ������������.
    // ������ ������� - �����, ��������������� ���� ������������ �� ������� UserTypes.
    // ������ ������� - ������.
    if (false == execute_request(handle, create_security, "Security", true)) {
        sqlite3_close(handle);
        return 1;
    }
    
    // ������� ������� Security ��� ��� ����������� �������������.
    if (false == execute_request(handle, insert_into_security, "Security", false)) {
        sqlite3_close(handle);
        return 1;
    }

    // - 4 - �������� ������� TaskStatuses
    // ��� ������� �������� �������� ������� �� ��������� ��������� ������ � � ������ �� �������� �� ����� ������ �������.
    // ������ ������� - �����-����. ������ ������� - ��������� �������� ���������.
    if (false == execute_request(handle, create_task_statuses, "TaskStatuses", true)) {
        sqlite3_close(handle);
        return 1;
    }

    // ���������� ���������� �������� � ������� TaskStatuses.
    if (false == execute_request(handle, insert_into_task_statuses, "TaskStatuses", false)) {
        sqlite3_close(handle);
        return 1;
    }


    // - 5 - �������� ������� Tasks
    // ��������� ������� �������� ������ � �� ����� � ��������������.
    // ������ ������� - ���������� ������������� ������.
    // ������ ������� - �������� ������.
    // ������ ������� - ��������, ��� ��������� ������� � ������ ������ ������.
    // ��������� ������� - ���������� ������������� ������������.
    // ����� ������� - �����-������������� ��������� ������ �� ������� TaskStatuses.
    // ��������� ������� - ������� ���� ����������.
    if (false == execute_request(handle, create_tasks, "Tasks", true)) {
        sqlite3_close(handle);
        return 1;
    }
    
    // �������� ��������� ��������� ����� � ������� Tasks.
    // ������ ��� �������������� - ������� ��� ������ ��������� ��.
    // ������ ��� ��������� ���� ������ - ������� ��������� ����� ����.
    // � ��� ������� ������ ��� ������������.
    if (false == execute_request(handle, insert_into_tasks, "Tasks", false)) {
        sqlite3_close(handle);
        return 1;
    }

    std::cout << std::endl;

    // ����� �� ����� ����������� ��������� ������.
    

    // ���������� ������� UserTypes
    execute_request_output_columns(handle, "PRAGMA table_info(UserTypes)", "UserTypes");
    execute_request_output_data(handle, select_from_user_types, "UserTypes");

    // ���������� ������� Users
    execute_request_output_columns(handle, "PRAGMA table_info(Users)", "Users");
    execute_request_output_data(handle, select_from_users, "Users");

    // ���������� ������� Security
    execute_request_output_columns(handle, "PRAGMA table_info(Security)", "Security");
    execute_request_output_data(handle, select_from_security, "Security");

    // ���������� ������� TaskStatuses
    execute_request_output_columns(handle, "PRAGMA table_info(TaskStatuses)", "TaskStatuses");
    execute_request_output_data(handle, select_from_task_statuses, "TaskStatuses");

    // ���������� ������� Tasks
    execute_request_output_columns(handle, "PRAGMA table_info(Tasks)", "Tasks");
    execute_request_output_data(handle, select_from_tasks, "Tasks");

    // �������� ���� ������.
    sqlite3_close(handle);


    return 0;
}
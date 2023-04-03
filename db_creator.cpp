#include <iostream>
#include "sqlite3.h"


int main(int argc, char* argv[])
{
    const char* db_default_name = "tasks_sqlite_db.sqlite";
    std::string db_name;

    sqlite3* handle = nullptr;
    
    if (argc == 2) {
        db_name = argv[1];
    }
    else {
        db_name = db_default_name;
    }

    if (SQLITE_ERROR == sqlite3_open_v2(db_name.c_str(), &handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {
        std::cout << "Database creation error!" << std::endl;
        return 1;
    }

    if (handle == nullptr) {
        std::cout << "Unexpected error with database handle!" << std::endl;
        return 1;
    }

    char* errmsg = nullptr;

    auto exec_callback = [](void*, int columns, char** data, char** names) -> int {
        for (int i = 0; i < columns; ++i) {
            std::cout << names[i] << " = " << (data[i] ? data[i] : "NULL") << std::endl;
        }

        std::cout << std::endl;

        return 0;
    };
        

    // 1 table TaskStatusTypes
    int result = SQLITE_OK;

    const char* create_task_status_types = "CREATE TABLE TaskStatusTypes ("
        "status_type_number TINYINT NOT NULL,"
        "status_type_description CHAR(16) NOT NULL"
        "); ";

    result = sqlite3_exec(handle, create_task_status_types, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table TaskStatusTypes successfully created" << std::endl;
    }
    else {
        std::cout << "An error occured while creation of DB table TaskStatusTypes! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    // 2 table UserTypes
    result = SQLITE_OK;

    const char* create_user_types = "CREATE TABLE UserTypes ("
        "user_type_number TINYINT NOT NULL,"
        "user_type_description CHAR(16) NOT NULL"
        "); ";

    result = sqlite3_exec(handle, create_user_types, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table UserTypes successfully created" << std::endl;
    }
    else {
        std::cout << "An error occured while creation of DB table UserTypes! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    // 3 table Security
    result = SQLITE_OK;

    const char* create_security = "CREATE TABLE Security ("
        "user_unique_id CHAR(10) NOT NULL,"
        "user_type_number TINYINT NOT NULL,"
        "password CHAR(32) NOT NULL"
        "); ";

    result = sqlite3_exec(handle, create_security, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table Security successfully created" << std::endl;
    }
    else {
        std::cout << "An error occured while creation of DB table Security! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    // 4 table Users
    result = SQLITE_OK;

    const char* create_users = "CREATE TABLE Users ("
        "user_unique_id CHAR(10) NOT NULL,"
        "user_unique_login CHAR(16) NOT NULL,"
        "first_name CHAR(24) NOT NULL,"
        "second_name CHAR(24) NOT NULL,"
        "patronymic CHAR(24) NULL"
        "); ";

    
    result = sqlite3_exec(handle, create_users, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table UsersTable successfully created" << std::endl;
    }
    else {
        std::cout << "An error occured while creation of DB table UsersTable! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    // 5 table Tasks
    result = SQLITE_OK;

    const char* create_tasks = "CREATE TABLE Tasks ("
        "task_unique_id CHAR(10) NOT NULL,"
        "task_name CHAR(64) NOT NULL,"
        "task_description VARCHAR(1000) NOT NULL,"
        "user_unique_id CHAR(10) NOT NULL,"
        "status_type_number TINYINT NOT NULL,"
        "deadline DATE NOT NULL"
        "); ";


    result = sqlite3_exec(handle, create_tasks, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table Tasks successfully created" << std::endl;
    }
    else {
        std::cout << "An error occured while creation of DB table Tasks! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }


    
    sqlite3_close(handle);


    return 0;
}
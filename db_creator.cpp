#include <iostream>

#include "sqlite3.h"

#include <boost/filesystem.hpp>


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
    // Ёта таблица содержит описани€ возможного состо€ни€ задачи и соответствующее число-ключ.
    // ƒанные не мен€ютс€.
    int result = SQLITE_OK;

    const char* create_task_status_types = "CREATE TABLE TaskStatusTypes ("
        "status_type_number TINYINT PRIMARY KEY NOT NULL,"
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

    // ƒобавление значений в таблицу TaskStatusTypes
    const char* insert_into_tst = "INSERT INTO TaskStatusTypes (status_type_number, status_type_description) "
        "VALUES (1,'not appointed'), (2,'appointed'), (3,'in progress'), (4,'rejected'), (5,'completed');";

    result = sqlite3_exec(handle, insert_into_tst, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table TaskStatusTypes successfully updated" << std::endl;
    }
    else {
        std::cout << "An error occured while updation of DB table TaskStatusTypes! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }


    // 2 table UserTypes
    // Ёта таблица содержит названи€ типов пользователей и соответствующее число-ключ.
    // ƒанные не мен€ютс€.
    result = SQLITE_OK;

    const char* create_user_types = "CREATE TABLE UserTypes ("
        "user_type_number TINYINT PRIMARY KEY NOT NULL,"
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

    // ƒобавление значений в таблицу UserTypes
    const char* insert_into_user_types = "INSERT INTO UserTypes (user_type_number, user_type_description) "
        "VALUES (1,'administrator'), (2,'db_operator'), (3,'user');";

    result = sqlite3_exec(handle, insert_into_user_types, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table UserTypes successfully updated" << std::endl;
    }
    else {
        std::cout << "An error occured while updation of DB table TaskStatusTypes! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    // 3 table Security
    result = SQLITE_OK;

    const char* create_security = "CREATE TABLE Security ("
        "user_unique_id SMALLINT PRIMARY KEY NOT NULL,"
        "user_type_number TINYINT DEFAULT 1,"
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
        "user_unique_id SMALLINT PRIMARY KEY NOT NULL,"
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
        "task_unique_id INTEGER PRIMARY KEY NOT NULL,"
        "task_name CHAR(64) NOT NULL,"
        "task_description VARCHAR(1000) NOT NULL,"
        "user_unique_id SMALLINT NOT NULL,"
        "status_type_number TINYINT DEFAULT 1,"
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


    // ƒобавление начальных/тестовых значений в таблицу Users
    const char* insert_into_users = "INSERT INTO Users "
        "(user_unique_id, user_unique_login, first_name, second_name, patronymic) "
        "VALUES "
        "(1,'admin_test','Oleg','Ivanov','Ivanovich'), "
        "(2,'oper_db_test','Marina','Nikolaeva','Georgievna'), "
        "(3,'user_test','Jeffry','Bezos','Amazonovich');";

    result = sqlite3_exec(handle, insert_into_users, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table Users successfully updated" << std::endl;
    }
    else {
        std::cout << "An error occured while updation of DB table Users! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }
    
    // ƒобавление начальных/тестовых значений в таблицу Security
    const char* insert_into_security = "INSERT INTO Security "
        "(user_unique_id, user_type_number, password) "
        "VALUES "
        "(1,1,'qwerty123'), "
        "(2,2,'asdf4321'), "
        "(3,3,'password');";

    result = sqlite3_exec(handle, insert_into_security, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table Security successfully updated" << std::endl;
    }
    else {
        std::cout << "An error occured while updation of DB table Security! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    // ƒобавление начальных/тестовых значений в таблицу Tasks
    const char* insert_into_tasks = "INSERT INTO Tasks "
        "(task_unique_id, task_name, task_description, user_unique_id, status_type_number, deadline) "
        "VALUES "
        "(1,'Create operator','Create new data base operator',1,2,'2023-04-10'), "
        "(2,'Make reserv','Make a new reserv copy of database',2,2,'2023-04-11'), "
        "(3,'Have a rest','Choose a time period for vacation',3,2,'2023-04-14'), "
        "(4,'Repair printer','Buy paper and cartriges for office printer',3,2,'2023-04-15')";

    result = sqlite3_exec(handle, insert_into_tasks, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table Tasks successfully updated" << std::endl;
    }
    else {
        std::cout << "An error occured while updation of DB table Tasks! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    sqlite3_close(handle);


    return 0;
}
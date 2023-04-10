#include <iostream>

#include "sqlite3.h"

#include <boost/filesystem/operations.hpp>

#include "sql_create_requests.h"
#include "sql_insert_requests.h"
#include "sql_select_requests.h"

#ifdef WIN32
#define appname "db_creator.exe"
#else
#define appname "./db_creator"
#endif

bool execute_db_request(sqlite3* handle, const char* request)
{
    auto exec_callback = [](void*, int, char**, char**) -> int {
        /*for (int i = 0; i < columns; ++i) {
            std::cout << names[i] << " = " << (data[i] ? data[i] : "NULL") << std::endl;
        }

        std::cout << std::endl;*/

        return 0;
    };

    sqlite3_free(errmsg);
}

int main(int argc, char* argv[])
{
    // Нужно указать имя файла новой базы данных.
    if (argc != 2) {
        std::cout << "Usage: " << appname << " data_base_file_name" << std::endl;
        return 1;
    }

    std::string db_name{ argv[1] };
    if (!db_name.ends_with(".sqlite")) {
        db_name.append(".sqlite");
    }

    // Такой файл для БД не должен существовать. Старую базу не перезаписываем.
    if (boost::filesystem::exists(db_name)) {
        std::cout << "Such file for data base already exists. It will NOT be rewritten" << std::endl;
        return 1;
    }
    
    std::cout << "New data base will be created in file: " << db_name << std::endl;
    std::cout << " --------- " << std::endl;

    sqlite3* handle = nullptr;
     
    // Создание файл с базой данных.
    if (SQLITE_ERROR == sqlite3_open_v2(db_name.c_str(), &handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {
        std::cout << "Database creation error!" << std::endl;
        return 1;
    }

    if (handle == nullptr) {
        std::cout << "Unexpected error with database handle!" << std::endl;
        return 1;
    }

    char* errmsg = nullptr;

    // Callback функция для sqlite3_exec.
    // В рамках данной программы нам от неё каких-либо действий не требуется.
    //auto exec_callback = [](void*, int columns, char** data, char** names) -> int {
    auto exec_callback = [](void*, int , char** , char** ) -> int {
        /*for (int i = 0; i < columns; ++i) {
            std::cout << names[i] << " = " << (data[i] ? data[i] : "NULL") << std::endl;
        }

        std::cout << std::endl;*/

        return 0;
    };
        

    int result = SQLITE_OK;


    // 1 table - UserTypes
    // Эта таблица содержит названия типов пользователей.
    // Первый столбец - число-ключ. Второй столбец - текстовое описание типа пользователя.
    // Данные этой таблицы не меняются во время работы сервера.
    result = sqlite3_exec(handle, create_user_types, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table UserTypes successfully created" << std::endl;
    }
    else {
        std::cout << "An error occured while creation of DB table UserTypes! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    // Добавление постоянных значений в таблицу UserTypes
    result = sqlite3_exec(handle, insert_into_user_types, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table UserTypes successfully updated\n" << std::endl;
    }
    else {
        std::cout << "An error occured while updation of DB table TaskStatusTypes! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    

    // 2 table - Users
    // Данная таблица содержит всех пользователей, имеющих доступ к базе данных.
    // Изменять данную таблицу может только администратор.
    // Первый столбец - уникальный идентификатор пользователя. Используется в таблице Security.
    // Второй столбец - логин, который также должен быть уникален.
    // Третий столбец - имя, четвертый - фамилия, пятый - отчество.
     result = sqlite3_exec(handle, create_users, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table Users successfully created" << std::endl;
    }
    else {
        std::cout << "An error occured while creation of DB table Users! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    // Добавим в таблицу Users администратора, оператора базы данных и пользователя.
    result = sqlite3_exec(handle, insert_into_users, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table Users successfully updated\n" << std::endl;
    }
    else {
        std::cout << "An error occured while updation of DB table Users! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    

    // 3 table - Security
    // Данная таблица отвечает за безопасность доступа к данным и может быть изменена только администратором.
    // Используется при проверке пар логин/пароль и для получения типа пользователя для дальнейшей проверки его прав.
    // Первый столбец - уникальный идентификатор пользователя.
    // Второй столбец - число, соответствующее типу пользователя из таблицы UserTypes.
    // Третий столбец - пароль.
    result = sqlite3_exec(handle, create_security, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table Security successfully created" << std::endl;
    }
    else {
        std::cout << "An error occured while creation of DB table Security! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }
    
    // Обновим таблицу Security для трёх добавленных пользователей.
    result = sqlite3_exec(handle, insert_into_security, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table Security successfully updated\n" << std::endl;
    }
    else {
        std::cout << "An error occured while updation of DB table Security! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    // 4 table - TaskStatuses
    // Эта таблица содержит описание каждого из возможных состояний задачи.
    // Первый столбец - число-ключ. Второй столбец - текстовое описание состояния.
    // Данные этой таблицы не меняются во время работы сервера.
    result = sqlite3_exec(handle, create_task_statuses, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table TaskStatuses successfully created" << std::endl;
    }
    else {
        std::cout << "An error occured while creation of DB table TaskStatuses! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }

    // Добавление постоянных значений в таблицу TaskStatuses.
    result = sqlite3_exec(handle, insert_into_tss, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table TaskStatuses successfully updated\n" << std::endl;
    }
    else {
        std::cout << "An error occured while updation of DB table TaskStatusTypes! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }


    // 5 table - Tasks
    // Последняя таблица содержит задачи и их связи с пользователями.
    // Первый столбец - уникальный идентификатор задачи.
    // Второй столбец - название задачи.
    // Третий столбец - описание, что требуется сделать в рамках данной задачи.
    // Четвертый столбец - уникальный идентификатор пользователя.
    // Пятый столбец - число-идентификатор состояния задачи из таблицы TaskStatuses.
    // Последний столбец - крайний срок выполнения.
    result = sqlite3_exec(handle, create_tasks, exec_callback, 0, &errmsg);

    if (result == SQLITE_OK) {
        std::cout << "DB table Tasks successfully created" << std::endl;
    }
    else {
        std::cout << "An error occured while creation of DB table Tasks! Error code: " << result << std::endl;
        sqlite3_close(handle);
        return 1;
    }
    
    // Создадим несколько начальных задач в таблице Tasks.
    // Задача для администратора - создать ещё одного оператора БД.
    // Задача для оператора базы данных - сделать резервную копию базы.
    // И две офисных задачи для пользователя.
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
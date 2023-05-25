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
    // Нужно указать имя файла новой базы данных.
    if (argc != 2) {
        std::cout << "Usage: " << appname << " <data base file name>" << std::endl;
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
     
    // Создание файла с базой данных.
    if (SQLITE_ERROR == sqlite3_open_v2(db_name.c_str(), &handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {
        std::cout << "Database creation error!" << std::endl;
        return 1;
    }

    if (handle == nullptr) {
        std::cout << "Unexpected error with database handle!" << std::endl;
        return 1;
    }
 
    execute_simple_request(handle, "PRAGMA foreign_keys = ON");


    // - 1 - Создание таблицы UserTypes
    // Эта таблица содержит названия типов пользователей и её данные не меняются во время работы сервера.
    // Первый столбец - число-ключ. Второй столбец - текстовое описание типа пользователя.
    if (false == execute_request(handle, create_user_types, "UserTypes", true)) {
        sqlite3_close(handle);
        return 1;
    }

    // Добавление постоянных значений в таблицу UserTypes
    if (false == execute_request(handle, insert_into_user_types, "UserTypes", false)) {
        sqlite3_close(handle);
        return 1;
    }
        
    // - 2 - Создание таблицы Users
    // Данная таблица содержит всех пользователей, имеющих доступ к базе данных.
    // Изменять данную таблицу может только администратор.
    // Первый столбец - уникальный идентификатор пользователя. Используется в таблице Security.
    // Второй столбец - логин, который также должен быть уникален.
    // Третий столбец - имя, четвертый - фамилия, пятый - отчество.
    if (false == execute_request(handle, create_users, "Users", true)) {
        sqlite3_close(handle);
        return 1;
    }

    // Добавим в таблицу Users администратора, оператора базы данных и пользователя.
    if (false == execute_request(handle, insert_into_users, "Users", false)) {
        sqlite3_close(handle);
        return 1;
    }
   
    // - 3 - Создание таблицы Security
    // Данная таблица отвечает за безопасность доступа к данным и может быть изменена только администратором.
    // Используется при проверке пар логин/пароль и для получения типа пользователя для дальнейшей проверки его прав.
    // Первый столбец - уникальный идентификатор пользователя.
    // Второй столбец - число, соответствующее типу пользователя из таблицы UserTypes.
    // Третий столбец - пароль.
    if (false == execute_request(handle, create_security, "Security", true)) {
        sqlite3_close(handle);
        return 1;
    }
    
    // Обновим таблицу Security для трёх добавленных пользователей.
    if (false == execute_request(handle, insert_into_security, "Security", false)) {
        sqlite3_close(handle);
        return 1;
    }

    // - 4 - Создание таблицы TaskStatuses
    // Эта таблица содержит описание каждого из возможных состояний задачи и её данные не меняются во время работы сервера.
    // Первый столбец - число-ключ. Второй столбец - текстовое описание состояния.
    if (false == execute_request(handle, create_task_statuses, "TaskStatuses", true)) {
        sqlite3_close(handle);
        return 1;
    }

    // Добавление постоянных значений в таблицу TaskStatuses.
    if (false == execute_request(handle, insert_into_task_statuses, "TaskStatuses", false)) {
        sqlite3_close(handle);
        return 1;
    }


    // - 5 - Создание таблицы Tasks
    // Последняя таблица содержит задачи и их связи с пользователями.
    // Первый столбец - уникальный идентификатор задачи.
    // Второй столбец - название задачи.
    // Третий столбец - описание, что требуется сделать в рамках данной задачи.
    // Четвертый столбец - уникальный идентификатор пользователя.
    // Пятый столбец - число-идентификатор состояния задачи из таблицы TaskStatuses.
    // Последний столбец - крайний срок выполнения.
    if (false == execute_request(handle, create_tasks, "Tasks", true)) {
        sqlite3_close(handle);
        return 1;
    }
    
    // Создадим несколько начальных задач в таблице Tasks.
    // Задача для администратора - создать ещё одного оператора БД.
    // Задача для оператора базы данных - сделать резервную копию базы.
    // И две офисных задачи для пользователя.
    if (false == execute_request(handle, insert_into_tasks, "Tasks", false)) {
        sqlite3_close(handle);
        return 1;
    }

    std::cout << std::endl;

    // Вывод на экран содержимого созданных таблиц.
    

    // Содержимое таблицы UserTypes
    execute_request_output_columns(handle, "PRAGMA table_info(UserTypes)", "UserTypes");
    execute_request_output_data(handle, select_from_user_types, "UserTypes");

    // Содержимое таблицы Users
    execute_request_output_columns(handle, "PRAGMA table_info(Users)", "Users");
    execute_request_output_data(handle, select_from_users, "Users");

    // Содержимое таблицы Security
    execute_request_output_columns(handle, "PRAGMA table_info(Security)", "Security");
    execute_request_output_data(handle, select_from_security, "Security");

    // Содержимое таблицы TaskStatuses
    execute_request_output_columns(handle, "PRAGMA table_info(TaskStatuses)", "TaskStatuses");
    execute_request_output_data(handle, select_from_task_statuses, "TaskStatuses");

    // Содержимое таблицы Tasks
    execute_request_output_columns(handle, "PRAGMA table_info(Tasks)", "Tasks");
    execute_request_output_data(handle, select_from_tasks, "Tasks");

    // Закрытие базы данных.
    sqlite3_close(handle);


    return 0;
}
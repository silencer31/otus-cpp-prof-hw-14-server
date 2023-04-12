#pragma once

// Создание таблицы UserTypes
const char* create_user_types = "CREATE TABLE UserTypes ("
"user_type_number TINYINT PRIMARY KEY NOT NULL,"
"user_type_description CHAR(16) NOT NULL UNIQUE"
");";

// Создание таблицы Users
const char* create_users = "CREATE TABLE Users ("
"user_unique_id SMALLINT PRIMARY KEY,"
"user_unique_login CHAR(16) NOT NULL UNIQUE,"
"first_name CHAR(24) NOT NULL,"
"second_name CHAR(24) NOT NULL,"
"patronymic CHAR(24) NULL"
");";

// Создание таблицы Security
const char* create_security = "CREATE TABLE Security ("
"user_unique_id SMALLINT NOT NULL,"
"user_type_number TINYINT NOT NULL,"
"password CHAR(32) NOT NULL,"
"FOREIGN KEY (user_unique_id) REFERENCES Users (user_unique_id),"
"FOREIGN KEY (user_type_number) REFERENCES UserTypes (user_type_number)"
");";

// Создание таблицы TaskStatuses
const char* create_task_statuses = "CREATE TABLE TaskStatuses ("
"status_type_number TINYINT PRIMARY KEY,"
"status_type_description CHAR(16) NOT NULL UNIQUE"
");";

// Создание таблицы Tasks
const char* create_tasks = "CREATE TABLE Tasks ("
"task_unique_id INTEGER PRIMARY KEY NOT NULL,"
"user_unique_id SMALLINT NOT NULL REFERENCES Users (user_unique_id),"
"status_type_number TINYINT REFERENCES TaskStatuses (status_type_number),"
"deadline DATE NOT NULL,"
"task_name CHAR(64) NOT NULL,"
"task_description VARCHAR(1000) NOT NULL"
");";





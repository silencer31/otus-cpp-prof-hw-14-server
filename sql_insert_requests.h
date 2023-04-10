// ƒобавление посто€нных значений в таблицу UserTypes
const char* insert_into_user_types = "INSERT INTO UserTypes (user_type_number, user_type_description) "
"VALUES "
"(1,'administrator'), "
"(2,'db_operator'), "
"(3,'user');";

// ƒобавление в таблицу Users трЄх тестовых пользователей.
const char* insert_into_users = "INSERT INTO Users "
"(user_unique_id, user_unique_login, first_name, second_name, patronymic) "
"VALUES "
"(1,'admin_test','Oleg','Ivanov','Ivanovich'), "
"(2,'oper_db_test','Marina','Nikolaeva','Georgievna'), "
"(3,'user_test','Jeffry','Bezos','Amazonovich');";

// ƒобавление в таблицу Security пользователей из таблицы Users
const char* insert_into_security = "INSERT INTO Security "
"(user_unique_id, user_type_number, password) "
"VALUES "
"(1,1,'qwerty123'), "
"(2,2,'asdf4321'), "
"(3,3,'password');";

// ƒобавление посто€нных значений в таблицу TaskStatuses
const char* insert_into_tss = "INSERT INTO TaskStatuses (status_type_number, status_type_description) "
"VALUES "
"(1,'not appointed'), "
"(2,'appointed'), "
"(3,'in progress'), "
"(4,'rejected'), "
"(5,'completed');";

// ƒобавление 4 тестовых задач в таблицу Tasks
const char* insert_into_tasks = "INSERT INTO Tasks "
"(task_unique_id, task_name, task_description, user_unique_id, status_type_number, deadline) "
"VALUES "
"(1,'Create operator','Create a new data base operator',1,2,'2023-05-30'), "
"(2,'Make reserv','Make a new reserv copy of database',2,2,'2023-05-25'), "
"(3,'Have a rest','Choose a time period for vacation',3,2,'2023-05-14'), "
"(4,'Repair printer','Buy paper and cartriges for office printer',3,3,'2023-05-15')";
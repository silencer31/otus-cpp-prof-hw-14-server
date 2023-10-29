#pragma once

#include "data_storage.h"

enum class UserType {
	Administrator = 1,
	Operator = 2,
	User = 3
};

enum class TaskStatus {
	NotAppointed = 1,
	Appointed = 2,
	InProgress = 3,
	Rejected = 4,
	Completed = 5
};

class RequestManager {
public:
	RequestManager() = delete;

	explicit RequestManager(const char* file_path)
		: data_storage_ptr(std::make_unique<DataStorage>(file_path))
	{}

	~RequestManager() = default;

// ***** Получаем разные данные, связанные с пользователями *****

	// Узнать, есть ли такой логин в базе.
	bool check_login(const std::string& login);

	// Проверка пароля.
	bool check_password(const int user_id, const std::string& password);

	// Получить user_unique_id по логину пользователя. Уникальный идентификатор пользователя в базе.
	int get_user_id_by_login(const std::string& username);

	// Получить user_type_number по user_unique_id. Число, соответствующее типу пользователя.
	int get_user_type_by_user_id(const int user_id);

	// Текстовое название типа пользователя. 
	bool get_user_type_name_by_user_id(const int user_id, std::string& type_name);

	// Получить ФИО пользователя по user_unique_id.
	bool get_fio_by_user_id(const int user_id, vector_str& fio);

// ***** Получаем разные данные, связанные с задачами *****

	// Узнать status_type_number по task_unique_id. Число, соответствующее статусу задачи.
	int get_status_type_by_task_id(const int task_id);

	// Текстовое название состояния/статуса задачи.
	bool get_status_name_by_task_id(const int task_id, std::string& status_name);

	// Получить перечень идентификаторов задач для одного user_unique_id. Список задач одного пользователя. 
	bool get_task_ids_by_user_id(const int user_id, vector_int& task_ids);

	// Получить данные указанной задачи. Срок исполнения, название, описание.
	bool get_task_data_by_task_id(const int task_id, vector_str& task_data);

// ***** Добавляем в базу данных *****
	// Добавить нового пользователя.
	bool add_user(UserType user_type, const std::string& login,
		const std::string& sname, const std::string& fname, const std::string& patr);

	// Добавить новую задачу.
	bool add_task();

// ***** Удаляем из базы данных *****
	// Удалить пользователя.
	bool del_user();

	// Удалить задачу.
	bool del_task();

// ***** Изменяем значения в базе *****
	// Установить новый пароль.
	bool set_password();

	// Изменить тип пользователя.
	bool set_user_type();

	// Изменить статус задачи.
	bool set_task_status();

private: // methods
	// Узнать пароль пользователя по user_unique_id.
	bool get_password_by_user_id(const int user_id, std::string& password);

private: // data
	const storage_unique data_storage_ptr;
};

using request_manager_shared = std::shared_ptr<RequestManager>;
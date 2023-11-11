#pragma once

#include "data_storage.h"

enum class UserType {
	Unknown = 0,
	Administrator = 1,
	Operator = 2,
	User = 3
};

enum class TaskStatus {
	Unknown = 0,
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

	void lock_access() {
		data_storage_ptr->lock_db();
	}

	void free_access() {
		data_storage_ptr->lock_db();
	}

// ***** Получаем разные данные, связанные с пользователями *****

	// Узнать, есть ли такой логин в базе.
	bool login_present(const std::string& login);

	// Проверка пароля.
	bool check_password(const int user_id, const std::string& password);

	// Узнать наибольший id пользователя.
	int get_user_id_max_number();

	// Получить user_unique_id по логину пользователя. Уникальный идентификатор пользователя в базе.
	int get_user_id_by_login(const std::string& username);

	// Получить user_type_number по user_unique_id. Число, соответствующее типу пользователя.
	int get_user_type_by_user_id(const int user_id);

	// Текстовое название типа пользователя. 
	bool get_user_type_name_by_user_id(const int user_id, std::string& type_name);

	// Получить тип пользователя из числа.
	UserType get_user_type_from_int(const int user_type);

	// Получить ФИО пользователя по user_unique_id.
	bool get_fio_by_user_id(const int user_id, vector_str& fio);

	// Получить список всех id пользователей.
	int get_user_id_list(vector_int& id_list);

	// Получить список номеров типов пользователя с описанием. 
	int get_type_description_lists(vector_int& numbers, vector_str& descriptions);

// ***** Получаем разные данные, связанные с задачами *****

	// Узнать наибольший id задач.
	int get_task_id_max_number();

	// Узнать status_type_number по task_unique_id. Число, соответствующее статусу задачи.
	int get_status_type_by_task_id(const int task_id);

	// Получить статус задачи из числа.
	TaskStatus get_status_type_from_int(const int status_type);

	// Текстовое название состояния/статуса задачи.
	bool get_status_name_by_task_id(const int task_id, std::string& status_name);

	// Получить перечень идентификаторов задач для одного user_unique_id. Список задач одного пользователя. 
	int get_task_ids_by_user_id(const int user_id, vector_int& task_ids);

	// Получить данные указанной задачи. id пользователя и статус. Срок исполнения, название, описание.
	bool get_task_data_by_task_id(const int task_id, vector_int& user_and_status, vector_str& time_name_description);

	// Получить список всех id задач.
	int get_task_id_list(vector_int& id_list);

	// Получить список номеров статусов задач с описанием. 
	int get_status_description_lists(vector_int& numbers, vector_str& descriptions);

// ***** Добавляем в базу данных *****
	// Добавить нового пользователя.
	bool add_user(UserType user_type, const std::string& login, const std::string& password,
		const std::string& sname, const std::string& fname, const std::string& patr);

	// Добавить новую задачу.
	bool add_task(const int user_id, const std::string& deadline,
		const std::string& name, const std::string& description);

// ***** Удаляем из базы данных *****
	// Удалить пользователя.
	bool del_user(const int user_id);

	// Удалить задачу.
	bool del_task(const int task_id);

// ***** Изменяем значения в базе *****
	// Установить новый пароль.
	bool set_password(const int user_id, const std::string& password);

	// Изменить тип пользователя.
	bool set_user_type(const int user_id, UserType user_type);

	// Изменить статус задачи.
	bool set_task_status(const int task_id, TaskStatus task_status);

private: // methods
	// Узнать пароль пользователя по user_unique_id.
	bool get_password_by_user_id(const int user_id, std::string& password);

private: // data
	const storage_unique data_storage_ptr;
};

using request_manager_shared = std::shared_ptr<RequestManager>;
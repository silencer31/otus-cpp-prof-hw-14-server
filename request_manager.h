#pragma once

#include "data_storage.h"

/**
* @brief Перечисление - возможные типы пользователей.
*/
enum class UserType {
	Unknown = 0,
	Administrator = 1,
	Operator = 2,
	User = 3
};

/**
* @brief Перечисление - возможные статусы задач.
*/
enum class TaskStatus {
	Unknown = 0,
	NotAppointed = 1,
	Appointed = 2,
	InProgress = 3,
	Rejected = 4,
	Completed = 5
};

/**
* @brief Класс - менеджер запросов к базе данных.
*/
class RequestManager {
public:
	RequestManager() = delete;

	explicit RequestManager(const char* file_path)
		: data_storage_ptr(std::make_unique<DataStorage>(file_path))
	{}

	~RequestManager() = default;

	/**
	* Получить владение доступом к базе данных.
	*/
	void lock_access() {
		data_storage_ptr->lock_db();
	}

	/**
	* Освободить владение доступом к базе данных.
	*/
	void free_access() {
		data_storage_ptr->unlock_db();
	}

// ***** Получаем разные данные, связанные с пользователями *****

	/**
	* Узнать, есть ли такой логин в базе.
	* @param login логин пользователя.
	*/
	bool login_present(const std::string& login);
 
	/**
	* Проверка пароля.
	* @param user_id уникальный id пользователя.
	* @param password пароль для проверки.
	*/
	bool check_password(const int user_id, const std::string& password);

	/**
	* Узнать число - наибольший id пользователя в базе.
	*/
	int get_user_id_max_number();

	/**
	* Получить уникальный id пользователя по логину пользователя.
	* @param username логин пользователя.
	*/
	int get_user_id_by_login(const std::string& username);

	/**
	* Получить число, соответствующее типу пользователя по id пользователя.
	* @param user_id уникальный id пользователя.
	*/
	int get_user_type_by_user_id(const int user_id);

	/**
	* Узнать логин по id пользователя.
	* @param user_id уникальный id пользователя.
	* @param username параметр для записи значения - логина.
	*/
	bool get_login_by_user_id(const int user_id, std::string& username);

	/**
	* Получить текстовое название типа пользователя по id пользователя. 
	* @param user_id уникальный id пользователя.
	* @param type_name параметр для записи значения - название типа пользователя.
	*/
	bool get_user_type_name_by_user_id(const int user_id, std::string& type_name);

	/**
	* Получить тип пользователя из числа.
	* @param user_type число - номер типа пользователя.
	*/
	UserType get_user_type_from_int(const int user_type);

	/**
	* Получить ФИО пользователя по user_unique_id.
	* @param user_id уникальный id пользователя.
	* @param fio параметр для записи значений - ФИО пользователя.
	*/
	bool get_fio_by_user_id(const int user_id, vector_str& fio);

	/**
	* Получить список всех id пользователей.
	* @param id_list параметр для записи значений - всех id пользователей.
	*/
	int get_user_id_list(vector_int& id_list);

	/**
	* Получить список номеров типов пользователя и список с текстовым описанием.
	* @param numbers параметр для записи значений - номеров типов пользователя.
	* @param descriptions параметр для записи значений - список с текстовым описанием.
	*/
	int get_type_description_lists(vector_int& numbers, vector_str& descriptions);

// ***** Получаем разные данные, связанные с задачами *****
 
	/**
	* Узнать число - наибольший id задач в базе.
	*/
	int get_task_id_max_number();

	/**
	* Узнать число, соответствующее статусу задачи по id задачи.
	* @param task_id уникальный id задачи.
	*/
	int get_status_type_by_task_id(const int task_id);

	/**
	* Получить статус задачи из числа.
	* @param status_type число, соответствующее статусу задачи.
	*/
	TaskStatus get_status_type_from_int(const int status_type);

	/**
	* Получить текстовое название статуса задачи.
	* @param task_id уникальный id задачи.
	* @param status_name параметр для записи значения - текстового описания статуса задачи.
	*/
	bool get_status_name_by_task_id(const int task_id, std::string& status_name);

	/**
	* Получить id исполнителя задачи по id задачи.
	* @param task_id уникальный id задачи.
	*/
	int get_task_user_id_by_task_id(const int task_id);

	/**
	* Получить список из id задач, которые назначены на пользователя с переданным user_id.
	* @param user_id уникальный id пользователя.
	* @param task_ids параметр для записи значений - списка id задач.
	*/
	int get_task_ids_by_user_id(const int user_id, vector_int& task_ids);

	/**
	* Получить данные указанной задачи. id пользователя и статус. Срок исполнения, название, описание.
	* @param task_id уникальный id задачи.
	* @param user_and_status параметр для записи значений - исполнителя и текущего статуса.
	* @param time_name_description параметр для записи значений - срок исполнения, название и описание задачи.
	*/
	bool get_task_data_by_task_id(const int task_id, vector_int& user_and_status, vector_str& time_name_description);

	/**
	* Получить список всех id задач.
	* @param параметр для записи значений - список id всех задач.
	*/
	int get_task_id_list(vector_int& id_list);

	/**
	* Получить список номеров статусов задач с описанием.
	* @param numbers параметр для записи значений - номера статусов задач.
	* @param descriptions параметр для записи значений - текстовые описания статусов задач.
	*/
	int get_status_description_lists(vector_int& numbers, vector_str& descriptions);

// ***** Добавляем в базу данных *****
 
	/**
	* Добавить нового пользователя.
	* @param
	*/
	int add_user(UserType user_type, const std::string& login, const std::string& password,
		const std::string& sname, const std::string& fname, const std::string& patr);

	/**
	* Добавить новую задачу.
	* @param user_id уникальный id пользователя.
	* @param deadline срок исполнения задачи.
	* @param name название задачи.
	* @param description описание задачи.
	*/
	int add_task(const int user_id, const std::string& deadline,
		const std::string& name, const std::string& description);

// ***** Удаляем из базы данных *****

	/**
	* Удалить пользователя.
	* @param user_id уникальный id пользователя.
	*/
	bool del_user(const int user_id);

	/**
	* Удалить задачу.
	* @param task_id уникальный id задачи.
	*/
	bool del_task(const int task_id);

// ***** Изменяем значения в базе *****

	/**
	* Установить новый пароль.
	* @param user_id уникальный id пользователя.
	* @param password новый пароль.
	*/
	bool set_password(const int user_id, const std::string& password);
 
	/**
	* Изменить тип пользователя.
	* @param user_id уникальный id пользователя.
	* @param user_type новый тип пользователя.
	*/
	bool set_user_type(const int user_id, UserType user_type);

	/**
	* Изменить статус задачи.
	* @param task_id уникальный id задачи.
	* @param task_status новый статус задачи.
	*/
	bool set_task_status(const int task_id, TaskStatus task_status);

	/**
	* Установить исполнителя и назначить статус "appointed".
	* @param task_id уникальный id задачи.
	* @param user_id уникальный id пользователя - нового исполнителя.
	*/
	bool set_task_appointed(const int task_id, const int user_id);

	/**
	* Сбросить исполнителя задачи.
	* @param task_id уникальный id задачи.
	*/
	bool set_task_not_appointed(const int task_id);

	/**
	* Изменить исполнителя задачи.
	* @param task_id уникальный id задачи.
	* @param user_id уникальный id пользователя - нового исполнителя.
	*/
	bool set_task_user(const int task_id, const int user_id);

	/**
	* Изменить deadline задачи.
	* @param task_id уникальный id задачи.
	* @param dl_value новое значение срока испонения задачи.
	*/
	bool set_task_deadline(const int task_id, const std::string& dl_value);

private: // methods
 
	/**
	* Узнать пароль пользователя по user_unique_id.
	* @param user_id уникальный id пользователя.
	* @param password параметр для получения значения пароля.
	*/
	bool get_password_by_user_id(const int user_id, std::string& password);

private: // data
	const storage_unique data_storage_ptr;
};

using request_manager_shared = std::shared_ptr<RequestManager>;
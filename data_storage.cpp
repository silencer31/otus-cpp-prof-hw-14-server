#include "data_storage.h"

#include "sql_select_requests.h"

#include <iostream>

bool DataStorage::init_database()
{

    if (SQLITE_OK != sqlite3_open_v2(db_path, &handle, SQLITE_OPEN_READWRITE, NULL))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(handle) << std::endl;
        sqlite3_close(handle);
        return false;
    }

    std::cout << db_path << " database opened successfully!" << std::endl;

    bool res = check_login("user_test", "pass");

    return res;
}

bool DataStorage::handle_request(const std::string& request)
{
    auto exec_callback = [](void*, int columns, char** data, char** names) -> int {
        for (int i = 0; i < columns; ++i) {
            std::cout << names[i] << " = " << (data[i] ? data[i] : "NULL") << std::endl;
        }
            
        std::cout << std::endl;

        return 0;
    };

    char* errmsg;
    
    data_mutex.lock();
    int rc = sqlite3_exec(handle, request.c_str(), exec_callback, 0, &errmsg);
    data_mutex.unlock();
        
    if (rc != SQLITE_OK)
    {
        std::cerr << "Can't execute query: " << errmsg << std::endl;
        sqlite3_free(errmsg);
        return false;
    }
    
    return true;
}

// ��������� ������ � ���� ������ � ��������� ������.
bool DataStorage::execute_db_request(const std::string& request, char** err_ptr)
{
    auto exec_callback = [](void*, int, char**, char**) -> int {
        return 0;
    };    

    data_mutex.lock();

    int result = sqlite3_exec(handle, request.c_str(), exec_callback, 0, &(*err_ptr));
    
    data_mutex.unlock();

    return (result == SQLITE_OK);
}
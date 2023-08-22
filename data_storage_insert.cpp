#include "data_storage.h"

#include <iostream>

// ƒобавить в таблицу новую строку со значени€ми.
bool DataStorage::insert_values(const std::string& table, const std::string& values)
{
    const std::string request = "INSERT INTO  " + table
        + " VALUES " + values + ";";


    auto exec_callback = [](void*, int, char**, char**) -> int {
        return 0;
    };

    char* errmsg = nullptr;

    if (SQLITE_OK == sqlite3_exec(handle, request.c_str(), exec_callback, 0, &errmsg)) {
        return true;
    }

    std::cout << "Table " << table << " INSERT INTO error: " << errmsg << std::endl;

    if (errmsg) {
        sqlite3_free(errmsg);
    }

    return false;
}
#include "data_storage.h"

#include <iostream>

// Обновить значение в столбце таблицы с соблюдением условия.
bool DataStorage::set_value_with_clause(const std::string& table,
	const std::string& target_col, const std::string& new_value,
	const std::string& clause_col, const std::string& clause_val)
{
    const std::string request = "UPDATE " + table
        + " SET " + target_col + " = " + new_value
        + " WHERE " + clause_col + " = " + clause_val;

    auto exec_callback = [](void*, int, char**, char**) -> int {
        return 0;
    };

    char* errmsg = nullptr;

    if (SQLITE_OK == sqlite3_exec(handle, request.c_str(), exec_callback, 0, &errmsg)) {
        return true;
    }

    std::cout << "Table " << table << " UPDATE error: " << errmsg << std::endl;
    
    if (errmsg) {
        sqlite3_free(errmsg);
    }

    return false;
}
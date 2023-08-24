#include "data_storage.h"

#include <iostream>

// Обновить значение в столбце таблицы с соблюдением условия WHERE.
bool DataStorage::set_value_with_clause(const std::string& table,
	const std::string& target_col, const std::string& new_value,
	const std::string& clause_col, const std::string& clause_val)
{
    const std::string request = "UPDATE " + table
        + " SET " + target_col + " = " + new_value
        + " WHERE " + clause_col + " = " + clause_val;  

    char* errmsg = nullptr;

    if (execute_db_request(request, &errmsg)) {
        return true;
    }

    std::cout << "Table " << table << " UPDATE error: " << errmsg << std::endl;
    
    if (errmsg) {
        sqlite3_free(errmsg);
    }

    return false;
}
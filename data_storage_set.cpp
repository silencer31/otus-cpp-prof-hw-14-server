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

    //std::cout << "req: " << request << std::endl;
    
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

// Обновить значения в столбцах таблицы с соблюдением условия.
bool DataStorage::set_several_values_with_clause(const std::string& table,
    const vector_str& target_columns, const vector_str& values,
    const std::string& clause_col, const std::string& clause_val)
{
    // SET column_name1= value1, column_name2= value2
    std::string columns_and_values_str;

    for (size_t i = 0; i < target_columns.size(); ++i) {
        columns_and_values_str.append(columns_and_values_str.empty()
            ? (target_columns.at(i) + "=" + values.at(i))
            : (", " + (target_columns.at(i) + "=" + values.at(i))));
    }

    const std::string request = "UPDATE " + table
        + " SET " + columns_and_values_str
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

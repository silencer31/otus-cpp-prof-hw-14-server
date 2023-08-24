#include "data_storage.h"

#include <iostream>

// Удалить из таблицы строку с указанием столбца и значения в нём.
bool DataStorage::delete_row(const std::string& table, const std::string& column, const std::string& value)
{
    const std::string request = "DELETE FROM  " + table
        + " WHERE " + column + "=" + value + ";";

    char* errmsg = nullptr;

    if (execute_db_request(request, &errmsg)) {
        return true;
    }

    std::cout << "Table " << table << " DELETE FROM: " << errmsg << std::endl;

    if (errmsg) {
        sqlite3_free(errmsg);
    }

    return false;
}

// Удалить из таблицы строки с указанием столбца и значений в нём.
bool DataStorage::delete_rows(const std::string& table, const std::string& column, const vector_str& values)
{
    std::string values_str;

    for (vector_str::const_iterator row_iter = values.cbegin(); row_iter != values.cend(); ++row_iter) {
        values_str.append(values_str.empty() ? (*row_iter) : (", " + (*row_iter)));
    }

    const std::string request = "DELETE FROM  " + table
        + " WHERE " + column + "IN (" + values_str + ");";

    char* errmsg = nullptr;

    if (execute_db_request(request, &errmsg)) {
        return true;
    }

    std::cout << "Table " << table << " DELETE FROM: " << errmsg << std::endl;

    if (errmsg) {
        sqlite3_free(errmsg);
    }

    return false;
}
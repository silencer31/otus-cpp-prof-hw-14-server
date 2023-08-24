#include "data_storage.h"

#include <iostream>

// Добавить в таблицу новую строку со значениями.
bool DataStorage::insert_row(const std::string& table, const std::string& values)
{
    const std::string request = "INSERT INTO  " + table
        + " VALUES " + values + ";";

    char* errmsg = nullptr;

    if (execute_db_request(request, &errmsg)) {
        return true;
    }

    std::cout << "Table " << table << " INSERT INTO error: " << errmsg << std::endl;

    if (errmsg) {
        sqlite3_free(errmsg);
    }

    return false;
}

// Добавить в таблицу несколько новых строк со значениями.
bool DataStorage::insert_rows(const std::string& table, const vector_str& rows)
{
    std::string values_str;

    for (vector_str::const_iterator row_iter = rows.cbegin(); row_iter != rows.cend(); ++row_iter) {
        values_str.append(values_str.empty() ? (*row_iter) : (", " + (*row_iter)));
    }

    const std::string request = "INSERT INTO  " + table
        + " VALUES " + values_str + ";";

    char* errmsg = nullptr;

    if (execute_db_request(request, &errmsg)) {
        return true;
    }

    std::cout << "Table " << table << " INSERT INTO error: " << errmsg << std::endl;

    if (errmsg) {
        sqlite3_free(errmsg);
    }

    return false;
}

// Добавить в таблицу новую строку со значениями для указанных столбцов.
bool DataStorage::insert_row_for_columns(const std::string& table, const vector_str& columns,
    const std::string& values)
{
    std::string columns_str;

    for (vector_str::const_iterator row_iter = columns.cbegin(); row_iter != columns.cend(); ++row_iter) {
        columns_str.append( columns_str.empty() ? (*row_iter) : (", " + (*row_iter)) );
    }

    const std::string request = "INSERT INTO  " + table + " (" + columns_str + ") "
        + " VALUES " + values + ";";

    char* errmsg = nullptr;

    if (execute_db_request(request, &errmsg)) {
        return true;
    }

    std::cout << "Table " << table << " INSERT INTO error: " << errmsg << std::endl;

    if (errmsg) {
        sqlite3_free(errmsg);
    }

    return false;
}

// Добавить в таблицу несколько новых строк со значениями для указанных столбцов.
bool DataStorage::insert_rows_for_columns(const std::string& table, const vector_str& columns,
    const vector_str& rows)
{
    std::string columns_str;

    for (vector_str::const_iterator row_iter = columns.cbegin(); row_iter != columns.cend(); ++row_iter) {
        columns_str.append(columns_str.empty() ? (*row_iter) : (", " + (*row_iter)));
    }

    std::string values_str;

    for (vector_str::const_iterator row_iter = rows.cbegin(); row_iter != rows.cend(); ++row_iter) {
        values_str.append(values_str.empty() ? (*row_iter) : (", " + (*row_iter)));
    }

    const std::string request = "INSERT INTO  " + table + " (" + columns_str + ") "
        + " VALUES " + values_str + ";";

    char* errmsg = nullptr;

    if (execute_db_request(request, &errmsg)) {
        return true;
    }

    std::cout << "Table " << table << " INSERT INTO error: " << errmsg << std::endl;

    if (errmsg) {
        sqlite3_free(errmsg);
    }

    return false;
}
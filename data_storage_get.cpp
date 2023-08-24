#include "data_storage.h"

#include <iostream>

// Получить значение-число из определённой ячейки в таблице.
// Кроме имени столбца, в функцию передаётся номер строки.
bool DataStorage::get_row_value_int(const std::string& table, const std::string& col, const int row,
    int& value)
{
    const std::string request = "SELECT " + col + " FROM " + table;

    int current_row = 0;
    bool value_found = false;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (current_row == row) {
            value = sqlite3_column_int(stmt, 0);
            value_found = true;
            break;
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return value_found;
}

// Получить значение-текст из определённой ячейки в таблице.
// Кроме имени столбца, в функцию передаётся номер строки.
bool DataStorage::get_row_value_txt(const std::string& table, const std::string& col, const int row,
    std::string& value)
{
    const std::string request = "SELECT " + col + " FROM " + table;
    int current_row = 0;
    bool value_found = false;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (current_row == row) {
            value = (char*)sqlite3_column_text(stmt, 0);
            value_found = true;
            break;
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return value_found;
}

// Ищем строку, содержащую переданное значение-текст в указанном столбце.
// Если находим, возвращаем значение-число из той же строки, но из второго указанного столбца.
bool DataStorage::get_corresp_col_value_int(const std::string& table,
    const std::string& find_col, const std::string& target_col,
    const std::string& find_value, int& get_value)
{
    const std::string request = "SELECT " + find_col + ", " + target_col + " FROM " + table;
    bool value_found = false;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (find_value == (char*)sqlite3_column_text(stmt, 0)) {
            get_value = sqlite3_column_int(stmt, 1);
            value_found = true;
            break;
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return value_found;
}

// Ищем строку, содержащую переданное значение-текст в указанном столбце.
// Если находим, возвращаем значение-текст из той же строки, но из второго указанного столбца.
bool DataStorage::get_corresp_col_value_txt(const std::string& table,
    const std::string& find_col, const std::string& target_col,
    const std::string& find_value, std::string& get_value)
{
    const std::string request = "SELECT " + find_col + ", " + target_col + " FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    bool value_found = false;

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (find_value == (char*)sqlite3_column_text(stmt, 0)) {
            get_value = (char*)sqlite3_column_text(stmt, 1);
            value_found = true;
            break;
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return value_found;
}
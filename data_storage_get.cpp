#include "data_storage.h"

#include <iostream>

// Получить максимальное числовое значение из указанного столбца.
bool DataStorage::get_column_max_int_value(const std::string& table, const std::string& col, int& max_value)
{
    const std::string request = "SELECT " + col + " FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    int current_row = 0;
    int value = 0;

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        value = sqlite3_column_int(stmt, 0);

        if (current_row == 0) {
            max_value = value;
        }
        else {
            if (value > max_value) { max_value = value; }
        }

        ++current_row;
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return true;
}

// Узнать, есть ли переданное строковое значение в указанном столбце таблицы.
bool DataStorage::get_txt_value_presence(const std::string& table, const std::string& col, const std::string& find_value)
{
    const std::string request = "SELECT " + col + " FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    bool value_found = false;

    // Значения из целевого столбца добавляем в вектор чисел.
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (find_value == (char*)sqlite3_column_text(stmt, 0)) {
            value_found = true;
            break;
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return value_found;
}

// Получить значение-число из определённой ячейки в таблице.
// Кроме имени столбца, в функцию передаётся номер строки.
bool DataStorage::get_row_value_int(const std::string& table, const std::string& col, const int row,
    int& value)
{
    const std::string request = "SELECT " + col + " FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    int current_row = 0;
    bool value_found = false;

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (current_row == row) {
            value = sqlite3_column_int(stmt, 0);
            value_found = true;
            break;
        }

        ++current_row;
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
    
    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    int current_row = 0;
    bool value_found = false;

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (current_row == row) {
            value = (char*)sqlite3_column_text(stmt, 0);
            value_found = true;
            break;
        }

        ++current_row;
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return value_found;
}

// Получить все значения-числа из указанного столбца.
int DataStorage::get_column_values_int(const std::string& table, const std::string& col, vector_int& values)
{
    const std::string request = "SELECT " + col + " FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return 0;
    }
   
    int number = 0;

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        values.push_back(sqlite3_column_int(stmt, 0));
        ++number;
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return number;
}

// Получить все значения-строки из указанного столбца.
int DataStorage::get_column_values_txt(const std::string& table, const std::string& col, vector_str& values)
{
    const std::string request = "SELECT " + col + " FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return 0;
    }

    int number = 0;

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        values.push_back((char*)sqlite3_column_text(stmt, 0));
        ++number;
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return number;
}

// Получить все значения-числа из столбца num_col и значения-строки из столбца txt_col.
int DataStorage::get_pairs_int_txt(const std::string& table, const std::string& num_col, const std::string& txt_col,
    vector_int& numbers, vector_str& strings)
{
    const std::string request = "SELECT " + num_col + ", " + txt_col + " FROM " + table;

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        return false;
    }

    int number = 0;

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        numbers.push_back(sqlite3_column_int(stmt, 0));
        strings.push_back((char*)sqlite3_column_text(stmt, 1));
        ++number;
    }

    sqlite3_finalize(stmt);

    return number;
}

// Ищем строку, содержащую переданное значение-текст в указанном столбце.
// Если находим, возвращаем значение-число из той же строки, но из второго указанного столбца.
bool DataStorage::get_corresp_col_value_int(const std::string& table,
    const std::string& find_col, const std::string& target_col,
    const std::string& find_value, int& get_value)
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
            get_value = sqlite3_column_int(stmt, 1);
            value_found = true;
            break;
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return value_found;
}

// Узнать, сколько раз значение встречается в указанном столбце указанной таблицы.
int DataStorage::get_occurances_number(const std::string& table, const std::string& find_col, const std::string& find_value)
{
    const std::string request = "SELECT " + find_col + " FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return 0;
    }

    int number = 0;

    // Значения из целевого столбца добавляем в вектор чисел.
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (find_value == (char*)sqlite3_column_text(stmt, 0)) {
            ++number;
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return number;
}

// Перебираем все строки, содержащие переданное значение в указанном столбце.
// Если находим, берём значение-число из той же строки, но из второго указанного столбца и добавляем его в вектор чисел.
void DataStorage::get_corresp_col_int_vector(const std::string& table,
    const std::string& find_col, const std::string& target_col,
    const std::string& find_value, vector_int& values)
{
    const std::string request = "SELECT " + find_col + ", " + target_col + " FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return;
    }

    // Значения из целевого столбца добавляем в вектор чисел.
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (find_value == (char*)sqlite3_column_text(stmt, 0)) {
            values.push_back( sqlite3_column_int(stmt, 1));
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();
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

// Ищем строку, содержащую переданное значение в указанном столбце.
// Собираем значения из переданных столбцов в той же строке.
bool DataStorage::get_corresp_columns_txt_values(const std::string& table,
    const std::string& find_col, const vector_str& target_columns,
    const std::string& find_value, vector_str& values)
{
    std::string columns;

    for (auto iter = target_columns.cbegin(); iter != target_columns.cend(); ++iter) {
        columns += ((iter == (target_columns.cend()-1)) ? (*iter) : (*iter + ", "));
    }

    const std::string request = "SELECT " + find_col + ", " + columns + " FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (find_value == (char*)sqlite3_column_text(stmt, 0)) {
            for (int i = 0; i < target_columns.size(); ++i) {
                values.push_back((char*)sqlite3_column_text(stmt, i+1));
            }
            
            break;
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return !values.empty();
}
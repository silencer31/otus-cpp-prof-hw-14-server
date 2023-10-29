#include "data_storage.h"

#include <iostream>

// ������ ���-�� �������/����� � �������.
int DataStorage::get_records_number(const std::string& table)
{
    const std::string request = "SELECT COUNT(*) FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    bool value_found = false;

    // �������� �� �������� ������� ��������� � ������ �����.
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

// ������, ���� �� ���������� ��������� �������� � ��������� ������� �������.
bool DataStorage::get_txt_value_presence(const std::string& table, const std::string& col, std::string& find_value)
{
    const std::string request = "SELECT " + col + " FROM " + table;

    data_mutex.lock();

    if (SQLITE_OK != sqlite3_prepare_v2(handle, request.c_str(), -1, &stmt, nullptr)) {
        std::cout << "sqlite prepare error " << sqlite3_errmsg(handle) << std::endl;
        data_mutex.unlock();
        return false;
    }

    bool value_found = false;

    // �������� �� �������� ������� ��������� � ������ �����.
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

// �������� ��������-����� �� ����������� ������ � �������.
// ����� ����� �������, � ������� ��������� ����� ������.
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
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return value_found;
}

// �������� ��������-����� �� ����������� ������ � �������.
// ����� ����� �������, � ������� ��������� ����� ������.
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
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return value_found;
}

// ���� ������, ���������� ���������� ��������-����� � ��������� �������.
// ���� �������, ���������� ��������-����� �� ��� �� ������, �� �� ������� ���������� �������.
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

// ������, ������� ��� �������� ����������� � ��������� ������� ��������� �������.
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

    // �������� �� �������� ������� ��������� � ������ �����.
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (find_value == (char*)sqlite3_column_text(stmt, 0)) {
            ++number;
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();

    return number;
}

// ���������� ��� ������, ���������� ���������� �������� � ��������� �������.
// ���� �������, ���� ��������-����� �� ��� �� ������, �� �� ������� ���������� ������� � ��������� ��� � ������ �����.
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

    // �������� �� �������� ������� ��������� � ������ �����.
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        if (find_value == (char*)sqlite3_column_text(stmt, 0)) {
            values.push_back( sqlite3_column_int(stmt, 1));
        }
    }

    sqlite3_finalize(stmt);

    data_mutex.unlock();
}

// ���� ������, ���������� ���������� ��������-����� � ��������� �������.
// ���� �������, ���������� ��������-����� �� ��� �� ������, �� �� ������� ���������� �������.
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

// ���� ������, ���������� ���������� �������� � ��������� �������.
// �������� �������� �� ���������� �������� � ��� �� ������.
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
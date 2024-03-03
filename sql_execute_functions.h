#pragma once

#include "sqlite3.h"
/**
* ���������� ������� � ���� ������ � ������� �������� � �������.
* @param handle ����� � ����� ������.
* @param request sql ������ � ����.
* @param table_name ��� ���� ������.
*/
void execute_request_output_columns(sqlite3* handle, const char* request, const char* table_name);

/**
* ���������� ������� � ���� ������ � ������� � �������.
* @param handle ����� � ����� ������.
* @param request sql ������ � ����.
* @param table_name ��� ���� ������.
*/
void execute_request_output_data(sqlite3* handle, const char* request, const char* table_name);

/**
* ���������� ������� � ���� ������.
* @param handle ����� � ����� ������.
* @param request sql ������ � ����.
* @param table_name ��� ���� ������.
* @param create_flag �������� �� ����� ������� ��� ����������� ������������.
*/
bool execute_request(sqlite3* handle, const char* request, const char* table_name, bool create_flag);

/**
* ���������� ������� � ���� ������.
* @param handle ����� � ����� ������.
* @param request sql ������ � ����.
*/
bool execute_simple_request(sqlite3* handle, const char* request);
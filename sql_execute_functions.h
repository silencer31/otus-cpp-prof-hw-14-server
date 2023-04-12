#pragma once

#include "sqlite3.h"

void execute_request_output_columns(sqlite3* handle, const char* request, const char* table_name);

void execute_request_output_data(sqlite3* handle, const char* request, const char* table_name);

bool execute_request(sqlite3* handle, const char* request, const char* table_name, bool create_flag);

bool execute_simple_request(sqlite3* handle, const char* request);
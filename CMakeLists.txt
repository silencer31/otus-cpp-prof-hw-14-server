cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(task_server VERSION ${PROJECT_VERSION})

find_package(Boost COMPONENTS REQUIRED system filesystem)
find_package(Threads)

# see https://www.sqlite.org/howtocompile.html
include(FetchContent)
FetchContent_Declare(sqlite3 URL "https://www.sqlite.org/snapshot/sqlite-snapshot-202401231504.tar.gz")
FetchContent_MakeAvailable(sqlite3)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

if (NOT sqlite3_POPULATED)
  FetchContent_Populate(sqlite3)
endif()

include_directories(${Boost_INCLUDE_DIR})

add_definitions(-DBOOST_ALL_NO_LIB) # Without this string boost filesystem will not compile. 

configure_file(version.h.in version.h)

#Main target 
add_executable(task_server main.cpp 
client_session.cpp client_session_handle.cpp client_session_reply.cpp client_session_reply_error.cpp
client_session_add_data.cpp client_session_del_data.cpp client_session_get_data.cpp client_session_set_data.cpp
task_server.cpp
data_storage.cpp data_storage_get.cpp data_storage_set.cpp data_storage_insert.cpp data_storage_delete.cpp
request_manager.cpp request_manager_get_users.cpp request_manager_set_users.cpp request_manager_get_tasks.cpp request_manager_set_tasks.cpp request_manager_add_del.cpp
"${sqlite3_SOURCE_DIR}/sqlite3.c")

add_executable(db_creator db_creator.cpp sql_execute_functions.cpp "${sqlite3_SOURCE_DIR}/sqlite3.c")

add_library(lib_task_server lib.cpp)

set_target_properties(task_server db_creator lib_task_server PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(task_server 
	PUBLIC "${sqlite3_SOURCE_DIR}"	
)

target_include_directories(db_creator 
	PUBLIC "${sqlite3_SOURCE_DIR}"
)

target_include_directories(lib_task_server 
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(task_server PRIVATE
	${Boost_LIBRARIES}
	nlohmann_json::nlohmann_json
    ${CMAKE_THREAD_LIBS_INIT}
    lib_task_server
)

target_link_libraries(db_creator PRIVATE
	${Boost_LIBRARIES}
)

if (MSVC)
    target_compile_options(task_server PRIVATE
        /W4
    )
	target_compile_options(db_creator PRIVATE
        /W4
    )
    target_compile_options(lib_task_server PRIVATE
        /W4
    )
else ()
    target_compile_options(task_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
	target_compile_options(db_creator PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(lib_task_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS task_server RUNTIME DESTINATION bin)
install(TARGETS db_creator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
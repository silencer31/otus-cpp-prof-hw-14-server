cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(task_server VERSION ${PROJECT_VERSION})

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads)

# see https://www.sqlite.org/howtocompile.html
include(FetchContent)
FetchContent_Declare(sqlite3 URL "https://www.sqlite.org/snapshot/sqlite-snapshot-202205041843.tar.gz")
FetchContent_MakeAvailable(sqlite3)
if (NOT sqlite3_POPULATED)
  FetchContent_Populate(sqlite3)
endif()

include_directories(${Boost_INCLUDE_DIR})

configure_file(version.h.in version.h)

#Main target 
add_executable(task_server main.cpp client_session.cpp task_server.cpp data_storage.cpp "${sqlite3_SOURCE_DIR}/sqlite3.c")
add_executable(db_creator db_creator.cpp  "${sqlite3_SOURCE_DIR}/sqlite3.c")
add_library(lib_task_server lib.cpp)

set_target_properties(task_server db_creator lib_task_server PROPERTIES
    CXX_STANDARD 17
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
    ${CMAKE_THREAD_LIBS_INIT}
    lib_task_server
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

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
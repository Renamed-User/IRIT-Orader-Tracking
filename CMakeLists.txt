cmake_minimum_required(VERSION 3.15)

project(IRIT_Order_Tracking VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    "src/Order-Tracking/main.cpp"
    "src/database/database.cpp"
    "src/database/statement.cpp"
    "src/repos/user_repository.cpp"
    "src/repos/category_repository.cpp"
    "src/repos/role_repository.cpp"
    "src/repos/request_status_repository.cpp"
    "src/repos/product_repository.cpp"
    "src/repos/request_repository.cpp"
    "src/repos/request_item_repository.cpp")

add_library(sqlite3 STATIC
    "src/third_party/sqlite/sqlite3.c"
    "src/third_party/sqlite/sqlite3.h"
    "src/third_party/sqlite/sqlite3ext.h")
set_target_properties(sqlite3 PROPERTIES LINKER_LANGUAGE C)
target_include_directories(sqlite3 PUBLIC "src/third_party/sqlite")
target_link_libraries(${PROJECT_NAME} PRIVATE sqlite3)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "src/database/" 
    "src/repos/" 
    "src/models/"
    "src/services/"
    "src/ui/")

target_compile_definitions(IRIT_Order_Tracking PRIVATE
    DB_PATH="${CMAKE_SOURCE_DIR}/src/database/database.db")

if(NOT WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()

cmake_minimum_required(VERSION 3.15)

project(IRIT_Order_Tracking VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sqlite3 STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/sqlite/sqlite3.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/sqlite/sqlite3.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/sqlite/sqlite3ext.h"
)
set_target_properties(sqlite3 PROPERTIES LINKER_LANGUAGE C)
target_include_directories(sqlite3 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/sqlite")

add_executable(${PROJECT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Order-Tracking/main.cpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE sqlite3)

if(NOT WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()

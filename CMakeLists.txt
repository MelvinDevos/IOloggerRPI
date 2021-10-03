cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(LoggerIO)

add_executable(${PROJECT_NAME} main.c)

add_subdirectory(PJ_RPI)

target_link_libraries(${PROJECT_NAME} PJ_RPI)

include_directories(/usr/include/mariadb /usr/include/mariadb/mysql)

target_link_libraries(${PROJECT_NAME} mariadb)
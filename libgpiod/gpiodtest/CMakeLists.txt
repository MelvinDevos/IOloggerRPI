cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(GPIOlibDtest)

add_executable(${PROJECT_NAME} main.c)

include_directories(/usr/include)

target_link_libraries(${PROJECT_NAME} gpiod)

include_directories(/usr/include/mariadb /usr/include/mariadb/mysql)

target_link_libraries(${PROJECT_NAME} mariadb)
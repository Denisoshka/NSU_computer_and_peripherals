cmake_minimum_required(VERSION 3.8)

set(CMAKE_C_COMPILER clang)
set(MAIN main.c)
project(../lab1 C)
add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} m)
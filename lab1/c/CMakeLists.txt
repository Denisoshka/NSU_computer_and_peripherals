cmake_minimum_required(VERSION 3.8)

set(MAIN main.c)
project(c C)
add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} m)
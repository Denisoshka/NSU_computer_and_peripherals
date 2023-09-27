cmake_minimum_required(VERSION 3.8)

set(CMAKE_C_COMPILER g++)
set(MAIN main.cpp)
project(lab4 CXX)
add_executable(${PROJECT_NAME} ${MAIN})

#[[
cmake_minimum_required(VERSION 3.8)
project(lab5 CXX)
set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -march=native")
set(MAIN other_main.cpp)
add_executable(${PROJECT_NAME} ${MAIN})]]
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

set(MAIN main.cpp)
project(lab4 CXX)
find_library(CBLAS libblas.a /usr/local/lib)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${CBLAS})
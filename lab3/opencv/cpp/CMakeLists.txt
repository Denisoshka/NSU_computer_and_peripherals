cmake_minimum_required(VERSION 3.8)

set(MAIN main.cpp
        ../../../lab4/blas/main.cpp)
project(cpp CXX)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )

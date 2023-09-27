cmake_minimum_required(VERSION 3.8)

set(MAIN main.cpp)
project(cpp CXX)

find_package(OpenCV REQUIRED)
#find_package(libusb-1.0 REQUIRED)
#include_directories(${LIBUSB_1_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )

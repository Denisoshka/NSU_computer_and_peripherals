cmake_minimum_required(VERSION 3.8)

set(MAIN main.cpp)
project(lib_usb CXX)

set(LIBUSB_DEBUG 3)
add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} usb-1.0)

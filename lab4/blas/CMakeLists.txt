cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

set(MAIN main.cpp)
project(lab4 CXX)
#add_executable(${PROJECT_NAME} ${MAIN})

#find_package( Atlas REQUIRED )
find_package(MKL CONFIG)
if (${MKL_FOUND})
    message(STATUS "MKL found")
    add_executable(BLAS
            main.cpp)
    target_compile_options(BLAS PUBLIC
            $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)
    target_include_directories(BLAS PUBLIC
            $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)
    target_link_libraries(BLAS PUBLIC
            $<LINK_ONLY:MKL::MKL>)
    add_test(test_BLAS BLAS)
endif()
#target_link_libraries(${PROJECT_NAME} /usr/include/x86_64-linux-gnu/atlas)

cmake_minimum_required(VERSION 3.22)
project(CXXTHreadPool)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE OFF)

add_subdirectory(src)

add_executable(test example.cpp)
target_link_libraries(test PRIVATE mythread)
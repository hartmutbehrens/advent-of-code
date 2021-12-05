project(day04)
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

add_executable(day04 main.cpp)
configure_file(input ${CMAKE_CURRENT_BINARY_DIR}/input)
configure_file(numbers ${CMAKE_CURRENT_BINARY_DIR}/numbers)

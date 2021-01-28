cmake_minimum_required(VERSION 3.5)

project(reserving_allocator)

add_executable(reserving_allocator main.cpp)
set_target_properties(reserving_allocator PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)


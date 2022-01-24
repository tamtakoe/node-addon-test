cmake_minimum_required(VERSION 3.16)
project(cl_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "/O2")

add_executable(cl_project main.cpp)
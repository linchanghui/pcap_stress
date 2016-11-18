cmake_minimum_required(VERSION 3.5)
project(pacp_stress)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(SOURCE_FILES main.c main.c main.h)
add_executable(pacp_stress ${SOURCE_FILES})
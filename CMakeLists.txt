cmake_minimum_required(VERSION 3.5)
project(pacp_stress)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")
set(SOURCE_FILES sds.c sds.h sdsalloc.h main.c main.h)
add_executable(pacp_stress ${SOURCE_FILES})

target_link_libraries(pacp_stress
        pcre)

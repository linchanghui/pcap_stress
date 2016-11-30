cmake_minimum_required(VERSION 3.5)
project(pacp_stress)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")
set(SOURCE_FILES sds.c sds.h sdsalloc.h main.c main.h)
set(SOURCE_FILES_TEST test.c)

add_executable(pcap_stress ${SOURCE_FILES})


target_link_libraries(pcap_stress
        pcre)

add_executable(test1 ${SOURCE_FILES_TEST})

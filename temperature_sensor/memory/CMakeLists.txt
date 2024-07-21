cmake_minimum_required(VERSION 3.13)
project(simple_memory)
add_library(simple_memory memory.cpp)
set_target_properties(simple_memory PROPERTIES PUBLIC_HEADER memory.h)
install(TARGETS simple_memory LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)
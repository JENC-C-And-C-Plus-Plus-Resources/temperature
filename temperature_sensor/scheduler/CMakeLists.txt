cmake_minimum_required(VERSION 3.13)
project(simple_scheduler)
add_library(simple_scheduler scheduler.cpp)
set_target_properties(simple_scheduler PROPERTIES PUBLIC_HEADER scheduler.h)
install(TARGETS simple_scheduler LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)
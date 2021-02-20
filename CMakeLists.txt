cmake_minimum_required(VERSION 3.18)
project(topo_plusplus)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

add_subdirectory(src)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(tests)
    enable_testing()
endif()
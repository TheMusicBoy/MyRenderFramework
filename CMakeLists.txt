cmake_minimum_required(VERSION 3.15)

project(MyWindowFramework)

add_subdirectory(thirdparty)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(include)
add_subdirectory(src)

option(BUILD_EXAMPLE "" ON)

if(BUILD_EXAMPLE)
    message("Bulding examples...")
    add_subdirectory(example)
endif()

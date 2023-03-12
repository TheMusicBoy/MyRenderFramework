cmake_minimum_required(VERSION 3.15)

project(MyWindowFramework)

add_subdirectory(thirdparty)

set(CMAKE_CXX_STANDARD 17)

# Thread configs
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_subdirectory(src)

option(BUILD_EXAMPLE "" OFF)

if(BUILD_EXAMPLE)
    message("Bulding examples...")
    add_subdirectory(example)
endif()

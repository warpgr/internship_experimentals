cmake_minimum_required (VERSION 3.1.2)
project(concurrent)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package (Threads REQUIRED)

set (HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/containers/include/list.hpp)
set (HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/containers/include/mutex.hpp)

set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/containers/src/list.cpp )
set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/containers/src/mutex.cpp )

add_library(containers ${HEADERS} ${SOURCES})

target_include_directories (containers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/containers/include)

add_executable (thread_exec thread_main.cpp)
target_link_libraries (thread_exec ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (thread_exec containers)

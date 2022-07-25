
find_package ( Threads REQUIRED )

add_subdirectory ( containers )
add_executable ( thread_exec thread_main.cpp )
target_link_libraries (thread_exec ${CMAKE_THREAD_LIBS_INIT} )

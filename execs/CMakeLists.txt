
add_executable (thread_exec thread_main.cpp)
target_link_libraries (thread_exec ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (thread_exec containers)

add_executable ( vector_exec main.cpp )
target_link_libraries ( vector_exec my_lib )

add_executable ( lock_guard_exec lock_guard.cpp )
target_link_libraries (lock_guard_exec ${CMAKE_THREAD_LIBS_INIT} )


add_executable ( q_lock__exec queued_lock_exec.cpp )
target_link_libraries (q_lock__exec ${CMAKE_THREAD_LIBS_INIT} containers )




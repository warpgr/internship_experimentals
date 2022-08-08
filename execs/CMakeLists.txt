
add_executable (thread_exec thread_main.cpp)
target_link_libraries (thread_exec ${CMAKE_THREAD_LIBS_INIT} containers)

add_executable ( vector_exec main.cpp )
target_link_libraries ( vector_exec my_lib )

add_executable ( lock_guard_exec lock_guard.cpp )
target_link_libraries (lock_guard_exec ${CMAKE_THREAD_LIBS_INIT} )


add_executable ( q_lock__exec queued_lock_exec.cpp )
target_link_libraries (q_lock__exec ${CMAKE_THREAD_LIBS_INIT} containers )


add_executable ( ticket_lock ticket_lock_exec.cpp )
target_link_libraries (ticket_lock ${CMAKE_THREAD_LIBS_INIT} containers )


add_executable ( lock_types lock_types.cpp )
target_link_libraries (lock_types ${CMAKE_THREAD_LIBS_INIT} containers )
set_target_properties ( lock_types PROPERTIES CXX_STANDARD 17)


add_executable ( custom_unique_lock_exec custom_unique_lock.cpp )
target_link_libraries (custom_unique_lock_exec ${CMAKE_THREAD_LIBS_INIT} containers )



add_executable ( condvar_exec condvar_exec.cpp )
target_link_libraries (condvar_exec ${CMAKE_THREAD_LIBS_INIT} containers )

add_executable ( thread_pool_exec thread_pool_exec.cpp )
target_link_libraries (thread_pool_exec ${CMAKE_THREAD_LIBS_INIT} containers )


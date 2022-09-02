set(CMAKE_CXX_STANDARD 20)

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

add_executable ( custom_unique_lock_exec custom_unique_lock.cpp )
target_link_libraries (custom_unique_lock_exec ${CMAKE_THREAD_LIBS_INIT} containers )

add_executable ( condvar_exec condvar_exec.cpp )
target_link_libraries (condvar_exec ${CMAKE_THREAD_LIBS_INIT} containers )

add_executable ( thread_pool_exec thread_pool_exec.cpp )
target_link_libraries (thread_pool_exec ${CMAKE_THREAD_LIBS_INIT} containers )

add_executable ( future_std_example future_std_example.cpp )
target_link_libraries ( future_std_example ${CMAKE_THREAD_LIBS_INIT})

add_executable ( custom_future_exec future_custom_exec.cpp )
target_link_libraries ( custom_future_exec ${CMAKE_THREAD_LIBS_INIT} containers)


add_executable ( pack_task_exec packaged_task_test.cpp )
target_link_libraries ( pack_task_exec ${CMAKE_THREAD_LIBS_INIT} containers)


add_executable ( custom_async_exec custom_async_exec.cpp )
target_link_libraries ( custom_async_exec ${CMAKE_THREAD_LIBS_INIT} containers)

add_executable ( simple_fiber_exec simple_fiber_exec.cpp )
target_link_libraries ( simple_fiber_exec ${CMAKE_THREAD_LIBS_INIT} containers)


add_executable ( fibers_with_thread_pool fibers_with_thread_pool.cpp )
target_link_libraries ( fibers_with_thread_pool ${CMAKE_THREAD_LIBS_INIT} containers)


add_executable ( custom_fiber_async_exec custom_fiber_async_exec.cpp )
target_link_libraries ( custom_fiber_async_exec ${CMAKE_THREAD_LIBS_INIT} containers)



add_executable ( fiber_future_exec fiber_future_exec.cpp )
target_link_libraries ( fiber_future_exec ${CMAKE_THREAD_LIBS_INIT} containers)

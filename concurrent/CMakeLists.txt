set(CMAKE_CXX_STANDARD 20)

set (HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/containers/list.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/containers/mpmc_queue.hpp

             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/mutex.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/queued_lock.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/unique_lock.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/condition_variable.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/thread_pool.hpp 
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/utility_concepts.hpp
             
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/future.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/async.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/packaged_task.hpp
             
             
             ${CMAKE_CURRENT_SOURCE_DIR}/impl/shared_state_impl/shared_state_interface.hpp 
             ${CMAKE_CURRENT_SOURCE_DIR}/impl/shared_state_impl/shared_state_recv.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/impl/shared_state_impl/shared_state_send.hpp
             )


# set ( IMPL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/impl/shared_state_impl/shared_state_interface.hpp 
#                    ${CMAKE_CURRENT_SOURCE_DIR}/impl/shared_state_impl/shared_state_recv.hpp
#                    ${CMAKE_CURRENT_SOURCE_DIR}/impl/shared_state_impl/shared_state_send.hpp )

set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/mutex.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/src/containers/list.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/src/condition_variable.cpp )

add_library(containers ${HEADERS} ${SOURCES}  ) #${IMPL_HEADERS}

target_include_directories ( containers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )
target_include_directories ( containers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/impl )

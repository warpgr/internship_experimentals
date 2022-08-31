set(CMAKE_CXX_STANDARD 20)

set (LINUX FALSE)
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (LINUX TRUE)
endif()

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


             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/fiber/fiber.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/fiber/executor.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/fiber/scheduller.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/fiber/round_robin_scheduller.hpp


             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/asio/asio.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/asio/error_code.hpp

             ${CMAKE_CURRENT_SOURCE_DIR}/impl/shared_state_impl/shared_state_interface.hpp 
             ${CMAKE_CURRENT_SOURCE_DIR}/impl/shared_state_impl/shared_state_recv.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/impl/shared_state_impl/shared_state_send.hpp

             ${CMAKE_CURRENT_SOURCE_DIR}/impl/fiber/posix_context_impl.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/impl/fiber/windows_context_impl.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/impl/fiber/context_impl.hpp


             ${CMAKE_CURRENT_SOURCE_DIR}/impl/synchronization_primitives/mutex_impl.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/impl/synchronization_primitives/condition_variable_impl.hpp
             )

set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/containers/list.cpp

             ${CMAKE_CURRENT_SOURCE_DIR}/src/condition_variable.cpp



             ${CMAKE_CURRENT_SOURCE_DIR}/src/fiber/fiber.cpp



             ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/fiber/windows_context_impl.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/fiber/posix_context_impl.cpp
             )

if ( WIN32 )
    list ( REMOVE_ITEM HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/impl/fiber/posix_context_impl.hpp" )
    list ( REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/impl/fiber/posix_context_impl.cpp" )
elseif( LINUX )
    list ( REMOVE_ITEM HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/impl/fiber/windows_context_impl.hpp" )
    list ( REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/impl/fiber/windows_context_impl.cpp" )
endif()

add_library(containers ${HEADERS} ${SOURCES} )

if ( WIN32 )
    message (STATUS "USING_PLATFORM_WIN")
    target_compile_definitions ( containers PUBLIC USING_PLATFORM_WIN_)
else()
    message (STATUS "USING_PLATFORM_LINUX")
    target_compile_definitions ( containers PUBLIC USING_PLATFORM_LINUX_)
endif()

target_include_directories ( containers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                                               ${CMAKE_CURRENT_SOURCE_DIR}/impl
                            )


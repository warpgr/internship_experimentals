
set (HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/containers/list.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/containers/mpmc_queue.hpp

             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/mutex.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/queued_lock.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/unique_lock.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/condition_variable.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/include/concurrent/thread_pool.hpp )



set (SOURCES  )
set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/mutex.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/src/containers/list.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/src/queued_lock.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/src/condition_variable.cpp )

add_library(containers ${HEADERS} ${SOURCES})

target_include_directories (containers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

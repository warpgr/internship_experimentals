
find_package ( Threads REQUIRED )

set ( HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/containers/include/list.hpp)
set ( HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/containers/include/mutex.hpp)

set ( SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/list.cpp )
set ( SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/mutex.cpp )


add_library(containers ${HEADERS} ${SOURCES})

target_include_directories (containers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/containers/include)

add_executable ( thread_exec thread_main.cpp )
target_link_libraries (thread_exec ${CMAKE_THREAD_LIBS_INIT} )

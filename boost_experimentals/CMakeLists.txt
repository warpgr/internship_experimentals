set(CMAKE_CXX_STANDARD 20)

set (SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/pp_tree.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example_1.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/example_2.cpp")

add_executable (pp_tree_exec ${SOURCES} )
target_include_directories (pp_tree_exec PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries (pp_tree_exec ${Boost_LIBRARIES})

add_executable ( b_thread_ex ${CMAKE_CURRENT_SOURCE_DIR}/thread.cpp )
target_link_libraries (b_thread_ex ${Boost_LIBRARIES})

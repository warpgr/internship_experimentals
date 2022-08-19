# set (FILE GLOB HEADERs "include/conteiners/*.hpp")
set(CMAKE_CXX_STANDARD 20)
set ( HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/conteiners/vector.hpp)

set ( SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/vector.cpp )

add_library (my_lib ${HEADERS} ${SOURCES})

target_include_directories (my_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

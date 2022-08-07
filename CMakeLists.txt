cmake_minimum_required ( VERSION 3.1.2)

project ( internship_cource )

set_property ( GLOBAL PROPERTY CMAKE_CXX_COMPILER "g++-10")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package ( Boost REQUIRED COMPONENTS filesystem thread chrono )
find_package ( Threads REQUIRED )

add_subdirectory ( boost_experimentals )
add_subdirectory ( concurrent )
add_subdirectory ( my_lib )
add_subdirectory ( execs )

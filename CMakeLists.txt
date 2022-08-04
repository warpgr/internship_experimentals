cmake_minimum_required ( VERSION 3.1.2)

project ( internship_cource )


find_package ( Boost REQUIRED COMPONENTS filesystem thread chrono )
find_package ( Threads REQUIRED )

set_property ( GLOBAL PROPERTY CXX_STANDARD 17 )
# set_property ( GLOBAL PROPERTY CMAKE_CXX_COMPILER_VERSION "10.3.0")
set_property ( GLOBAL PROPERTY CMAKE_CXX_COMPILER "clang++")

add_subdirectory ( boost_experimentals )
add_subdirectory ( concurrent )
add_subdirectory ( my_lib )
add_subdirectory ( execs )

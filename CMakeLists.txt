cmake_minimum_required ( VERSION 3.1.2)

project ( internship_cource )


find_package ( Boost REQUIRED COMPONENTS filesystem thread chrono )
find_package ( Threads REQUIRED )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fsanitize=address")

add_subdirectory ( boost_experimentals )
add_subdirectory ( concurrent )
add_subdirectory ( my_lib )
add_subdirectory ( execs )

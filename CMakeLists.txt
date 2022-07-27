cmake_minimum_required ( VERSION 3.1.2)

project ( internship_cource )


find_package ( Boost REQUIRED COMPONENTS filesystem thread chrono )

add_subdirectory ( boost_experimentals )
add_subdirectory ( concurrent )
add_subdirectory ( my_lib )
add_subdirectory ( execs )

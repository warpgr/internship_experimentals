cmake_minimum_required ( VERSION 3.1.2)

project ( internship_cource )


find_package ( Boost REQUIRED COMPONENTS filesystem )

add_subdirectory ( boost_experimentals )
add_subdirectory ( my_lib )

add_executable ( vector_exec main.cpp )

target_link_libraries ( vector_exec my_lib )


add_executable ( lambda_exec lambda_main.cpp )

cmake_minimum_required(VERSION 3.5)
project(leaftests)

include_directories(../Log/include)

include_directories(
	../include
	./include
)

add_subdirectory(test_main)

add_subdirectory(tests/Sanity)
add_subdirectory(tests/Package)
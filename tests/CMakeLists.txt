cmake_minimum_required(VERSION 3.5)
project(leaftests)

include_directories(../Log/include)

add_definitions(-DDEBUG)

include_directories(
	../include
	./include
)

add_subdirectory(test_main)

add_subdirectory(tests/Sanity)

add_subdirectory(tests/Downloader)
add_subdirectory(tests/LeafArchive)
add_subdirectory(tests/PackageListParser)

add_subdirectory(tests/Package)
add_subdirectory(tests/Leafcore)

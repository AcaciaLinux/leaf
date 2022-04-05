cmake_minimum_required(VERSION 3.5)
project(leaf)

include_directories(include)
include_directories(args)
include_directories(leafcore/include)

set(CMAKE_CXX_STANDARD 17)

option(LEAF_STATIC "Build a static version of leaf" OFF)

if (LEAF_STATIC)
	message("Building a static leaf executable!")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	set(BUILD_SHARED_LIBS OFF)
	set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

if (CMAKE_BUILD_TYPE)
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		enable_testing(true)
		add_subdirectory(googletest)
		add_subdirectory(tests)
		add_definitions(-DDEBUG)
	endif()
endif()

add_subdirectory(leafcore)

add_subdirectory(Log)
include_directories(Log/include)

set (PNAME ${PROJECT_NAME})

file (GLOB_RECURSE CXXSOURCES "src/**.cpp")
file (GLOB_RECURSE HEADERS "include/**.h")

add_executable(${PNAME} ${HEADERS} ${CXXSOURCES})
target_link_libraries(${PNAME} log leafcore)

install (TARGETS ${PNAME} RUNTIME DESTINATION /usr/bin)
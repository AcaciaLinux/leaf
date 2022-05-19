cmake_minimum_required(VERSION 3.5)
project(leaf)

message("Compiler used: " ${CMAKE_CXX_COMPILER_ID})

set(LEAF_ADDITIONAL_LIBRARIES "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(LEAF_STATIC "Build a static version of leaf" OFF)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("Using macOS include path for libarchive, use \"brew install libarchive\" to install it")
	set (LibArchive_INCLUDE_DIR /opt/homebrew/opt/libarchive/include)
endif()

if (LEAF_STATIC)
	set(BUILD_SHARED_LIBS OFF)
else()
	find_package(LibArchive REQUIRED)
	find_package(CURL REQUIRED)
	set(LEAF_ADDITIONAL_LIBRARIES -lcurl -larchive ${LibArchive_LIBRARIES} ${CURL_LIBRARIES})
endif()

if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
	message("Forcing the GNU compiler to link against stdc++fs")
	set(LEAF_ADDITIONAL_LIBRARIES ${LEAF_ADDITIONAL_LIBRARIES} stdc++fs)
endif()

include_directories(include)
include_directories(args)
include_directories(leafcore/include)

if (CMAKE_BUILD_TYPE)
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		enable_testing(true)
		add_subdirectory(Log/googletest)
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

if (LEAF_STATIC)
	message("Building a static leaf executable!")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	set(BUILD_SHARED_LIBS OFF)
endif()

add_executable(${PNAME} ${HEADERS} ${CXXSOURCES})
target_link_libraries(${PNAME} log leafcore ${LEAF_ADDITIONAL_LIBRARIES})
if (LEAF_STATIC)
	target_link_options(${PNAME} PRIVATE -static)
endif()
install (TARGETS ${PNAME} RUNTIME DESTINATION /usr/bin)

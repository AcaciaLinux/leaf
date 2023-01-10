cmake_minimum_required(VERSION 3.5)
project(leaf)

message("Compiler used: " ${CMAKE_CXX_COMPILER_ID})

set(LEAF_ADDITIONAL_LIBRARIES "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(LEAF_STATIC "Build a static version of leaf" OFF)
option(LEAF_NONCURSES "Build leaf without ncurses dependency" OFF)
option(BUILD_DOCS "Build the documentation for leafcore" OFF)
option(BUILD_TESTS "Build the test suite" OFF)

set(LOG_MAXLEVEL 15 CACHE STRING "Set the maximum log level")
set(LOG_NOCHECK ON CACHE BOOL "Disable Log checks")
set(LOG_NOMUTEX ON CACHE BOOL "Disable Log mutexes")

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

if (BUILD_TESTS)
	enable_testing(true)
	add_subdirectory(Log/googletest)
	add_subdirectory(tests)
	add_subdirectory(cleaf/tests)
	add_definitions(-DDEBUG)
endif()

if(BUILD_DOCS)
	message("Building Doxygen documentation")
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/docs)
endif()

set (PNAME ${PROJECT_NAME})

file (GLOB_RECURSE CXXSOURCES "src/**.cpp")
file (GLOB_RECURSE HEADERS "include/**.h")

if (LEAF_STATIC)
	message("[Leaf] Building a static leaf executable!")
	set(LEAF_NONCURSES ON BOOL "Disable ncurses support")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	set(BUILD_SHARED_LIBS OFF)
endif()

set(LOG_MAXLEVEL 13 CACHE STRING "Set the maximum log level")

add_subdirectory(leafcore)
add_subdirectory(cleaf)
add_subdirectory(pyleaf)

add_subdirectory(Log)
include_directories(Log/include)

add_executable(${PNAME} ${HEADERS} ${CXXSOURCES})
target_link_libraries(${PNAME} PRIVATE leafcore ${LEAF_ADDITIONAL_LIBRARIES})
if (LEAF_STATIC)
	target_link_options(${PNAME} PRIVATE -static)
endif()
install (TARGETS ${PNAME} RUNTIME DESTINATION /usr/bin)

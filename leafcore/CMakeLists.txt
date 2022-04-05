cmake_minimum_required(VERSION 3.5)
project(libleafcore)

if (CMAKE_BUILD_TYPE)
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		message("Building in debug mode, performance will be much lower!")
		add_definitions(-DDEBUG)
	endif()
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("Using macOS include path for libarchive, use \"brew install libarchive\" to install it")
	set (LibArchive_INCLUDE_DIR /opt/homebrew/opt/libarchive/include)
endif()

find_package(CURL REQUIRED)
find_package(LibArchive REQUIRED)

include_directories(include)
include_directories(../Log/include)

include_directories(${CURL_INCLUDE_DIRS})
include_directories(${LibArchive_INCLUDE_DIRS})

file (GLOB_RECURSE CXXSOURCES "src/**.cpp")
file (GLOB_RECURSE HEADERS "include/**.h")

add_library(leafcore ${HEADERS} ${CXXSOURCES})
target_link_libraries(leafcore ${CURL_LIBRARIES} ${LibArchive_LIBRARIES})
target_compile_features(leafcore PRIVATE cxx_std_17)
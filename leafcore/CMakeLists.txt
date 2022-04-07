cmake_minimum_required(VERSION 3.5)
project(libleafcore)

if (CMAKE_BUILD_TYPE)
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		message("Building in debug mode, performance will be much lower!")
		add_definitions(-DDEBUG)
	endif()
endif()

include_directories(include)
include_directories(../Log/include)

include_directories(libarchive/libarchive)
include_directories(curl/include)

##### CURL
set(CURL_STATICLIB ON) 
set(HTTP_ONLY ON CACHE BOOL "")
set(CMAKE_USE_OPENSSL OFF CACHE BOOL "")
set(BUILD_CURL_EXE OFF CACHE BOOL "")
set(CURL_STATICLIB ON CACHE BOOL "")
set(CURL_ZLIB OFF CACHE BOOL "")
set(CMAKE_USE_LIBSSH2 OFF CACHE BOOL "")
#####

add_subdirectory(curl)
add_subdirectory(libarchive/libarchive)

file (GLOB_RECURSE CXXSOURCES "src/**.cpp")
file (GLOB_RECURSE HEADERS "include/**.h")

add_library(leafcore STATIC ${HEADERS} ${CXXSOURCES})
target_link_libraries(leafcore curl archive_static)
target_link_options(leafcore PRIVATE -static)
target_compile_features(leafcore PRIVATE cxx_std_17)
cmake_minimum_required(VERSION 3.5)
project(libleafcore)

if (CMAKE_BUILD_TYPE)
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		message("Building in debug mode, performance will be much lower!")
		add_definitions(-DDEBUG)
	endif()
endif()

if (LEAF_STATIC)
	set(BUILD_SHARED_LIBS OFF)
endif()

include_directories(include)
include_directories(../Log/include)

include_directories(libarchive/libarchive)
include_directories(curl/include)

##### CURL
set(CURL_STATICLIB ON) 
set(CURL_USE_LIBSSH OFF)
set(CURL_USE_LIBSSH2 OFF)
set(CURL_STATICLIB ON CACHE BOOL "") 
set(HTTP_ONLY ON CACHE BOOL "")
set(CMAKE_USE_OPENSSL OFF CACHE BOOL "")
set(BUILD_CURL_EXE OFF CACHE BOOL "")
set(CURL_STATICLIB ON CACHE BOOL "")
set(CURL_ZLIB OFF CACHE BOOL "")
set(CMAKE_USE_LIBSSH2 OFF CACHE BOOL "")
#####

###### LIBARCHIVE
set(ENABLE_ZLIB OFF)
set(ENABLE_BZip2 OFF)
set(ENABLE_LZMA OFF)
set(ENABLE_LZ4 OFF)
set(ENABLE_ZSTD OFF)
set(ENABLE_OPENSSL OFF)
set(ENABLE_LIBXML2 OFF)
set(ENABLE_ACL OFF)
set(ENABLE_EXPAT OFF)

set(ENABLE_ZLIB OFF CACHE BOOL "")
set(ENABLE_BZip2 OFF CACHE BOOL "")
set(ENABLE_LZMA OFF CACHE BOOL "")
set(ENABLE_LZ4 OFF CACHE BOOL "")
set(ENABLE_ZSTD OFF CACHE BOOL "")
set(ENABLE_OPENSSL OFF CACHE BOOL "")
set(ENABLE_LIBXML2 OFF CACHE BOOL "")
set(ENABLE_ACL OFF CACHE BOOL "")
set(ENABLE_EXPAT OFF CACHE BOOL "")

set(ENABLE_OPENSSL OFF)
#####

add_subdirectory(curl)
add_subdirectory(libarchive)

file (GLOB_RECURSE CXXSOURCES "src/**.cpp")
file (GLOB_RECURSE HEADERS "include/**.h")

if (LEAF_STATIC)
	message("Building a static leaf executable!")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	set(BUILD_SHARED_LIBS OFF)
endif()

add_library(leafcore STATIC ${HEADERS} ${CXXSOURCES})
target_link_libraries(leafcore libcurl archive_static)
target_link_options(leafcore PRIVATE -static)
target_compile_features(leafcore PRIVATE cxx_std_17)
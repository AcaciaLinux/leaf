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

include_directories(${CURL_INCLUDE_DIRS})
include_directories(${LibArchive_INCLUDE_DIRS})

file (GLOB_RECURSE CXXSOURCES "src/**.cpp")
file (GLOB_RECURSE HEADERS "include/**.h")

add_library(leafcore STATIC ${HEADERS} ${CXXSOURCES})
target_link_libraries(leafcore ${CURL_LIBRARIES} ${LibArchive_LIBRARIES})
target_compile_features(leafcore PRIVATE cxx_std_17)
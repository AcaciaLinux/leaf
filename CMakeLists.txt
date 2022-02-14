cmake_minimum_required(VERSION 3.5)
project(leaf)

include_directories(include)
include_directories(args)
include_directories(leafcore/include)

if (CMAKE_BUILD_TYPE)
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		enable_testing(true)
		add_subdirectory(googletest)
		add_subdirectory(tests)
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
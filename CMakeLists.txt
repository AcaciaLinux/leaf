cmake_minimum_required(VERSION 3.5)
project(leaf)

include_directories(include)

add_subdirectory(Log)
include_directories(Log/include)

set (PNAME ${PROJECT_NAME})

file (GLOB_RECURSE CXXSOURCES "src/**.cpp")
file (GLOB_RECURSE HEADERS "include/**.h")

add_executable(${PNAME} ${HEADERS} ${CXXSOURCES})
target_link_libraries(${PNAME} log)
cmake_minimum_required(VERSION 3.5)
project(leaf)

set (PNAME ${PROJECT_NAME})

file (GLOB_RECURSE CXXSOURCES "src/**.cpp")
file (GLOB_RECURSE HEADERS "include/**.h")

add_executable(${PNAME} ${HEADERS} ${CXXSOURCES})
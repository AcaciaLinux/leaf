cmake_minimum_required(VERSION 3.5)
project(libleafcore)

include_directories(include)
include_directories(../Log/include)

file (GLOB_RECURSE CXXSOURCES "src/**.cpp")
file (GLOB_RECURSE HEADERS "include/**.h")

add_library(leafcore ${HEADERS} ${CXXSOURCES})
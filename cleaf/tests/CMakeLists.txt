cmake_minimum_required(VERSION 3.5)
project(cleaftests)

include_directories(../Log/include)

add_definitions(-DDEBUG)

include_directories(
    ../include
    ./include
    ../../Log/include
    ../../tests/include
)

add_subdirectory(tests/Cleaf)


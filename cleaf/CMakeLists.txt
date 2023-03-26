
cmake_minimum_required(VERSION 3.5)
project(libcleaf)

if (BUILD_TESTS)
        message("Building in tests, performance will be much lower and fail guards will be used!")
        add_definitions(-DDEBUG)
endif()

include_directories(include)
include_directories(../leafcore/include)
include_directories(../Log/include)

file (GLOB_RECURSE CXXSOURCES "src/**.cpp")
file (GLOB_RECURSE HEADERS "include/**.h")

if (LEAF_STATIC)
    message("[Leaf] Building a static leaf executable!")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)
endif()

if (LEAF_STATIC)
    add_library(cleaf STATIC ${HEADERS} ${CXXSOURCES})
    target_link_options(cleaf PRIVATE -static)
else()
    add_library(cleaf SHARED ${HEADERS} ${CXXSOURCES})
endif()

target_link_libraries(cleaf log leafcore)
target_compile_features(cleaf PRIVATE cxx_std_17)

install(TARGETS cleaf DESTINATION /usr/lib)
install(DIRECTORY include/ DESTINATION include/cleaf)

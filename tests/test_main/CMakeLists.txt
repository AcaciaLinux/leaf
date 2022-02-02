
file(GLOB_RECURSE sources "./**.cpp")

add_library(test_main STATIC ${sources})

target_link_libraries(test_main PUBLIC
    gtest
    log
)

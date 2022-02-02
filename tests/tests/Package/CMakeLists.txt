
set(TEST_NAME PackageTest)

file(GLOB_RECURSE sources "./src/**.cpp")

add_executable(${TEST_NAME} ${sources})

target_link_libraries(${TEST_NAME} PUBLIC
    gtest
    test_main
    leafcore
)

add_test(
    NAME ${TEST_NAME}
    COMMAND ${TEST_NAME}
)

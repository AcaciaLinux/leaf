set(TEST_NAME CleafconfigTest)

file(GLOB_RECURSE sources "./src/**.cpp")

add_executable(${TEST_NAME} ${sources})

target_link_libraries(${TEST_NAME} PUBLIC
    gtest
    test_main
    leafcore
    cleaf
)

target_compile_features(${TEST_NAME} PRIVATE cxx_std_17)

add_test(
    NAME ${TEST_NAME}
    COMMAND ${TEST_NAME}
)

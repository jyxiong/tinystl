function(add_test_case TEST_CASE)
    set(TEST_TARGET test_${TEST_CASE})

    add_executable(${TEST_TARGET} ${TEST_CASE}.cpp)
    target_link_libraries(${TEST_TARGET} PRIVATE Catch2WithMain tinystl)

	add_test(NAME test_${TEST_CASE} COMMAND ${TEST_TARGET})
endfunction()

add_test_case(array)
function(add_test_case TEST_FILE CASE_NAME)
    set(TEST_TARGET test_${CASE_NAME})

    add_executable(${TEST_TARGET} ${TEST_FILE})
    target_link_libraries(${TEST_TARGET} PRIVATE Catch2WithMain tinystl)

	add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
endfunction()

file(GLOB_RECURSE TEST_SOURCES *.cpp)
foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(CASE_NAME ${TEST_FILE} NAME_WE)
    add_test_case(${TEST_FILE} ${CASE_NAME})
endforeach()

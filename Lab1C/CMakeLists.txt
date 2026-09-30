# Lab 1: two programs for array processing, built on top of the C library.

add_executable(count_divisible count_divisible.c)
target_include_directories(count_divisible PUBLIC ../LibraryC)
target_link_libraries(count_divisible LibraryC)

add_executable(shift_array shift_array.c)
target_include_directories(shift_array PUBLIC ../LibraryC)
target_link_libraries(shift_array LibraryC)

# ---------------------------------------------------------------------------
# Tests (run: ctest --output-on-failure)
# ---------------------------------------------------------------------------
set(LAB1_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests/data)

# Run the program on tests/data/<folder>/<case>.in and compare its output
# with <case>.expected
function(lab1_add_io_test test_name target data_subdir case_name)
    add_test(
        NAME ${test_name}
        COMMAND ${CMAKE_COMMAND}
            -DPROGRAM=$<TARGET_FILE:${target}>
            -DINPUT=${LAB1_DATA_DIR}/${data_subdir}/${case_name}.in
            -DEXPECTED=${LAB1_DATA_DIR}/${data_subdir}/${case_name}.expected
            -DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/${test_name}.out
            -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_case.cmake)
endfunction()

# A test where the program MUST fail (incorrect input data)
function(lab1_add_bad_input_test test_name target input_file)
    add_test(NAME ${test_name} COMMAND ${target} ${input_file})
    set_tests_properties(${test_name} PROPERTIES WILL_FAIL TRUE)
endfunction()

# Program 1: count numbers divisible by at least one of 2..9
foreach(case IN ITEMS case1 case2 case3 case4 case5)
    lab1_add_io_test(Lab1C_count_${case} count_divisible count ${case})
endforeach()

# Program 2: array shift
foreach(case IN ITEMS case1 case2 case3 case4 case5 case6 case7 case8)
    lab1_add_io_test(Lab1C_shift_${case} shift_array shift ${case})
endforeach()

# Incorrect input data
lab1_add_bad_input_test(Lab1C_count_missing_file count_divisible ${LAB1_DATA_DIR}/bad/no_such_file.in)
lab1_add_bad_input_test(Lab1C_count_too_few_numbers count_divisible ${LAB1_DATA_DIR}/bad/too_few_numbers.in)
lab1_add_bad_input_test(Lab1C_count_negative_size count_divisible ${LAB1_DATA_DIR}/bad/negative_size.in)
lab1_add_bad_input_test(Lab1C_count_not_a_number count_divisible ${LAB1_DATA_DIR}/bad/not_a_number.in)
lab1_add_bad_input_test(Lab1C_shift_missing_file shift_array ${LAB1_DATA_DIR}/bad/no_such_file.in)
lab1_add_bad_input_test(Lab1C_shift_bad_direction shift_array ${LAB1_DATA_DIR}/bad/bad_direction.in)
lab1_add_bad_input_test(Lab1C_shift_negative_steps shift_array ${LAB1_DATA_DIR}/bad/negative_steps.in)
lab1_add_bad_input_test(Lab1C_shift_too_few_numbers shift_array ${LAB1_DATA_DIR}/bad/too_few_numbers.in)

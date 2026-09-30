cmake_minimum_required(VERSION 3.14)
project(LabArray LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Контейнер Array - заголовочная библиотека
add_library(lab_array INTERFACE)
target_include_directories(lab_array INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Две программы обработки массива
add_executable(count_divisible src/count_divisible.cpp)
target_link_libraries(count_divisible PRIVATE lab_array)

add_executable(shift_array src/shift_array.cpp)
target_link_libraries(shift_array PRIVATE lab_array)

# Модульные тесты
add_executable(test_array tests/test_array.cpp)
target_link_libraries(test_array PRIVATE lab_array)

add_executable(test_algorithms tests/test_algorithms.cpp)
target_link_libraries(test_algorithms PRIVATE lab_array)

# ---------------------------------------------------------------------------
# Тесты (запуск: ctest --output-on-failure)
# ---------------------------------------------------------------------------
enable_testing()

add_test(NAME unit_array COMMAND test_array)
add_test(NAME unit_algorithms COMMAND test_algorithms)

set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests/data)

# Запуск программы на tests/data/<папка>/<кейс>.in и сравнение с <кейс>.expected
function(add_io_test test_name target data_subdir case_name)
    add_test(
        NAME ${test_name}
        COMMAND ${CMAKE_COMMAND}
            -DPROGRAM=$<TARGET_FILE:${target}>
            -DINPUT=${DATA_DIR}/${data_subdir}/${case_name}.in
            -DEXPECTED=${DATA_DIR}/${data_subdir}/${case_name}.expected
            -DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/${test_name}.out
            -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_case.cmake)
endfunction()

# Тест, в котором программа ДОЛЖНА завершиться с ошибкой (некорректный ввод)
function(add_bad_input_test test_name target input_file)
    add_test(NAME ${test_name} COMMAND ${target} ${input_file})
    set_tests_properties(${test_name} PROPERTIES WILL_FAIL TRUE)
endfunction()

# Программа 1: подсчёт чисел, кратных хотя бы одному из 2..9
foreach(case IN ITEMS case1 case2 case3 case4 case5)
    add_io_test(count_divisible_${case} count_divisible count ${case})
endforeach()

# Программа 2: сдвиг массива
foreach(case IN ITEMS case1 case2 case3 case4 case5 case6 case7 case8)
    add_io_test(shift_array_${case} shift_array shift ${case})
endforeach()

# Некорректные входные данные
add_bad_input_test(count_divisible_missing_file count_divisible ${DATA_DIR}/bad/no_such_file.in)
add_bad_input_test(count_divisible_too_few_numbers count_divisible ${DATA_DIR}/bad/too_few_numbers.in)
add_bad_input_test(count_divisible_negative_size count_divisible ${DATA_DIR}/bad/negative_size.in)
add_bad_input_test(count_divisible_not_a_number count_divisible ${DATA_DIR}/bad/not_a_number.in)
add_bad_input_test(shift_array_missing_file shift_array ${DATA_DIR}/bad/no_such_file.in)
add_bad_input_test(shift_array_bad_direction shift_array ${DATA_DIR}/bad/bad_direction.in)
add_bad_input_test(shift_array_negative_steps shift_array ${DATA_DIR}/bad/negative_steps.in)
add_bad_input_test(shift_array_too_few_numbers shift_array ${DATA_DIR}/bad/too_few_numbers.in)

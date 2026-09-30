# Запускает программу на входном файле и сравнивает вывод с ожидаемым.
# Параметры (передаются через -D): PROGRAM, INPUT, EXPECTED, OUTPUT

execute_process(
    COMMAND "${PROGRAM}" "${INPUT}" "${OUTPUT}"
    RESULT_VARIABLE run_result
    OUTPUT_QUIET
    ERROR_VARIABLE run_error)

if(NOT run_result EQUAL 0)
    message(FATAL_ERROR "Program failed (code ${run_result}): ${run_error}")
endif()

file(READ "${OUTPUT}" actual)
file(READ "${EXPECTED}" expected)
string(STRIP "${actual}" actual)
string(STRIP "${expected}" expected)

if(NOT "${actual}" STREQUAL "${expected}")
    message(FATAL_ERROR "Output mismatch!\n  expected: '${expected}'\n  actual:   '${actual}'")
endif()

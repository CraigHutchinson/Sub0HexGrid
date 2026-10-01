function(run_checked)
    execute_process(COMMAND ${ARGV} RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Consumer step failed: ${ARGV}\n${output}\n${error}")
    endif()
endfunction()
string(RANDOM LENGTH 12 ALPHABET 0123456789abcdef suffix)
set(test_root "${PROJECT_BINARY}/package-test-${suffix}")
run_checked("${CMAKE_COMMAND}" --install "${PROJECT_BINARY}" --config "${CONSUMER_CONFIG}"
    --prefix "${test_root}/stage")
file(RENAME "${test_root}/stage" "${test_root}/relocated")
run_checked("${CMAKE_COMMAND}" -S "${PROJECT_SOURCE}/tests/consumer" -B "${test_root}/consumer"
    -G "${CONSUMER_GENERATOR}" "-DCMAKE_CXX_COMPILER=${CONSUMER_CXX}"
    "-DCMAKE_BUILD_TYPE=${CONSUMER_CONFIG}" "-DCMAKE_PREFIX_PATH=${test_root}/relocated"
    "-DSUB0HEXGRID_CONSUMER_SANITIZERS=${CONSUMER_SANITIZERS}")
run_checked("${CMAKE_COMMAND}" --build "${test_root}/consumer" --config "${CONSUMER_CONFIG}" --parallel 2)
run_checked("${CMAKE_CTEST_COMMAND}" --test-dir "${test_root}/consumer"
    -C "${CONSUMER_CONFIG}" --output-on-failure)

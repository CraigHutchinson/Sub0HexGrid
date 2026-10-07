include(${PROJECT_SOURCE_DIR}/cmake/get_cpm.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/DependencyPins.cmake)
CPMAddPackage(
    NAME doctest
    GITHUB_REPOSITORY doctest/doctest
    GIT_TAG ${SUB0HEXGRID_DOCTEST_COMMIT}
    OPTIONS "DOCTEST_WITH_TESTS OFF" "DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF" "DOCTEST_NO_INSTALL ON")

function(sub0hexgrid_add_tests target label)
    add_executable(${target} ${ARGN} ${PROJECT_SOURCE_DIR}/tests/main.cpp)
    target_link_libraries(${target} PRIVATE Sub0HexGrid::Sub0HexGrid doctest::doctest)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4;/WX;/permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Werror>)
    sub0hexgrid_instrument(${target})
    add_test(NAME ${label} COMMAND ${target})
    set_tests_properties(${label} PROPERTIES LABELS ${label})
endfunction()
add_library(sub0hexgrid_headers OBJECT headers/test_axial_header.cpp headers/test_point_header.cpp headers/test_layout_header.cpp
    headers/test_region_header.cpp headers/test_candidates_header.cpp headers/test_cursor_header.cpp
    headers/test_umbrella_header.cpp)
target_link_libraries(sub0hexgrid_headers PRIVATE Sub0HexGrid::Sub0HexGrid)
target_compile_options(sub0hexgrid_headers PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4;/WX;/permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Werror>)
add_subdirectory(topology)
add_subdirectory(geometry)
add_subdirectory(regions)
add_subdirectory(candidates)
add_subdirectory(spatial)
add_executable(sub0hexgrid_allocation_tests test_allocation.cpp)
target_link_libraries(sub0hexgrid_allocation_tests PRIVATE Sub0HexGrid::Sub0HexGrid)
sub0hexgrid_instrument(sub0hexgrid_allocation_tests)
add_test(NAME allocation COMMAND sub0hexgrid_allocation_tests)
set_tests_properties(allocation PROPERTIES LABELS allocation)
if(SUB0HEXGRID_BUILD_EXAMPLES)
    add_test(NAME mapping_example COMMAND sub0hexgrid_example)
    add_test(NAME spatial_example COMMAND sub0hexgrid_spatial)
endif()
add_test(NAME package_consumer COMMAND ${CMAKE_COMMAND}
    "-DPROJECT_BINARY=${PROJECT_BINARY_DIR}" "-DPROJECT_SOURCE=${PROJECT_SOURCE_DIR}"
    "-DCONSUMER_CXX=${CMAKE_CXX_COMPILER}" "-DCONSUMER_CONFIG=$<CONFIG>"
    "-DCONSUMER_GENERATOR=${CMAKE_GENERATOR}"
    "-DCONSUMER_SANITIZERS=${SUB0HEXGRID_ENABLE_SANITIZERS}"
    -P "${PROJECT_SOURCE_DIR}/cmake/CheckConsumer.cmake")
set_tests_properties(package_consumer PROPERTIES TIMEOUT 120 RUN_SERIAL TRUE)

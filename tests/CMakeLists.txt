add_executable(sub0hexgrid_tests kernel.cpp headers/axial.cpp headers/point.cpp headers/layout.cpp)
target_link_libraries(sub0hexgrid_tests PRIVATE Sub0HexGrid::Sub0HexGrid)
target_compile_options(sub0hexgrid_tests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4;/WX;/permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Werror>)
sub0hexgrid_instrument(sub0hexgrid_tests)
add_test(NAME kernel COMMAND sub0hexgrid_tests)
if(SUB0HEXGRID_BUILD_EXAMPLES)
    add_test(NAME mapping_example COMMAND sub0hexgrid_example)
endif()
add_test(NAME package_consumer COMMAND ${CMAKE_COMMAND}
    "-DPROJECT_BINARY=${PROJECT_BINARY_DIR}" "-DPROJECT_SOURCE=${PROJECT_SOURCE_DIR}"
    "-DCONSUMER_CXX=${CMAKE_CXX_COMPILER}" "-DCONSUMER_CONFIG=$<CONFIG>"
    "-DCONSUMER_GENERATOR=${CMAKE_GENERATOR}"
    "-DCONSUMER_SANITIZERS=${SUB0HEXGRID_ENABLE_SANITIZERS}"
    -P "${PROJECT_SOURCE_DIR}/cmake/CheckConsumer.cmake")
set_tests_properties(package_consumer PROPERTIES TIMEOUT 120 RUN_SERIAL TRUE)

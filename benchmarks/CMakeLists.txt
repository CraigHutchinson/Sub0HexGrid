include(${PROJECT_SOURCE_DIR}/cmake/get_cpm.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/DependencyPins.cmake)
CPMAddPackage(
    NAME nanobench
    GITHUB_REPOSITORY martinus/nanobench
    GIT_TAG ${SUB0HEXGRID_NANOBENCH_COMMIT})

add_executable(sub0hexgrid_benchmarks kernel.cpp)
target_link_libraries(sub0hexgrid_benchmarks PRIVATE Sub0HexGrid::Sub0HexGrid nanobench::nanobench)
target_compile_options(sub0hexgrid_benchmarks PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4;/WX;/permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Werror>)
sub0hexgrid_instrument(sub0hexgrid_benchmarks)

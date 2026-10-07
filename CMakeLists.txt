cmake_minimum_required(VERSION 3.25)
project(Sub0HexGrid VERSION 0.1.0 LANGUAGES CXX)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(SUB0HEXGRID_BUILD_TESTS "Build kernel and package tests" ${PROJECT_IS_TOP_LEVEL})
option(SUB0HEXGRID_BUILD_EXAMPLES "Build the mapping example" ${PROJECT_IS_TOP_LEVEL})
option(SUB0HEXGRID_BUILD_BENCHMARKS "Build nanobench scalar benchmarks" OFF)
option(SUB0HEXGRID_ENABLE_SANITIZERS "Instrument GCC/Clang with ASan/UBSan" OFF)

add_library(Sub0HexGrid STATIC src/axial.cpp src/pointy_layout.cpp)
add_subdirectory(src/regions)
add_subdirectory(src/candidates)
add_library(Sub0HexGrid::Sub0HexGrid ALIAS Sub0HexGrid)
target_compile_features(Sub0HexGrid PUBLIC cxx_std_23)
target_include_directories(Sub0HexGrid PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(Sub0HexGrid PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(Sub0HexGrid PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4;/WX;/permissive-;/Zc:preprocessor>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Werror>)
if(SUB0HEXGRID_ENABLE_SANITIZERS)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "Sanitizers require GCC or Clang")
    endif()
    target_compile_options(Sub0HexGrid PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
endif()

function(sub0hexgrid_instrument target)
    if(SUB0HEXGRID_ENABLE_SANITIZERS)
        target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
endfunction()

if(SUB0HEXGRID_BUILD_EXAMPLES)
    add_executable(sub0hexgrid_example examples/mapping.cpp)
    target_link_libraries(sub0hexgrid_example PRIVATE Sub0HexGrid::Sub0HexGrid)
    sub0hexgrid_instrument(sub0hexgrid_example)
    add_executable(sub0hexgrid_spatial examples/spatial.cpp)
    target_link_libraries(sub0hexgrid_spatial PRIVATE Sub0HexGrid::Sub0HexGrid)
    target_compile_options(sub0hexgrid_spatial PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4;/WX;/permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic;-Werror>)
    sub0hexgrid_instrument(sub0hexgrid_spatial)
endif()

if(SUB0HEXGRID_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(SUB0HEXGRID_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

install(TARGETS Sub0HexGrid EXPORT Sub0HexGridTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT Sub0HexGridTargets NAMESPACE Sub0HexGrid::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sub0HexGrid)
configure_package_config_file(cmake/Sub0HexGridConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/Sub0HexGridConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sub0HexGrid)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/Sub0HexGridConfigVersion.cmake"
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Sub0HexGridConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/Sub0HexGridConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sub0HexGrid)

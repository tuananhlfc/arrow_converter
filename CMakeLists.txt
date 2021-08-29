cmake_minimum_required(VERSION 3.14)
project(arrow_json_converter CXX)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# enable cache system
include(cmake/Cache.cmake)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conaninfo.txt)
    list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
endif()

find_package(Arrow REQUIRED)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)

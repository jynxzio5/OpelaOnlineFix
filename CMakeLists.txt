cmake_minimum_required(VERSION 3.16)
project(OpelaOnlineFix VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/CompilerWarnings.cmake)

add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

include_directories(include)

file(GLOB_RECURSE SOURCES
    "src/core/*.cpp"
    "src/proxy/*.cpp"
    "src/hooks/*.cpp"
)

file(GLOB_RECURSE HEADERS
    "include/*.h"
)

add_library(OpelaOnlineFix SHARED
    ${SOURCES}
    ${HEADERS}
)

set_project_warnings(OpelaOnlineFix)

set_target_properties(OpelaOnlineFix PROPERTIES
    OUTPUT_NAME "steam_api64"
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_executable(test_runner tests/test_runner.cpp)
add_dependencies(test_runner OpelaOnlineFix)

add_custom_command(TARGET OpelaOnlineFix POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/config/OpelaOnlineFix.ini"
        "${CMAKE_BINARY_DIR}/bin/OpelaOnlineFix.ini"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/config/dlllist.txt"
        "${CMAKE_BINARY_DIR}/bin/dlllist.txt"
)

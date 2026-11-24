cmake_minimum_required(VERSION 3.16)
project(OpelaOnlineFix VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/CompilerWarnings.cmake)

add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

include_directories(
    include
    third_party/MinHook/include
)

set(MINHOOK_SOURCES
    third_party/MinHook/src/buffer.c
    third_party/MinHook/src/hook.c
    third_party/MinHook/src/trampoline.c
    third_party/MinHook/src/hde/hde64.c
)

# 1. Root Sideloading Hook Target: winmm.dll
add_library(OpelaWinMM SHARED
    src/proxy/winmm.cpp
    ${MINHOOK_SOURCES}
)

set_target_properties(OpelaWinMM PROPERTIES
    OUTPUT_NAME "winmm"
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 2. Proxy steam_api64 Target
file(GLOB_RECURSE CORE_SOURCES "src/core/*.cpp")
add_library(OpelaOnlineFix SHARED
    ${CORE_SOURCES}
    src/proxy/steamworks_exports.cpp
)

set_project_warnings(OpelaOnlineFix)

set_target_properties(OpelaOnlineFix PROPERTIES
    OUTPUT_NAME "steam_api64"
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Post-build configs
add_custom_command(TARGET OpelaWinMM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/config/OpelaOnlineFix.ini"
        "${CMAKE_BINARY_DIR}/bin/OpelaOnlineFix.ini"
)

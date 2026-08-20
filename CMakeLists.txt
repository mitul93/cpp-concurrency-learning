cmake_minimum_required(VERSION 4.0)

# turn on vcpkg verbose output
# set(VCPKG_VERBOSE ON)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
    message(STATUS "CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

# Location of custom triplets
# https://learn.microsoft.com/en-us/vcpkg/concepts/triplets#triplet-selection
set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/triplets/" CACHE STRING "")

# configure detection of host and target triplets
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/vcpkg-auto-triplet.cmake)

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cpp-concurrency-learning LANGUAGES CXX)

# --- toggle for benchmarks --- #
option(BUILD_BENCHMARKS "Build the benchmark suite" OFF)

# --- toggle for Thread Sanitizer --- #
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

if(ENABLE_TSAN)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        # Following options must be set before adding subdirectories
        add_compile_options(-fsanitize=thread)
        add_link_options(-fsanitize=thread)
    else()
        message(FATAL_ERROR "ThreadSanitizer is not supported with ${CMAKE_CXX_COMPILER_ID}")
    endif()

    if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR
            "ENABLE_TSAN requires a Debug build. "
            "Configure with -DCMAKE_BUILD_TYPE=Debug.")
    endif()

    message(STATUS "Enabling Thread Sanitizer")
endif()

add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()
cmake_minimum_required(VERSION 4.0)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

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

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()
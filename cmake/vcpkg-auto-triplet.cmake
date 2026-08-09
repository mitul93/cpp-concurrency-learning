# vcpkg-auto-triplet
# 
# This triplet automatically configures vcpkg host and target triplet.
#
# CMake arguments precedence when provided; otherwise, values are
# detected automatically or set to the defaults listed below.
#
# Supported environment overrides:
#   VCPKG_TARGET_ARCHITECTURE  : auto-detected from host architecture
#   VCPKG_LIBRARY_LINKAGE      : dynamic
#   VCPKG_BUILD_TYPE           : release
#
# Example:
#   cmake -DVCPKG_BUILD_TYPE=debug ..
#   cmake -DVCPKG_TARGET_ARCHITECTURE=arm64 ..
#

# Host architecture
cmake_host_system_information(RESULT _host_arch QUERY OS_PLATFORM)

if(_host_arch MATCHES "^(x86_64|AMD64)$")
    set(_VCPKG_HOST_ARCHITECTURE "x64")
elseif(_host_arch MATCHES "^(aarch64|arm64)$")
    set(_VCPKG_HOST_ARCHITECTURE "arm64")
elseif(_host_arch MATCHES "^arm")
    set(_VCPKG_HOST_ARCHITECTURE "arm")
elseif(_host_arch_lc MATCHES "^s390x$")
    set(_VCPKG_HOST_ARCHITECTURE "s390x")
elseif(_host_arch_lc MATCHES "^ppc64le$")
    set(_VCPKG_HOST_ARCHITECTURE "ppc64le")
elseif(_host_arch_lc MATCHES "^(riscv32|rv32.*)$")
    set(_VCPKG_HOST_ARCHITECTURE "riscv32")
elseif(_host_arch_lc MATCHES "^(riscv64|rv64.*)$")
    set(_VCPKG_HOST_ARCHITECTURE "riscv64")
elseif(_host_arch_lc MATCHES "^(loongarch32|loong32)$")
    set(_VCPKG_HOST_ARCHITECTURE "loongarch32")
elseif(_host_arch_lc MATCHES "^(loongarch64|loong64)$")
    set(_VCPKG_HOST_ARCHITECTURE "loongarch64")
elseif(_host_arch_lc MATCHES "^(mips64|mips64el)$")
    set(_VCPKG_HOST_ARCHITECTURE "mips64")
elseif(_host_arch_lc MATCHES "^(wasm32|webassembly)$")
    set(_VCPKG_HOST_ARCHITECTURE "wasm32")
else()
    set(_VCPKG_HOST_ARCHITECTURE "${_host_arch}")
endif()

# If VCPKG target architecture is not defined, use host architecture
if(NOT DEFINED VCPKG_TARGET_ARCHITECTURE)
    set(VCPKG_TARGET_ARCHITECTURE "${_VCPKG_HOST_ARCHITECTURE}")
endif()

# VCPKG library linkage - defaults to dynamic
if(DEFINED VCPKG_LIBRARY_LINKAGE)
    set(VCPKG_LIBRARY_LINKAGE "${VCPKG_LIBRARY_LINKAGE}")
else()
    set(VCPKG_LIBRARY_LINKAGE "dynamic")
endif()

# VCPKG build type - defaults to release
if(DEFINED VCPKG_BUILD_TYPE)
    set(VCPKG_BUILD_TYPE "$ENV{VCPKG_BUILD_TYPE}")
else()
    set(VCPKG_BUILD_TYPE release)
endif()

# VCPKG cmake system - defaults to Linux
if(NOT DEFINED VCPKG_CMAKE_SYSTEM_NAME)
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
        set(VCPKG_CMAKE_SYSTEM_NAME Linux)
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        set(VCPKG_CMAKE_SYSTEM_NAME Darwin)
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        set(VCPKG_CMAKE_SYSTEM_NAME Windows)
    else()
        set(VCPKG_CMAKE_SYSTEM_NAME "${CMAKE_HOST_SYSTEM_NAME}")
    endif()
endif()

##### VCPKG_HOST_TRIPLET #####
string(TOLOWER "${VCPKG_CMAKE_SYSTEM_NAME}" _os_lower)

set(_linkage_suffix "")
if(VCPKG_LIBRARY_LINKAGE STREQUAL "dynamic")
    set(_linkage_suffix "-dynamic")
endif()

if(NOT DEFINED VCPKG_HOST_TRIPLET)
    set(VCPKG_HOST_TRIPLET "${_VCPKG_HOST_ARCHITECTURE}-${_os_lower}${_linkage_suffix}" CACHE STRING "")
endif()

##### VCPKG_TARGET_TRIPLET #####
if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "${VCPKG_TARGET_ARCHITECTURE}-${_os_lower}${_linkage_suffix}" CACHE STRING "")
endif()

message(STATUS "_VCPKG_HOST_ARCHITECTURE          = ${_VCPKG_HOST_ARCHITECTURE}")
message(STATUS "VCPKG_TARGET_ARCHITECTURE         = ${VCPKG_TARGET_ARCHITECTURE}")
message(STATUS "VCPKG_LIBRARY_LINKAGE             = ${VCPKG_LIBRARY_LINKAGE}")
message(STATUS "VCPKG_CMAKE_SYSTEM_NAME           = ${VCPKG_CMAKE_SYSTEM_NAME}")
message(STATUS "VCPKG_BUILD_TYPE                  = ${VCPKG_BUILD_TYPE}")
message(STATUS "VCPKG_TARGET_TRIPLET              = ${VCPKG_TARGET_TRIPLET}")
message(STATUS "VCPKG_HOST_TRIPLET                = ${VCPKG_HOST_TRIPLET}")

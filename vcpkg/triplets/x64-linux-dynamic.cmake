set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CMAKE_SYSTEM_NAME Linux)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_BUILD_TYPE release)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_FIXUP_ELF_RPATH ON)

# https://learn.microsoft.com/en-us/vcpkg/users/triplets#vcpkg_cmake_configure_options
# https://learn.microsoft.com/en-us/vcpkg/maintainers/functions/vcpkg_cmake_configure
set(VCPKG_CMAKE_CONFIGURE_OPTIONS
    "-DCMAKE_C_COMPILER=clang"
    "-DCMAKE_CXX_COMPILER=clang++"
)

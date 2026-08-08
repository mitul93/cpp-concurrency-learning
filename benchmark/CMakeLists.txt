find_package(benchmark CONFIG REQUIRED)

add_executable(cpp_benchmark main.cpp)

target_link_libraries(cpp_benchmark
    PRIVATE
    benchmark::benchmark
)
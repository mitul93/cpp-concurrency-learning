#pragma once

#include <algorithm>
#include <random>

#include "matrix.h"

namespace matrix::util {

template <typename T>
requires std::is_arithmetic_v<T>
void random_initialize(Matrix<T>& M, unsigned long seed = 42) {
    std::mt19937 rng(seed);

    auto dist = [&] {
        if constexpr (std::is_integral_v<T>) {
            return std::uniform_int_distribution<T>(0, 100);
        } else {
            return std::uniform_real_distribution<T>(0.0, 1.0);
        }
    }(); // invoke lambda here

    std::ranges::generate(M, [&] { return dist(rng); });
}

} // namespace matrix::util
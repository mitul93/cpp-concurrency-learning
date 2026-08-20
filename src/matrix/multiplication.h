#pragma once
// referenced from https://github.com/sala-aliaksei/MatrixMultiplication

#include "matrix.h"

namespace matrix {

template <typename T>
void multiply_naive(const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C) {

    auto M = A.row();
    auto K = A.col();
    auto N = B.col();

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < K; ++k) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}

} // namespace matrix
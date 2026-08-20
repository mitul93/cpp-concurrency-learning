#pragma once
// referenced from https://github.com/sala-aliaksei/MatrixMultiplication

#include <iomanip>
#include <iostream>
#include <vector>

namespace matrix {

template <typename T>
using aligned_vector = std::vector<T>;

template <typename T>
class Matrix {
  public:
    using value_type = T;

    Matrix(std::size_t row_cnt, std::size_t col_cnt)
        : rows(row_cnt), cols(col_cnt), _matrix(rows * cols, 0) {
    }

    [[__nodiscard__]] T operator[](std::size_t idx) const noexcept {
        return _matrix[idx];
    }

    [[__nodiscard__]] T& operator[](std::size_t idx) noexcept {
        return _matrix[idx];
    }

    [[__nodiscard__]] constexpr T* data() noexcept {
        return _matrix.data();
    }

    [[__nodiscard__]] constexpr const T* data() const noexcept {
        return _matrix.data();
    }

    [[__nodiscard__]] std::size_t size() const noexcept {
        return cols * rows;
    }

    [[__nodiscard__]] std::size_t col() const noexcept {
        return cols;
    }

    [[__nodiscard__]] std::size_t row() const noexcept {
        return rows;
    }

    auto begin() {
        return _matrix.begin();
    }
    auto end() {
        return _matrix.end();
    }
    auto begin() const {
        return _matrix.begin();
    }
    auto end() const {
        return _matrix.end();
    }

    const T& operator()(std::size_t i, std::size_t j) const noexcept {
        return _matrix[i * cols + j];
    }

    T& operator()(std::size_t i, std::size_t j) {
        return _matrix[i * cols + j];
    }

  private:
    std::size_t rows;
    std::size_t cols;

    aligned_vector<T> _matrix;
};

template <typename Stream, typename T>
Stream& operator<<(Stream& os, const Matrix<T>& m) {
    constexpr int width = 6; // Adjust width as needed
    for (auto i = 0; i < m.row(); ++i) {
        for (auto j = 0; j < m.col(); ++j) {
            os << std::setw(width) << m[i * m.col() + j] << ", ";
        }
        os << "\n";
    }
    return os;
}

template <typename T>
bool operator==(const Matrix<T>& s1, const Matrix<T>& s2) {
    auto row_cnt = s1.row();
    auto col_cnt = s1.col();

    if (col_cnt != s2.col())
        return false;

    if (row_cnt != s2.row())
        return false;

    for (int i = 0; i < row_cnt; i++) {
        for (int j = 0; j < col_cnt; j++) {
            if constexpr (std::is_floating_point_v<T>) {
                constexpr T epsilon = std::is_same_v<T, float> ? 1E-1f : 1E-6;
                if (std::abs(s1[i * col_cnt + j] - s2[i * col_cnt + j]) >
                    epsilon) {
                    std::cout << std::fixed << std::setprecision(15) << "elem["
                              << i << "][" << j << "] doesn't match. "
                              << s1[i * col_cnt + j]
                              << " != " << s2[i * col_cnt + j] << std::endl;
                    return false;
                }
            } else {
                return s1[i * col_cnt + j] == s2[i * col_cnt + j];
            }
        }
    }

    return true;
}

} // namespace matrix
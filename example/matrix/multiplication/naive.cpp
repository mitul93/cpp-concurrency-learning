#include "matrix/matrix.h"
#include "matrix/multiplication.h"
#include "matrix/util.h"

int main() {

    std::size_t rows = 10;
    std::size_t columns = 10;

    matrix::Matrix<double> mat_a(rows, columns), mat_b(rows, columns),
        mat_c(rows, columns);

    matrix::util::random_initialize(mat_a);
    matrix::util::random_initialize(mat_b);

    matrix::multiply_naive(mat_a, mat_b, mat_c);

    std::cout << "mat_a data=\n" << mat_a << "\n";
    std::cout << "mat_b data=\n" << mat_b << "\n";
    std::cout << "mat_c data=\n" << mat_c << "\n";

    return 0;
}
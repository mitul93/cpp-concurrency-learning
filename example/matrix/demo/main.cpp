#include "matrix/matrix.h"
#include "matrix/util.h"

int main() {

    std::size_t rows = 3;
    std::size_t columns = 3;

    matrix::Matrix<double> mat_d(rows, columns);

    matrix::util::random_initialize(mat_d);

    // GCC's convention
    // d  - double, f  - float, i  - int, j - unsigned int, l  - long,
    // x  - long long, b  - bool
    std::cout << "mat_d type=" << typeid(decltype(mat_d)::value_type).name()
              << "\n";

    std::cout << "mat_d rows=" << mat_d.row() << ", columns=" << mat_d.col()
              << "\n";

    std::cout << "mat_d data\n" << mat_d << "\n";

    matrix::Matrix<unsigned int> mat_ui(rows, columns);

    matrix::util::random_initialize(mat_ui);

    std::cout << "mat_ui type=" << typeid(decltype(mat_ui)::value_type).name()
              << "\n";

    std::cout << "mat_ui rows=" << mat_ui.row() << ", columns=" << mat_ui.col()
              << "\n";

    std::cout << "mat_ui data\n" << mat_ui << "\n";

    return 0;
}
#include "matrix.hh"
#include "safe_numerics_typedefs.hh"

#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <numeric>
#include <sstream>
#include <type_traits>
#include <utility>

namespace cherry_blazer {

// Matrix can be constructed at compile-time, and in many ways.
TEST(MatrixCtorTest, MatrixConstexprCtors) { // NOLINT
    [[maybe_unused]] constexpr Matrix<2, 3> mat1{{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}}};
    [[maybe_unused]] constexpr Matrix<2, 3> mat2{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}};
    [[maybe_unused]] constexpr Matrix<2, 3> mat3{{1.0, 2.0, 3.0, 4.0, 5.0, 6.0}};
    [[maybe_unused]] constexpr Matrix<2, 3> mat4{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
}

class MatrixTest : public ::testing::Test {
  protected:
    MatrixTest()
        : mat0{2, 3}, mat1{{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}}}, mat2{{{1.0, 2.0, 3.0},
                                                                        {4.0, 5.0, 6.0}}},
          mat3{{1.0, 2.0, 3.0, 4.0, 5.0, 6.0}}, mat4{1.0, 2.0, 3.0, 4.0, 5.0, 6.0} {
        std::iota(std::begin(mat0), std::end(mat0), 0.0);
    }

    Matrix<0, 0> mat0; // runtime
    Matrix<2, 3> mat1;
    Matrix<2, 3> mat2;
    Matrix<2, 3> mat3;
    Matrix<2, 3> mat4;
};

// Matrix can be printed out.
TEST_F(MatrixTest, MatrixOutput) { // NOLINT
    std::stringstream ss;
    std::vector matrices{&mat1, &mat2, &mat3, &mat4};
    std::transform(std::cbegin(matrices), std::prev(std::cend(matrices)),
                   std::ostream_iterator<decltype(std::as_const(*(matrices[0])))>{ss, ", "},
                   [](auto mat) { return *mat; });
    ss << **std::prev(std::cend(matrices));
    std::cout << ss.str() << std::endl;
}

} // namespace cherry_blazer

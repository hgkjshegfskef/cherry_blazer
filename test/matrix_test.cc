#include "matrix.hh"
#include "safe_numerics_typedefs.hh"

#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

using namespace std::string_literals;

namespace cherry_blazer {

// Matrix can be constructed at compile-time, and in many ways.
TEST(MatrixCtorTest, MatrixConstexprCtors) { // NOLINT
    [[maybe_unused]] constexpr Matrix<2, 3> mat{{1, 2, 3}, {4, 5, 6}};
}

class MatrixTest : public ::testing::Test {
  protected:
    MatrixTest() : mat0{2, 3}, mat1{{1, 2, 3}, {4, 5, 6}} {
        std::iota(std::begin(mat0), std::end(mat0), 0);
    }

    Matrix<0, 0> mat0; // runtime
    Matrix<2, 3> mat1;
};

// Matrix can be printed out.
TEST_F(MatrixTest, MatrixOutput) { // NOLINT
    std::stringstream ss;
    ss << mat1;
    auto expected = "{1, 2, 3, 4, 5, 6}"s;
    EXPECT_EQ(ss.str(), expected);
}

} // namespace cherry_blazer

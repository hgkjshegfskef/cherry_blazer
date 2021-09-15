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
    [[maybe_unused]] constexpr Matrix mat{{1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}};

TEST(MatrixEqualsTest, MatrixEquals) { // NOLINT
    constexpr Matrix mat1{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    constexpr Matrix mat2{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    EXPECT_EQ(mat1, mat2);
}

TEST(MatrixEqualsTest, MatrixNotEqual) { // NOLINT
    constexpr Matrix mat1{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    constexpr Matrix mat2{{2, 3, 4, 5}, {6, 7, 8, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};
    EXPECT_NE(mat1, mat2);
}

class MatrixTest : public ::testing::Test {
  protected:
    MatrixTest() : mat1{{1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}} {}
    Matrix<4, 4> mat1;
};

// Matrix can be printed out.
TEST_F(MatrixTest, MatrixOutput) { // NOLINT
    std::stringstream ss;
    ss << mat1;
    auto expected = "{1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4}"s;
    EXPECT_EQ(ss.str(), expected);
}

} // namespace cherry_blazer

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
    constexpr Matrix mat{
        {1, 2, 3, 4}, {5.5, 6.5, 7.5, 8.5}, {9, 10, 11, 12}, {13.5, 14.5, 15.5, 16.5}};
}

TEST(MatrixCtorTest, Mat2x2) { // NOLINT
    constexpr Matrix mat{{-3, 5}, {1, -2}};
    EXPECT_EQ(mat(0, 0), -3);
    EXPECT_EQ(mat(0, 1), 5);
    EXPECT_EQ(mat(1, 0), 1);
    EXPECT_EQ(mat(1, 1), -2);
}

TEST(MatrixCtorTest, Mat3x3) { // NOLINT
    constexpr Matrix mat{{-3, 5, 0}, {1, -2, -7}, {0, 1, 1}};
    EXPECT_EQ(mat(0, 0), -3);
    EXPECT_EQ(mat(1, 1), -2);
    EXPECT_EQ(mat(2, 2), 1);
}

TEST(MatrixCtorTest, Mat4x4) { // NOLINT
    constexpr Matrix mat{
        {1, 2, 3, 4}, {5.5, 6.5, 7.5, 8.5}, {9, 10, 11, 12}, {13.5, 14.5, 15.5, 16.5}};
    EXPECT_EQ(mat(0, 0), 1);
    EXPECT_EQ(mat(0, 3), 4);
    EXPECT_EQ(mat(1, 0), 5.5);
    EXPECT_EQ(mat(1, 2), 7.5);
    EXPECT_EQ(mat(2, 2), 11);
    EXPECT_EQ(mat(3, 0), 13.5);
    EXPECT_EQ(mat(3, 2), 15.5);
}

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

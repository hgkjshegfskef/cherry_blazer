#include "matrix.hh"
#include "matrix_operations.hh"
#include "vector4d.hh"
#include "vector_properties.hh"

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
TEST(MatrixTest, MatrixConstexprCtors) { // NOLINT
    [[maybe_unused]] constexpr Matrix mat{
        {1., 2., 3., 4.}, {5.5, 6.5, 7.5, 8.5}, {9., 10., 11., 12.}, {13.5, 14.5, 15.5, 16.5}};
}

TEST(MatrixTest, Mat2x2) { // NOLINT
    constexpr Matrix mat{{-3., 5.}, {1., -2.}};
    EXPECT_EQ(mat(0, 0), -3);
    EXPECT_EQ(mat(0, 1), 5);
    EXPECT_EQ(mat(1, 0), 1);
    EXPECT_EQ(mat(1, 1), -2);
}

TEST(MatrixTest, Mat3x3) { // NOLINT
    constexpr Matrix mat{{-3., 5., 0.}, {1., -2., -7.}, {0., 1., 1.}};
    EXPECT_EQ(mat(0, 0), -3);
    EXPECT_EQ(mat(1, 1), -2);
    EXPECT_EQ(mat(2, 2), 1);
}

TEST(MatrixTest, Mat4x4) { // NOLINT
    constexpr Matrix mat{
        {1., 2., 3., 4.}, {5.5, 6.5, 7.5, 8.5}, {9., 10., 11., 12.}, {13.5, 14.5, 15.5, 16.5}};
    EXPECT_EQ(mat(0, 0), 1);
    EXPECT_EQ(mat(0, 3), 4);
    EXPECT_EQ(mat(1, 0), 5.5);
    EXPECT_EQ(mat(1, 2), 7.5);
    EXPECT_EQ(mat(2, 2), 11);
    EXPECT_EQ(mat(3, 0), 13.5);
    EXPECT_EQ(mat(3, 2), 15.5);
}

TEST(MatrixTest, MatrixEquals) { // NOLINT
    constexpr Matrix mat1{{1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 8., 7., 6.}, {5., 4., 3., 2.}};
    constexpr Matrix mat2{{1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 8., 7., 6.}, {5., 4., 3., 2.}};
    EXPECT_EQ(mat1, mat2);
}

TEST(MatrixTest, MatrixNotEqual) { // NOLINT
    constexpr Matrix mat1{{1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 8., 7., 6.}, {5., 4., 3., 2.}};
    constexpr Matrix mat2{{2., 3., 4., 5.}, {6., 7., 8., 9.}, {8., 7., 6., 5.}, {4., 3., 2., 1.}};
    EXPECT_NE(mat1, mat2);
}

// Matrix can be printed out.
TEST(MatrixTest, MatrixOutput) { // NOLINT
    constexpr Matrix mat{{1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 8., 7., 6.}, {5., 4., 3., 2.}};
    std::stringstream ss;
    ss << mat;
    EXPECT_EQ(ss.str(), "{1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2}"s);
}

// TODO: compile-time test that different sizes don't compile
TEST(MatrixTest, MatrixTimesMatrix) { // NOLINT
    constexpr Matrix mat1{{1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 8., 7., 6.}, {5., 4., 3., 2.}};
    constexpr Matrix mat2{{-2., 1., 2., 3.}, {3., 2., 1., -1.}, {4., 3., 6., 5.}, {1., 2., 7., 8.}};
    constexpr Matrix result = mat1 * mat2;
    EXPECT_EQ(result, (Mat4d{{20., 22., 50., 48.},
                             {44., 54., 114., 108.},
                             {40., 58., 110., 102.},
                             {16., 26., 46., 42.}}));
}

TEST(MatrixTest, MatrixTimesVector) { // NOLINT
    constexpr Matrix mat{{1., 2., 3., 4.}, {2., 4., 4., 2.}, {8., 6., 4., 1.}, {0., 0., 0., 1.}};
    constexpr Vector vec{1., 2., 3., 1.};
    constexpr auto result = mat * vec;
    EXPECT_EQ(result, (Vec4d{18, 24, 33, 1}));
}

TEST(MatrixTest, MatrixIdentityMatrix) { // NOLINT
    constexpr auto result = matrix::identity<double, 4>();
    EXPECT_EQ(result, (Mat4d{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
}

TEST(MatrixTest, MatrixIdentityMatrixFrom) { // NOLINT
    constexpr auto result1 = matrix::identity<double, 4>();
    constexpr Matrix mat{{0., 1., 2., 4.}, {1., 2., 4., 8.}, {2., 4., 8., 16.}, {4., 8., 16., 32.}};
    constexpr auto result2 = matrix::identity<decltype(mat)::value_type, decltype(mat)::row_size>();
    constexpr auto result3 = matrix::identity(mat);
    EXPECT_EQ(result1, (Mat4d{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
    EXPECT_EQ(result2, (Mat4d{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
    EXPECT_EQ(result3, (Mat4d{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
}

TEST(MatrixTest, MatrixTimesIdentityMatrix) { // NOLINT
    constexpr Matrix mat{{0., 1., 2., 4.}, {1., 2., 4., 8.}, {2., 4., 8., 16.}, {4., 8., 16., 32.}};
    constexpr auto identity = matrix::identity(mat);
    constexpr auto result = mat * identity;
    EXPECT_EQ(result, (Mat4d{{0, 1, 2, 4}, {1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}}));
}

TEST(MatrixTest, IdentityMatrixTimesVector) { // NOLINT
    constexpr Matrix identity = matrix::identity<double, 4>();
    constexpr Vector vec{1., 2., 3., 4.};
    constexpr auto result = identity * vec;
    EXPECT_EQ(result, (Vec4d{1, 2, 3, 4}));
}

} // namespace cherry_blazer

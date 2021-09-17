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
    constexpr auto result = Matrix<double, 4, 4>::identity();
    EXPECT_EQ(result, (Mat4d{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
}

TEST(MatrixTest, MatrixIdentityMatrixFrom) { // NOLINT
    constexpr Matrix mat{{0., 1., 2., 4.}, {1., 2., 4., 8.}, {2., 4., 8., 16.}, {4., 8., 16., 32.}};
    constexpr auto result = decltype(mat)::identity();
    EXPECT_EQ(result, (Mat4d{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
}

TEST(MatrixTest, MatrixTimesIdentityMatrix) { // NOLINT
    constexpr Matrix mat{{0., 1., 2., 4.}, {1., 2., 4., 8.}, {2., 4., 8., 16.}, {4., 8., 16., 32.}};
    constexpr auto identity = decltype(mat)::identity();
    constexpr auto result = mat * identity;
    EXPECT_EQ(result, (Mat4d{{0, 1, 2, 4}, {1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}}));
}

TEST(MatrixTest, IdentityMatrixTimesVector) { // NOLINT
    constexpr Matrix identity = Matrix<double, 4, 4>::identity();
    constexpr Vector vec{1., 2., 3., 4.};
    constexpr auto result = identity * vec;
    EXPECT_EQ(result, (Vec4d{1, 2, 3, 4}));
}

TEST(MatrixTest, TransposeMatrix) { // NOLINT
    constexpr Matrix mat{{0., 9., 3., 0.}, {9., 8., 0., 8.}, {1., 8., 5., 3.}, {0., 0., 5., 8.}};
    constexpr auto result = transpose(mat);
    EXPECT_EQ(result,
              (Mat4d{{0., 9., 1., 0.}, {9., 8., 8., 0.}, {3., 0., 5., 5.}, {0., 8., 3., 8.}}));
}

TEST(MatrixTest, TransposeIdentityMatrix) { // NOLINT
    constexpr auto identity = Matrix<double, 4, 4>::identity();
    constexpr auto result = transpose(identity);
    EXPECT_EQ(result, (Mat4d{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
}

TEST(MatrixTest, Matrix2x2Determinant) { // NOLINT
    constexpr Matrix mat{{1., 5.}, {-3., 2.}};
    constexpr auto result = det(mat);
    EXPECT_EQ(result, 17);
}

TEST(MatrixTest, Matrix3x3Submatrix) { // NOLINT
    constexpr Matrix mat{{1., 5., 0.}, {-3., 2., 7.}, {0., 6., -3.}};
    constexpr auto result = submatrix(mat, 0, 2);
    EXPECT_EQ(result, (Mat2d{{-3, 2}, {0, 6}}));
}

TEST(MatrixTest, Matrix4x4Submatrix) { // NOLINT
    constexpr Matrix mat{
        {-6., 1., 1., 6.}, {-8., 5., 8., 6.}, {-1., 0., 8., 2.}, {-7., 1., -1., 1.}};
    constexpr auto result = submatrix(mat, 2, 1);
    EXPECT_EQ(result, (Mat3d{{-6, 1, 6}, {-8, 8, 6}, {-7, -1, 1}}));
}

TEST(MatrixTest, Matrix3x3Minor) { // NOLINT
    constexpr Matrix mat{{3., 5., 0.}, {2., -1., -7.}, {6., -1., 5.}};
    constexpr auto result = minor(mat, 1, 0);
    EXPECT_EQ(result, 25);
}

TEST(MatrixTest, Matrix3x3CofactorSignSame) { // NOLINT
    constexpr Matrix mat{{3., 5., 0.}, {2., -1., -7.}, {6., -1., 5.}};
    constexpr auto result = cofactor(mat, 0, 0);
    EXPECT_EQ(result, -12);
}

TEST(MatrixTest, Matrix3x3CofactorSignChanges) { // NOLINT
    constexpr Matrix mat{{3., 5., 0.}, {2., -1., -7.}, {6., -1., 5.}};
    constexpr auto result = cofactor(mat, 1, 0);
    EXPECT_EQ(result, -25);
}

} // namespace cherry_blazer

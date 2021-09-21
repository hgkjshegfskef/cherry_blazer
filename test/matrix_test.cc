#include "config.hh"
#include "matrix.hh"
#include "matrix_operations.hh"
#include "point_properties.hh"
#include "vector.hh"
#include "vector_properties.hh"

#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <numbers>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

using namespace std::string_literals;

namespace {
inline constexpr double abs_error = 1e-5;
}

namespace cherry_blazer {

// Matrix can be constructed at compile-time, and in many ways.
TEST(MatrixTest, MatrixConstexprCtors) { // NOLINT
    [[maybe_unused]] CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {1., 2., 3., 4.}, {5.5, 6.5, 7.5, 8.5}, {9., 10., 11., 12.}, {13.5, 14.5, 15.5, 16.5}};
}

TEST(MatrixTest, Mat2x2) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{{-3., 5.}, {1., -2.}};
    EXPECT_EQ(mat(0, 0), -3);
    EXPECT_EQ(mat(0, 1), 5);
    EXPECT_EQ(mat(1, 0), 1);
    EXPECT_EQ(mat(1, 1), -2);
}

TEST(MatrixTest, Mat3x3) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{{-3., 5., 0.}, {1., -2., -7.}, {0., 1., 1.}};
    EXPECT_EQ(mat(0, 0), -3);
    EXPECT_EQ(mat(1, 1), -2);
    EXPECT_EQ(mat(2, 2), 1);
}

TEST(MatrixTest, Mat4x4) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
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
    CHERRY_BLAZER_CONSTEXPR Matrix mat1{
        {1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 8., 7., 6.}, {5., 4., 3., 2.}};
    CHERRY_BLAZER_CONSTEXPR Matrix mat2{
        {1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 8., 7., 6.}, {5., 4., 3., 2.}};
    EXPECT_EQ(mat1, mat2);
}

TEST(MatrixTest, MatrixNotEqual) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat1{
        {1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 8., 7., 6.}, {5., 4., 3., 2.}};
    CHERRY_BLAZER_CONSTEXPR Matrix mat2{
        {2., 3., 4., 5.}, {6., 7., 8., 9.}, {8., 7., 6., 5.}, {4., 3., 2., 1.}};
    EXPECT_NE(mat1, mat2);
}

// Matrix can be printed out.
TEST(MatrixTest, MatrixOutput) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 8., 7., 6.}, {5., 4., 3., 2.}};
    std::stringstream ss;
    ss << mat;
    auto expected = "{ 1, 2, 3, 4 }\n"
                    "{ 5, 6, 7, 8 }\n"
                    "{ 9, 8, 7, 6 }\n"
                    "{ 5, 4, 3, 2 }"s;
    EXPECT_EQ(ss.str(), expected);
}

// TODO: compile-time test that different sizes don't compile
TEST(MatrixTest, MatrixTimesMatrix) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat1{
        {1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 8., 7., 6.}, {5., 4., 3., 2.}};
    CHERRY_BLAZER_CONSTEXPR Matrix mat2{
        {-2., 1., 2., 3.}, {3., 2., 1., -1.}, {4., 3., 6., 5.}, {1., 2., 7., 8.}};
    CHERRY_BLAZER_CONSTEXPR Matrix result = mat1 * mat2;
    EXPECT_EQ(result, (Mat4d{{20., 22., 50., 48.},
                             {44., 54., 114., 108.},
                             {40., 58., 110., 102.},
                             {16., 26., 46., 42.}}));
}

TEST(MatrixTest, MatrixTimesVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {1., 2., 3., 4.}, {2., 4., 4., 2.}, {8., 6., 4., 1.}, {0., 0., 0., 1.}};
    CHERRY_BLAZER_CONSTEXPR Vector vec{1., 2., 3., 1.};
    CHERRY_BLAZER_CONSTEXPR auto result = mat * vec;
    EXPECT_EQ(result, (Vec4d{18, 24, 33, 1}));
}

TEST(MatrixTest, MatrixIdentityMatrix) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR auto result = Matrix<double, 4, 4>::identity();
    EXPECT_EQ(result, (Mat4d{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
}

TEST(MatrixTest, MatrixIdentityMatrixFrom) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {0., 1., 2., 4.}, {1., 2., 4., 8.}, {2., 4., 8., 16.}, {4., 8., 16., 32.}};
    CHERRY_BLAZER_CONSTEXPR auto result = decltype(mat)::identity();
    EXPECT_EQ(result, (Mat4d{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
}

TEST(MatrixTest, MatrixTimesIdentityMatrix) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {0., 1., 2., 4.}, {1., 2., 4., 8.}, {2., 4., 8., 16.}, {4., 8., 16., 32.}};
    CHERRY_BLAZER_CONSTEXPR auto identity = decltype(mat)::identity();
    CHERRY_BLAZER_CONSTEXPR auto result = mat * identity;
    EXPECT_EQ(result, (Mat4d{{0, 1, 2, 4}, {1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}}));
}

TEST(MatrixTest, IdentityMatrixTimesVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix identity = Matrix<double, 4, 4>::identity();
    CHERRY_BLAZER_CONSTEXPR Vector vec{1., 2., 3., 4.};
    CHERRY_BLAZER_CONSTEXPR auto result = identity * vec;
    EXPECT_EQ(result, (Vec4d{1, 2, 3, 4}));
}

TEST(MatrixTest, TransposeMatrix) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {0., 9., 3., 0.}, {9., 8., 0., 8.}, {1., 8., 5., 3.}, {0., 0., 5., 8.}};
    CHERRY_BLAZER_CONSTEXPR auto result = transpose(mat);
    EXPECT_EQ(result,
              (Mat4d{{0., 9., 1., 0.}, {9., 8., 8., 0.}, {3., 0., 5., 5.}, {0., 8., 3., 8.}}));
}

TEST(MatrixTest, TransposeIdentityMatrix) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR auto identity = Matrix<double, 4, 4>::identity();
    CHERRY_BLAZER_CONSTEXPR auto result = transpose(identity);
    EXPECT_EQ(result, (Mat4d{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}));
}

TEST(MatrixTest, Matrix2x2Determinant) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{{1., 5.}, {-3., 2.}};
    CHERRY_BLAZER_CONSTEXPR auto result = det(mat);
    EXPECT_EQ(result, 17);
}

TEST(MatrixTest, Matrix3x3Submatrix) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{{1., 5., 0.}, {-3., 2., 7.}, {0., 6., -3.}};
    CHERRY_BLAZER_CONSTEXPR auto result = submatrix(mat, 0, 2);
    EXPECT_EQ(result, (Mat2d{{-3, 2}, {0, 6}}));
}

TEST(MatrixTest, Matrix4x4Submatrix) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {-6., 1., 1., 6.}, {-8., 5., 8., 6.}, {-1., 0., 8., 2.}, {-7., 1., -1., 1.}};
    CHERRY_BLAZER_CONSTEXPR auto result = submatrix(mat, 2, 1);
    EXPECT_EQ(result, (Mat3d{{-6, 1, 6}, {-8, 8, 6}, {-7, -1, 1}}));
}

TEST(MatrixTest, Matrix3x3Minor) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{{3., 5., 0.}, {2., -1., -7.}, {6., -1., 5.}};
    ASSERT_EQ(det(submatrix(mat, 1, 0)), 25);
    CHERRY_BLAZER_CONSTEXPR auto result = minor(mat, 1, 0);
    EXPECT_EQ(result, 25);
}

TEST(MatrixTest, Matrix3x3CofactorSignSame) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{{3., 5., 0.}, {2., -1., -7.}, {6., -1., 5.}};
    ASSERT_EQ(minor(mat, 0, 0), -12);
    CHERRY_BLAZER_CONSTEXPR auto result = cofactor(mat, 0, 0);
    EXPECT_EQ(result, -12);
}

TEST(MatrixTest, Matrix3x3CofactorSignChanges) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{{3., 5., 0.}, {2., -1., -7.}, {6., -1., 5.}};
    ASSERT_EQ(minor(mat, 1, 0), 25);
    CHERRY_BLAZER_CONSTEXPR auto result = cofactor(mat, 1, 0);
    EXPECT_EQ(result, -25);
}

TEST(MatrixTest, Matrix3x3Determinant) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{{1., 2., 6.}, {-5., 8., -4.}, {2., 6., 4.}};
    ASSERT_EQ(cofactor(mat, 0, 0), 56);
    ASSERT_EQ(cofactor(mat, 0, 1), 12);
    ASSERT_EQ(cofactor(mat, 0, 2), -46);
    CHERRY_BLAZER_CONSTEXPR auto result = det(mat);
    EXPECT_EQ(result, -196);
}

TEST(MatrixTest, Matrix4x4Determinant) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {-2., -8., 3., 5.}, {-3., 1., 7., 3.}, {1., 2., -9., 6.}, {-6., 7., 7., -9.}};
    ASSERT_EQ(cofactor(mat, 0, 0), 690);
    ASSERT_EQ(cofactor(mat, 0, 1), 447);
    ASSERT_EQ(cofactor(mat, 0, 2), 210);
    ASSERT_EQ(cofactor(mat, 0, 3), 51);
    CHERRY_BLAZER_CONSTEXPR auto result = det(mat);
    EXPECT_EQ(result, -4071);
}

TEST(MatrixTest, MatrixInvertible) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {6., 4., 4., 4.}, {5., 5., 7., 6.}, {4., -9., 3., -7.}, {9., 1., 7., -6.}};
    ASSERT_EQ(det(mat), -2120);
    CHERRY_BLAZER_CONSTEXPR auto result = invertible(mat);
    EXPECT_EQ(result, true);
}

TEST(MatrixTest, MatrixNonInvertible) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {-4., 2., -2., -3.}, {9., 6., 2., 6.}, {0., -5., 1., -5.}, {0., 0., 0., 0.}};
    ASSERT_EQ(det(mat), 0);
    CHERRY_BLAZER_CONSTEXPR auto result = invertible(mat);
    EXPECT_EQ(result, false);
}

TEST(MatrixTest, MatrixInverse1) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {-5., 2., 6., -8.}, {1., -5., 1., 8.}, {7., 7., -6., -7.}, {1., -3., 7., 4.}};
    ASSERT_EQ(det(mat), 532);
    ASSERT_EQ(cofactor(mat, 2, 3), -160);
    ASSERT_EQ(cofactor(mat, 3, 2), 105);
    CHERRY_BLAZER_CONSTEXPR auto result = inverse(mat);
    ASSERT_EQ(result(3, 2), -160. / 532.);
    ASSERT_EQ(result(2, 3), 105. / 532.);
    CHERRY_BLAZER_CONSTEXPR auto expected = Mat4d{{0.218045, 0.451128, 0.240602, -0.0451128},
                                                  {-0.808271, -1.45677, -0.443609, 0.520677},
                                                  {-0.0789474, -0.223684, -0.0526316, 0.197368},
                                                  {-0.522556, -0.81391, -0.300752, 0.306391}};
    for (auto row{0U}; row < decltype(result)::row_size; ++row) {
        for (auto col{0U}; col < decltype(result)::col_size; ++col)
            EXPECT_NEAR(result(row, col), expected(row, col), abs_error) << result;
    }
}

TEST(MatrixTest, MatrixInverse2) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {8., -5., 9., 2.}, {7., 5., 6., 1.}, {-6., 0., 9., 6.}, {-3., 0., -9., -4.}};
    CHERRY_BLAZER_CONSTEXPR auto result = inverse(mat);
    CHERRY_BLAZER_CONSTEXPR auto expected = Mat4d{{-0.15385, -0.15385, -0.28205, -0.53846},
                                                  {-0.07692, 0.12308, 0.02564, 0.03077},
                                                  {0.35897, 0.35897, 0.43590, 0.92308},
                                                  {-0.69231, -0.69231, -0.76923, -1.92308}};
    for (auto row{0U}; row < decltype(result)::row_size; ++row) {
        for (auto col{0U}; col < decltype(result)::col_size; ++col)
            EXPECT_NEAR(result(row, col), expected(row, col), abs_error) << result;
    }
}

TEST(MatrixTest, MatrixInverse3) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix mat{
        {9., 3., 0., 9.}, {-5., -2., -6., -3.}, {-4., 9., 6., 4.}, {-7., 6., 6., 2.}};
    CHERRY_BLAZER_CONSTEXPR auto result = inverse(mat);
    CHERRY_BLAZER_CONSTEXPR auto expected = Mat4d{{-0.04074, -0.07778, 0.14444, -0.22222},
                                                  {-0.07778, 0.03333, 0.36667, -0.33333},
                                                  {-0.02901, -0.14630, -0.10926, 0.12963},
                                                  {0.17778, 0.06667, -0.26667, 0.33333}};
    for (auto row{0U}; row < decltype(result)::row_size; ++row) {
        for (auto col{0U}; col < decltype(result)::col_size; ++col)
            EXPECT_NEAR(result(row, col), expected(row, col), abs_error) << result;
    }
}

TEST(MatrixTest, MultiplyProductByItsInverse) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix a{
        {3., -9., 7., 3.}, {3., -8., 2., -9.}, {-4., 4., 4., 1.}, {-6., 5., -1., 1.}};
    CHERRY_BLAZER_CONSTEXPR Matrix b{
        {8., 2., 2., 2.}, {3., -1., 7., 0.}, {7., 0., 5., 4.}, {6., -2., 0., 5.}};
    CHERRY_BLAZER_CONSTEXPR auto c = a * b;
    CHERRY_BLAZER_CONSTEXPR auto result = c * inverse(b);
    for (auto row{0U}; row < decltype(result)::row_size; ++row) {
        for (auto col{0U}; col < decltype(result)::col_size; ++col)
            EXPECT_NEAR(result(row, col), a(row, col), abs_error) << result;
    }
}

TEST(MatrixTest, IdentityMatrixInvertion) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR auto identity = Matrix<double, 4, 4>::identity();
    CHERRY_BLAZER_CONSTEXPR auto result = inverse(identity);
    for (auto row{0U}; row < decltype(result)::row_size; ++row) {
        for (auto col{0U}; col < decltype(result)::col_size; ++col)
            EXPECT_NEAR(result(row, col), identity(row, col), abs_error) << result;
    }
}

TEST(MatrixTest, MultiplyMatrixByItsInverse) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix a{
        {3., -9., 7., 3.}, {3., -8., 2., -9.}, {-4., 4., 4., 1.}, {-6., 5., -1., 1.}};
    CHERRY_BLAZER_CONSTEXPR auto inverted_a = inverse(a);
    CHERRY_BLAZER_CONSTEXPR auto result = a * inverted_a;
    CHERRY_BLAZER_CONSTEXPR auto identity = decltype(a)::identity();
    for (auto row{0U}; row < decltype(result)::row_size; ++row) {
        for (auto col{0U}; col < decltype(result)::col_size; ++col)
            EXPECT_NEAR(result(row, col), identity(row, col), abs_error) << result;
    }
}

TEST(MatrixTest, InverseOfTransposeVsTransposeOfInverse) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix a{
        {3., -9., 7., 3.}, {3., -8., 2., -9.}, {-4., 4., 4., 1.}, {-6., 5., -1., 1.}};
    CHERRY_BLAZER_CONSTEXPR auto inverted_a = inverse(a);
    CHERRY_BLAZER_CONSTEXPR auto transposed_inverse_of_a = transpose(inverted_a);
    CHERRY_BLAZER_CONSTEXPR auto transposed_a = transpose(a);
    CHERRY_BLAZER_CONSTEXPR auto inverted_transpose_of_a = inverse(transposed_a);
    for (auto row{0U}; row < decltype(a)::row_size; ++row) {
        for (auto col{0U}; col < decltype(a)::col_size; ++col) {
            EXPECT_NEAR(transposed_inverse_of_a(row, col), inverted_transpose_of_a(row, col),
                        abs_error);
        }
    }
}

TEST(MatrixTest, TranslationMatrixTimesPoint) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{-3., 4., 5.};
    CHERRY_BLAZER_CONSTEXPR auto translation_matrix =
        Matrix<decltype(original_point)::value_type, decltype(original_point)::size + 1,
               decltype(original_point)::size + 1>::translation(Vector{5., -3., 2.});
    CHERRY_BLAZER_CONSTEXPR auto translated_point = translate(translation_matrix, original_point);
    CHERRY_BLAZER_CONSTEXPR Point expected{2., 1., 7.};
    EXPECT_EQ(translated_point, expected);
}

TEST(MatrixTest, ScalingMatrixTimesPoint) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{-4., 6., 8.};
    CHERRY_BLAZER_CONSTEXPR auto scaling_matrix =
        Matrix<decltype(original_point)::value_type, decltype(original_point)::size + 1,
               decltype(original_point)::size + 1>::scaling(Vector{2., 3., 4.});
    CHERRY_BLAZER_CONSTEXPR auto scaled_point = scale(scaling_matrix, original_point);
    CHERRY_BLAZER_CONSTEXPR Point expected{-8., 18., 32.};
    EXPECT_EQ(scaled_point, expected);
}

TEST(MatrixTest, ScalingMatrixTimesVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector original_vector{-4., 6., 8.};
    CHERRY_BLAZER_CONSTEXPR auto scaling_matrix =
        Matrix<decltype(original_vector)::value_type, decltype(original_vector)::size + 1,
               decltype(original_vector)::size + 1>::scaling(Vector{2., 3., 4.});
    CHERRY_BLAZER_CONSTEXPR auto scaled_vector = scale(scaling_matrix, original_vector);
    CHERRY_BLAZER_CONSTEXPR Vector expected{-8., 18., 32.};
    EXPECT_EQ(scaled_vector, expected);
}

TEST(MatrixTest, InverseOfScalingMatrixTimesVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector original_vector{-4., 6., 8.};
    CHERRY_BLAZER_CONSTEXPR auto inversed_scaling_matrix =
        inverse(Matrix<decltype(original_vector)::value_type, decltype(original_vector)::size + 1,
                       decltype(original_vector)::size + 1>::scaling(Vector{2., 3., 4.}));
    CHERRY_BLAZER_CONSTEXPR auto shrinked_vector = scale(inversed_scaling_matrix, original_vector);
    CHERRY_BLAZER_CONSTEXPR Vector expected{-2., 2., 2.};
    EXPECT_EQ(shrinked_vector, expected);
}

TEST(MatrixTest, ReflectionIsScalingByNegativeValue) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{2., 3., 4.};
    CHERRY_BLAZER_CONSTEXPR auto reflection_matrix =
        Matrix<decltype(original_point)::value_type, decltype(original_point)::size + 1,
               decltype(original_point)::size + 1>::scaling(Vector{-1., 1., 1.});
    CHERRY_BLAZER_CONSTEXPR auto reflected_point = scale(reflection_matrix, original_point);
    CHERRY_BLAZER_CONSTEXPR Point expected{-2., 3., 4.};
    EXPECT_EQ(reflected_point, expected);
}

TEST(MatrixTest, RotatingPointAroundXAxisHalfQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 1., 0.};
    CHERRY_BLAZER_CONSTEXPR auto half_quarter_rotation_matrix =
        Matrix<decltype(original_point)::value_type, decltype(original_point)::size + 1,
               decltype(original_point)::size +
                   1>::rotation(Axis::X, pi_v<decltype(original_point)::value_type> / 4);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point =
        rotate(half_quarter_rotation_matrix, original_point);
    CHERRY_BLAZER_CONSTEXPR Point expected{0., sqrt2_v<decltype(original_point)::value_type> / 2,
                                           sqrt2_v<decltype(original_point)::value_type> / 2};
    for (auto row{0U}; row < decltype(original_point)::size; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST(MatrixTest, RotatingPointAroundXAxisFullQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 1., 0.};
    CHERRY_BLAZER_CONSTEXPR auto full_quarter_rotation_matrix =
        Matrix<decltype(original_point)::value_type, decltype(original_point)::size + 1,
               decltype(original_point)::size +
                   1>::rotation(Axis::X, pi_v<decltype(original_point)::value_type> / 2);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point =
        rotate(full_quarter_rotation_matrix, original_point);
    CHERRY_BLAZER_CONSTEXPR Point expected{0., 0., 1.};
    for (auto row{0U}; row < decltype(original_point)::size; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST(MatrixTest, InverseOfXAxisRotationRotatesInOppositeDirection) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 1., 0.};
    CHERRY_BLAZER_CONSTEXPR auto half_quarter_rotation_matrix =
        Matrix<decltype(original_point)::value_type, decltype(original_point)::size + 1,
               decltype(original_point)::size +
                   1>::rotation(Axis::X, pi_v<decltype(original_point)::value_type> / 4);
    CHERRY_BLAZER_CONSTEXPR auto inversed_half_quarter_rotation_matrix =
        inverse(half_quarter_rotation_matrix);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point =
        rotate(inversed_half_quarter_rotation_matrix, original_point);
    CHERRY_BLAZER_CONSTEXPR Point expected{0., sqrt2_v<decltype(original_point)::value_type> / 2,
                                           -sqrt2_v<decltype(original_point)::value_type> / 2};
    for (auto row{0U}; row < decltype(original_point)::size; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST(MatrixTest, RotatingPointAroundYAxisHalfQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 0., 1.};
    CHERRY_BLAZER_CONSTEXPR auto half_quarter_rotation_matrix =
        Matrix<decltype(original_point)::value_type, decltype(original_point)::size + 1,
               decltype(original_point)::size +
                   1>::rotation(Axis::Y, pi_v<decltype(original_point)::value_type> / 4);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point =
        rotate(half_quarter_rotation_matrix, original_point);
    CHERRY_BLAZER_CONSTEXPR Point expected{sqrt2_v<decltype(original_point)::value_type> / 2, 0.,
                                           sqrt2_v<decltype(original_point)::value_type> / 2};
    for (auto row{0U}; row < decltype(original_point)::size; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST(MatrixTest, RotatingPointAroundYAxisFullQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 0., 1.};
    CHERRY_BLAZER_CONSTEXPR auto full_quarter_rotation_matrix =
        Matrix<decltype(original_point)::value_type, decltype(original_point)::size + 1,
               decltype(original_point)::size +
                   1>::rotation(Axis::Y, pi_v<decltype(original_point)::value_type> / 2);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point =
        rotate(full_quarter_rotation_matrix, original_point);
    CHERRY_BLAZER_CONSTEXPR Point expected{1., 0., 0.};
    for (auto row{0U}; row < decltype(original_point)::size; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST(MatrixTest, RotatingPointAroundZAxisHalfQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 1., 0.};
    CHERRY_BLAZER_CONSTEXPR auto half_quarter_rotation_matrix =
        Matrix<decltype(original_point)::value_type, decltype(original_point)::size + 1,
               decltype(original_point)::size +
                   1>::rotation(Axis::Z, pi_v<decltype(original_point)::value_type> / 4);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point =
        rotate(half_quarter_rotation_matrix, original_point);
    CHERRY_BLAZER_CONSTEXPR Point expected{-sqrt2_v<decltype(original_point)::value_type> / 2,
                                           sqrt2_v<decltype(original_point)::value_type> / 2, 0.};
    for (auto row{0U}; row < decltype(original_point)::size; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST(MatrixTest, RotatingPointAroundZAxisFullQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 1., 0.};
    CHERRY_BLAZER_CONSTEXPR auto full_quarter_rotation_matrix =
        Matrix<decltype(original_point)::value_type, decltype(original_point)::size + 1,
               decltype(original_point)::size +
                   1>::rotation(Axis::Z, pi_v<decltype(original_point)::value_type> / 2);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point =
        rotate(full_quarter_rotation_matrix, original_point);
    CHERRY_BLAZER_CONSTEXPR Point expected{-1., 0., 0.};
    for (auto row{0U}; row < decltype(original_point)::size; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

} // namespace cherry_blazer

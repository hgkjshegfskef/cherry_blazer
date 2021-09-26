#include <cherry_blazer/config.hh>
#include <cherry_blazer/matrix.hh>
#include <cherry_blazer/matrix_operations.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/shearing.hh>
#include <cherry_blazer/vector.hh>

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <numbers>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

using namespace std::string_literals;

using cherry_blazer::Axis;
using cherry_blazer::Mat2d;
using cherry_blazer::Mat3d;
using cherry_blazer::Mat4d;
using cherry_blazer::Matrix;
using cherry_blazer::Point;
using cherry_blazer::ShearDirection;
// using cherry_blazer::Vec4d;
// using cherry_blazer::Vector;
using cherry_blazer::Shear::X;
using cherry_blazer::Shear::Y;
using cherry_blazer::Shear::Z;

namespace {
inline constexpr double abs_error = 1e-5;
}

class TranslationMatrixTest : public testing::Test {
  protected:
    using T = double;                          // NOLINT(readability-identifier-naming)
    constexpr static inline std::size_t D = 4; // NOLINT(readability-identifier-naming)
};

TEST_F(TranslationMatrixTest, TranslationMatrixTimesPoint) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{-3., 4., 5.};

    CHERRY_BLAZER_CONSTEXPR auto translation_matrix = Matrix<T, D, D>::translation({5., -3., 2.});
    CHERRY_BLAZER_CONSTEXPR auto translated_point = translation_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{2., 1., 7.};
    EXPECT_EQ(translated_point, expected);
}

TEST_F(TranslationMatrixTest, ScalingMatrixTimesPoint) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{-4., 6., 8.};

    CHERRY_BLAZER_CONSTEXPR auto scaling_matrix = Matrix<T, D, D>::scaling({2., 3., 4.});
    CHERRY_BLAZER_CONSTEXPR auto scaled_point = scaling_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{-8., 18., 32.};
    EXPECT_EQ(scaled_point, expected);
}

TEST_F(TranslationMatrixTest, ScalingMatrixTimesVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix original_vector{-4., 6., 8.};

    CHERRY_BLAZER_CONSTEXPR auto scaling_matrix = Matrix<T, D, D>::scaling({2., 3., 4.});
    CHERRY_BLAZER_CONSTEXPR auto scaled_vector = scaling_matrix * original_vector;

    CHERRY_BLAZER_CONSTEXPR Matrix expected{-8., 18., 32.};
    EXPECT_EQ(scaled_vector, expected);
}

TEST_F(TranslationMatrixTest, InverseOfScalingMatrixTimesVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix original_vector{-4., 6., 8.};

    CHERRY_BLAZER_CONSTEXPR auto inverse_of_scaling_matrix =
        inverse(Matrix<T, D, D>::scaling({2., 3., 4.}));
    CHERRY_BLAZER_CONSTEXPR auto shrunk_vector = inverse_of_scaling_matrix * original_vector;

    CHERRY_BLAZER_CONSTEXPR Matrix expected{-2., 2., 2.};
    EXPECT_EQ(shrunk_vector, expected);
}

TEST_F(TranslationMatrixTest, ReflectionIsScalingByNegativeValue) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{2., 3., 4.};

    CHERRY_BLAZER_CONSTEXPR auto reflection_matrix = Matrix<T, D, D>::scaling({-1., 1., 1.});
    CHERRY_BLAZER_CONSTEXPR auto reflected_point = reflection_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{-2., 3., 4.};
    EXPECT_EQ(reflected_point, expected);
}

TEST_F(TranslationMatrixTest, RotatingPointAroundXAxisHalfQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 1., 0.};

    CHERRY_BLAZER_CONSTEXPR auto half_quarter_rotation_matrix =
        Matrix<T, D, D>::rotation(Axis::X, pi_v<T> / 4);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point = half_quarter_rotation_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{0., sqrt2_v<T> / 2, sqrt2_v<T> / 2};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, RotatingPointAroundXAxisFullQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 1., 0.};

    CHERRY_BLAZER_CONSTEXPR auto full_quarter_rotation_matrix =
        Matrix<T, D, D>::rotation(Axis::X, pi_v<T> / 2);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point = full_quarter_rotation_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{0., 0., 1.};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, InverseOfXAxisRotationRotatesInOppositeDirection) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 1., 0.};

    CHERRY_BLAZER_CONSTEXPR auto half_quarter_rotation_matrix =
        Matrix<T, D, D>::rotation(Axis::X, pi_v<T> / 4);
    CHERRY_BLAZER_CONSTEXPR auto inverted_half_quarter_rotation_matrix =
        inverse(half_quarter_rotation_matrix);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point =
        inverted_half_quarter_rotation_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{0., sqrt2_v<T> / 2, -sqrt2_v<T> / 2};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, RotatingPointAroundYAxisHalfQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 0., 1.};

    CHERRY_BLAZER_CONSTEXPR auto half_quarter_rotation_matrix =
        Matrix<T, D, D>::rotation(Axis::Y, pi_v<T> / 4);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point = half_quarter_rotation_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{sqrt2_v<T> / 2, 0., sqrt2_v<T> / 2};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, RotatingPointAroundYAxisFullQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 0., 1.};

    CHERRY_BLAZER_CONSTEXPR auto full_quarter_rotation_matrix =
        Matrix<T, D, D>::rotation(Axis::Y, pi_v<T> / 2);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point = full_quarter_rotation_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{1., 0., 0.};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, RotatingPointAroundZAxisHalfQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 1., 0.};

    CHERRY_BLAZER_CONSTEXPR auto half_quarter_rotation_matrix =
        Matrix<T, D, D>::rotation(Axis::Z, pi_v<T> / 4);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point = half_quarter_rotation_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{-sqrt2_v<T> / 2, sqrt2_v<T> / 2, 0.};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, RotatingPointAroundZAxisFullQuarter) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{0., 1., 0.};

    CHERRY_BLAZER_CONSTEXPR auto full_quarter_rotation_matrix =
        Matrix<T, D, D>::rotation(Axis::Z, pi_v<T> / 2);
    CHERRY_BLAZER_CONSTEXPR auto rotated_point = full_quarter_rotation_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{-1., 0., 0.};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(rotated_point[row], expected[row], abs_error)
            << "Got:\t\t" << rotated_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, ShearingPointComponentXAgainstY) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{2., 3., 4.};

    CHERRY_BLAZER_CONSTEXPR ShearDirection direction{X::AgainstY{}};
    CHERRY_BLAZER_CONSTEXPR auto shearing_matrix = Matrix<T, D, D>::shearing(direction);
    CHERRY_BLAZER_CONSTEXPR auto sheared_point = shearing_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{5., 3., 4.};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(sheared_point[row], expected[row], abs_error)
            << "Got:\t\t" << sheared_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, ShearingPointComponentXAgainstZ) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{2., 3., 4.};

    CHERRY_BLAZER_CONSTEXPR ShearDirection direction{X::AgainstZ{}};
    CHERRY_BLAZER_CONSTEXPR auto shearing_matrix = Matrix<T, D, D>::shearing(direction);
    CHERRY_BLAZER_CONSTEXPR auto sheared_point = shearing_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{6., 3., 4.};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(sheared_point[row], expected[row], abs_error)
            << "Got:\t\t" << sheared_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, ShearingPointComponentYAgainstX) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{2., 3., 4.};

    CHERRY_BLAZER_CONSTEXPR ShearDirection direction{Y::AgainstX{}};
    CHERRY_BLAZER_CONSTEXPR auto shearing_matrix = Matrix<T, D, D>::shearing(direction);
    CHERRY_BLAZER_CONSTEXPR auto sheared_point = shearing_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{2., 5., 4.};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(sheared_point[row], expected[row], abs_error)
            << "Got:\t\t" << sheared_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, ShearingPointComponentYAgainstZ) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{2., 3., 4.};

    CHERRY_BLAZER_CONSTEXPR ShearDirection direction{Y::AgainstZ{}};
    CHERRY_BLAZER_CONSTEXPR auto shearing_matrix = Matrix<T, D, D>::shearing(direction);
    CHERRY_BLAZER_CONSTEXPR auto sheared_point = shearing_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{2., 7., 4.};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(sheared_point[row], expected[row], abs_error)
            << "Got:\t\t" << sheared_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, ShearingPointComponentZAgainstX) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{2., 3., 4.};

    CHERRY_BLAZER_CONSTEXPR ShearDirection direction{Z::AgainstX{}};
    CHERRY_BLAZER_CONSTEXPR auto shearing_matrix = Matrix<T, D, D>::shearing(direction);
    CHERRY_BLAZER_CONSTEXPR auto sheared_point = shearing_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{2., 3., 6.};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(sheared_point[row], expected[row], abs_error)
            << "Got:\t\t" << sheared_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, ShearingPointComponentZAgainstY) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point original_point{2., 3., 4.};

    CHERRY_BLAZER_CONSTEXPR ShearDirection direction{Z::AgainstY{}};
    CHERRY_BLAZER_CONSTEXPR auto shearing_matrix = Matrix<T, D, D>::shearing(direction);
    CHERRY_BLAZER_CONSTEXPR auto sheared_point = shearing_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{2., 3., 7.};
    for (auto row{0U}; row < D; ++row) {
        EXPECT_NEAR(sheared_point[row], expected[row], abs_error)
            << "Got:\t\t" << sheared_point << "\nbut expected:\t" << expected;
    }
}

TEST_F(TranslationMatrixTest, TransformationsAppliedTogether) { // NOLINT
    using namespace std::numbers;
    CHERRY_BLAZER_CONSTEXPR Point original_point{1., 0., 1.};

    CHERRY_BLAZER_CONSTEXPR auto rotation_matrix = Matrix<T, D, D>::rotation(Axis::X, pi_v<T> / 2);
    CHERRY_BLAZER_CONSTEXPR auto scaling_matrix = Matrix<T, D, D>::scaling({5., 5., 5.});
    CHERRY_BLAZER_CONSTEXPR auto translation_matrix = Matrix<T, D, D>::translation({10., 5., 7.});
    CHERRY_BLAZER_CONSTEXPR auto shearing_matrix = Matrix<T, D, D>::shearing(Z::AgainstX{});

    CHERRY_BLAZER_CONSTEXPR auto transformed_point =
        shearing_matrix * translation_matrix * scaling_matrix * rotation_matrix * original_point;

    CHERRY_BLAZER_CONSTEXPR Point expected{15., 0., 22.};
    for (auto row{0U}; row < D - 1; ++row) {
        EXPECT_NEAR(transformed_point[row], expected[row], abs_error)
            << "Got:\t\t" << transformed_point << "\nbut expected:\t" << expected;
    }
}

#include <cherry_blazer/axis.hh>
#include <cherry_blazer/coord.hh>
#include <cherry_blazer/matrix_operations.hh>
#include <cherry_blazer/normal.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/sphere.hh>
#include <cherry_blazer/square_matrix.hh>
#include <cherry_blazer/transformation.hh>
#include <cherry_blazer/vector.hh>

#include <gtest/gtest.h>

#include <numbers>

namespace {
inline constexpr double abs_error = 1e-5;
}

using cherry_blazer::Axis;
using cherry_blazer::Coord;
using cherry_blazer::Mat4d;
using cherry_blazer::Point;
using cherry_blazer::Sphere;
using cherry_blazer::Transformation;
using cherry_blazer::Vector;

TEST(NormalTest, NormalAtSpherePointOnXAxis) {
    Sphere sphere;
    Point point{1., 0., 0.};
    Vector expected{1., 0., 0.};

    auto normal_vector = normal(sphere, point);

    EXPECT_EQ(normal_vector, expected);
}

TEST(NormalTest, NormalAtSpherePointOnYAxis) {
    Sphere sphere;
    Point point{0., 1., 0.};
    Vector expected{0., 1., 0.};

    auto normal_vector = normal(sphere, point);

    EXPECT_EQ(normal_vector, expected);
}

TEST(NormalTest, NormalAtSpherePointOnZAxis) {
    Sphere sphere;
    Point point{0., 0., 1.};
    Vector expected{0., 0., 1.};

    auto normal_vector = normal(sphere, point);

    EXPECT_EQ(normal_vector, expected);
}

TEST(NormalTest, NormalAtNonaxialSpherePoint) {
    using namespace std::numbers;

    Sphere sphere;
    constexpr auto coord = sqrt3_v<double> / 3.;
    Point point{coord, coord, coord};
    Vector expected{coord, coord, coord};

    auto normal_vector = normal(sphere, point);

    EXPECT_EQ(normal_vector, expected);
}

TEST(NormalTest, NormalIsNormalized) {
    using namespace std::numbers;

    Sphere sphere;
    constexpr auto coord = sqrt3_v<double> / 3.;
    Point point{coord, coord, coord};

    auto normal_vector = normal(sphere, point);
    Vector expected = normalize(normal_vector);

    EXPECT_EQ(normal_vector, expected);
}

TEST(NormalTest, NormalOnTranslatedSphere) {
    Sphere sphere{
        Transformation{Mat4d::translation(Vector{0., 1., 0.}), Transformation::Kind::Translation}};
    Point point{0., 1.70711, -0.70711};
    Vector expected{0., 0.70711, -0.70711};

    auto normal_vector = normal(sphere, point);

    // TODO: 1e-5 imprecision error, perhaps try implementing a better inverse(); Lengyel p. 47

    //    EXPECT_DOUBLE_EQ(normal_vector.get<Coord::X>(), expected.get<Coord::X>());
    //    EXPECT_DOUBLE_EQ(normal_vector.get<Coord::Y>(), expected.get<Coord::Y>());
    //    EXPECT_DOUBLE_EQ(normal_vector.get<Coord::Z>(), expected.get<Coord::Z>());

    EXPECT_NEAR(normal_vector.get<Coord::X>(), expected.get<Coord::X>(), abs_error);
    EXPECT_NEAR(normal_vector.get<Coord::Y>(), expected.get<Coord::Y>(), abs_error);
    EXPECT_NEAR(normal_vector.get<Coord::Z>(), expected.get<Coord::Z>(), abs_error);

    EXPECT_DOUBLE_EQ(normal_vector[Coord::W], expected[Coord::W]);
}

TEST(NormalTest, NormalOnTransformedSphere) {
    using namespace std::numbers;

    Sphere sphere{Transformation{Mat4d::scaling(Vector{1., 0.5, 1.}) *
                                     Mat4d::rotation(Axis::Z, pi_v<double> / 5.),
                                 Transformation::Kind::Scaling}};
    Point point{0., sqrt2_v<double> / 2., -sqrt2_v<double> / 2.};
    Vector expected{0., 0.97014, -0.24254};

    auto normal_vector = normal(sphere, point);

    // TODO: 1e-5 imprecision error, perhaps try implementing a better inverse(); Lengyel p. 47

    //    EXPECT_DOUBLE_EQ(normal_vector.get<Coord::X>(), expected.get<Coord::X>());
    //    EXPECT_DOUBLE_EQ(normal_vector.get<Coord::Y>(), expected.get<Coord::Y>());
    //    EXPECT_DOUBLE_EQ(normal_vector.get<Coord::Z>(), expected.get<Coord::Z>());

    EXPECT_NEAR(normal_vector.get<Coord::X>(), expected.get<Coord::X>(), abs_error);
    EXPECT_NEAR(normal_vector.get<Coord::Y>(), expected.get<Coord::Y>(), abs_error);
    EXPECT_NEAR(normal_vector.get<Coord::Z>(), expected.get<Coord::Z>(), abs_error);

    EXPECT_DOUBLE_EQ(normal_vector[Coord::W], expected[Coord::W]);
}

#include <cherry_blazer/coord.hh>
#include <cherry_blazer/reflect.hh>
#include <cherry_blazer/vector.hh>

#include <gtest/gtest.h>

#include <numbers>

using cherry_blazer::Coord;
using cherry_blazer::Vector;

using namespace std::numbers;

TEST(ReflectTest, ReflectVectorApproachingAt45Deg) {
    Vector in{1., -1., 0.};
    Vector normal{0., 1., 0.};
    Vector expected{1., 1., 0.};

    auto result = reflect(in, normal);

    EXPECT_DOUBLE_EQ(result[Coord::X], expected[Coord::X]);
    EXPECT_DOUBLE_EQ(result[Coord::Y], expected[Coord::Y]);
    EXPECT_DOUBLE_EQ(result[Coord::Z], expected[Coord::Z]);
    EXPECT_DOUBLE_EQ(result[Coord::W], expected[Coord::W]);
}

TEST(ReflectTest, ReflectVectorOffSurfaceSlantedAt45Deg) {
    Vector in{0., -1., 0.};
    Vector normal{sqrt2_v<double> / 2., sqrt2_v<double> / 2., 0.};
    Vector expected{1., 0., 0.};

    auto result = reflect(in, normal);

    EXPECT_DOUBLE_EQ(result[Coord::X], expected[Coord::X]);
    EXPECT_NEAR(result[Coord::Y], expected[Coord::Y], 2.23e-16);
    EXPECT_DOUBLE_EQ(result[Coord::Z], expected[Coord::Z]);
    EXPECT_DOUBLE_EQ(result[Coord::W], expected[Coord::W]);
}

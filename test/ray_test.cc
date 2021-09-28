#include <cherry_blazer/ray.hh>

#include <gtest/gtest.h>

using cherry_blazer::Point;
using cherry_blazer::Point3d;
using cherry_blazer::Ray;

namespace {
#if __cpp_deduction_guides >= 201907
using Vec = cherry_blazer::Vec3d; // NOLINT(readability-identifier-naming)
#else
using Vec = cherry_blazer::Matrix<double, 3, 1>; // NOLINT(readability-identifier-naming)
#endif
} // namespace

TEST(RayTest, RayIsConstructible) { // NOLINT
    Ray ray{Point{1., 2., 3.}, Vec{4., 5., 6.}};
}

TEST(RayTest, RayPosition) { // NOLINT
    Ray ray{Point{2., 3., 4.}, Vec{1., 0., 0.}};

    auto const position1 = ray.position(0.);
    auto const position2 = ray.position(1.);
    auto const position3 = ray.position(-1.);
    auto const position4 = ray.position(2.5);

    EXPECT_EQ(position1, (Point3d{2., 3., 4.}));
    EXPECT_EQ(position2, (Point3d{3., 3., 4.}));
    EXPECT_EQ(position3, (Point3d{1., 3., 4.}));
    EXPECT_EQ(position4, (Point3d{4.5, 3., 4.}));
}

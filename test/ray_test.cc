#include <cherry_blazer/ray.hh>

#include <gtest/gtest.h>

using cherry_blazer::Point;
using cherry_blazer::Ray;

#if __cpp_deduction_guides >= 201907
using Vec = cherry_blazer::Vec3d; // NOLINT(readability-identifier-naming)
#else
using Vec = cherry_blazer::Matrix<double, 3, 1>; // NOLINT(readability-identifier-naming)
#endif

TEST(RayTest, RayIsConstructible) { // NOLINT
    Point origin{2., 3., 4.};
    Vec direction{0., 1., 0.};
    Ray ray{origin, direction};
    EXPECT_EQ(origin, ray.origin);
    EXPECT_EQ(direction, ray.direction);
}

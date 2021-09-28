#include <ak_toolkit/markable.hpp>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/ray.hh>
#include <cherry_blazer/sphere.hh>
#include <cherry_blazer/vector.hh>

#include <gtest/gtest.h>

#include <iostream>

using cherry_blazer::Point;
using cherry_blazer::Ray;
using cherry_blazer::Sphere;

namespace {
#if __cpp_deduction_guides >= 201907
using Vec = cherry_blazer::Vec3d; // NOLINT(readability-identifier-naming)
#else
using Vec = cherry_blazer::Matrix<double, 3, 1>; // NOLINT(readability-identifier-naming)
#endif
} // namespace

TEST(SphereTest, SphereIsConstructible) { // NOLINT
    [[maybe_unused]] Sphere sphere;
}

TEST(SphereTest, SphereId) { // NOLINT
    Sphere s;
    auto const result = s.id();
    EXPECT_EQ(result, 2); // don't move this test around!
}

TEST(SphereTest, SphereIdEqualsNotEquals) { // NOLINT
    Sphere s1;
    Sphere s2;
    Sphere s3{s1};

    EXPECT_NE(s1, s2);
    EXPECT_EQ(s1, s3);
    EXPECT_NE(s2, s3);
}

TEST(SphereTest, RayIntersectsSphereAtTwoPoints) { // NOLINT
    Ray ray{Point{0., 0., -5.}, Vec{0., 0., 1.}};
    auto const result = Sphere::intersect(ray);
    ASSERT_TRUE(result.first.has_value());
    ASSERT_TRUE(result.second.has_value());
    EXPECT_EQ(result.first.value(), 4.);
    EXPECT_EQ(result.second.value(), 6.);
}

TEST(SphereTest, RayIntersectsSphereAtOnePoint) { // NOLINT
    Ray ray{Point{0., 1., -5.}, Vec{0., 0., 1.}};
    auto const result = Sphere::intersect(ray);
    ASSERT_TRUE(result.first.has_value());
    ASSERT_FALSE(result.second.has_value());
    EXPECT_EQ(result.first.value(), 5.);
}

TEST(SphereTest, RayDoesntIntersectSphere) { // NOLINT
    Ray ray{Point{0., 2., -5.}, Vec{0., 0., 1.}};
    auto const result = Sphere::intersect(ray);
    EXPECT_FALSE(result.first.has_value());
    EXPECT_FALSE(result.second.has_value());
}

TEST(SphereTest, RayOriginatesInsideSphereAndIntersectsAtTwoPoints) { // NOLINT
    Ray ray{Point{0., 0., 0.}, Vec{0., 0., 1.}};
    auto const result = Sphere::intersect(ray);
    ASSERT_TRUE(result.first.has_value());
    ASSERT_TRUE(result.second.has_value());
    EXPECT_EQ(result.first.value(), -1.);
    EXPECT_EQ(result.second.value(), 1.);
}

TEST(SphereTest, RayOriginatesBehindSphereAndIntersectsAtTwoPoints) { // NOLINT
    Ray ray{Point{0., 0., 5.}, Vec{0., 0., 1.}};
    auto const result = Sphere::intersect(ray);
    ASSERT_TRUE(result.first.has_value());
    ASSERT_TRUE(result.second.has_value());
    EXPECT_EQ(result.first.value(), -6.);
    EXPECT_EQ(result.second.value(), -4.);
}

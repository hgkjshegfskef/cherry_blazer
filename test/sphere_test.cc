#include <cherry_blazer/intersection.hh>
#include <cherry_blazer/ray.hh>
#include <cherry_blazer/sphere.hh>

#include <gtest/gtest.h>

using cherry_blazer::Matrix;
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
    [[maybe_unused]] auto const result = s.id();
    // Don't test id in any way, because it depends on the runtime.
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

    auto const intersections = intersect(Sphere{}, ray);

    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0].t, 4.);
    EXPECT_EQ(intersections[1].t, 6.);
}

TEST(SphereTest, RayIntersectsSphereAtOnePoint) { // NOLINT
    Ray ray{Point{0., 1., -5.}, Vec{0., 0., 1.}};

    auto const intersections = intersect(Sphere{}, ray);

    EXPECT_EQ(intersections.size(), 1);
    EXPECT_EQ(intersections[0].t, 5.);
}

TEST(SphereTest, RayDoesntIntersectSphere) { // NOLINT
    Ray ray{Point{0., 2., -5.}, Vec{0., 0., 1.}};

    auto const intersections = intersect(Sphere{}, ray);

    EXPECT_EQ(intersections.size(), 0);
}

TEST(SphereTest, RayOriginatesInsideSphereAndIntersectsAtTwoPoints) { // NOLINT
    Ray ray{Point{0., 0., 0.}, Vec{0., 0., 1.}};

    auto const intersections = intersect(Sphere{}, ray);

    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0].t, -1.);
    EXPECT_EQ(intersections[1].t, 1.);
}

TEST(SphereTest, RayOriginatesBehindSphereAndIntersectsAtTwoPoints) { // NOLINT
    Ray ray{Point{0., 0., 5.}, Vec{0., 0., 1.}};

    auto const intersections = intersect(Sphere{}, ray);

    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0].t, -6.);
    EXPECT_EQ(intersections[1].t, -4.);
}

TEST(SphereTest, SphereDefaultTransformation) { // NOLINT
    Sphere sphere;

    EXPECT_EQ(sphere.transformation, (Matrix<double, 4, 4>::identity()));
}

TEST(SphereTest, SphereSetTransformation) { // NOLINT
    Sphere sphere{Matrix<double, 4, 4>::translation(Vec{2., 3., 4.})};

    EXPECT_EQ(sphere.transformation, (Matrix<double, 4, 4>::translation(Vec{2., 3., 4.})));
}

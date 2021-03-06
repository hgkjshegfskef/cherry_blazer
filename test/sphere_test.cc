#include <cherry_blazer/intersection.hh>
#include <cherry_blazer/matrix_operations.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/point_operations.hh>
#include <cherry_blazer/ray.hh>
#include <cherry_blazer/sphere.hh>
#include <cherry_blazer/vector.hh>

#include <gtest/gtest.h>

#include <memory>

using cherry_blazer::Mat4d;
using cherry_blazer::Material;
using cherry_blazer::Matrix;
using cherry_blazer::Point;
using cherry_blazer::Ray;
using cherry_blazer::Sphere;
using cherry_blazer::Transformation;
using cherry_blazer::Vec3d;
using cherry_blazer::Vector;

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
    Ray ray{Point{0., 0., -5.}, Vector{0., 0., 1.}};

    auto const intersections = intersect(Sphere{}, ray);

    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0].t, 4.);
    EXPECT_EQ(intersections[1].t, 6.);
}

TEST(SphereTest, RayIntersectsSphereAtOnePoint) { // NOLINT
    Ray ray{Point{0., 1., -5.}, Vector{0., 0., 1.}};

    auto const intersections = intersect(Sphere{}, ray);

    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0].t, 5.);
    EXPECT_EQ(intersections[1].t, 5.);
}

TEST(SphereTest, RayDoesntIntersectSphere) { // NOLINT
    Ray ray{Point{0., 2., -5.}, Vector{0., 0., 1.}};

    auto const intersections = intersect(Sphere{}, ray);

    EXPECT_EQ(intersections.size(), 0);
}

TEST(SphereTest, RayOriginatesInsideSphereAndIntersectsAtTwoPoints) { // NOLINT
    Ray ray{Point{0., 0., 0.}, Vector{0., 0., 1.}};

    auto const intersections = intersect(Sphere{}, ray);

    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0].t, -1.);
    EXPECT_EQ(intersections[1].t, 1.);
}

TEST(SphereTest, RayOriginatesBehindSphereAndIntersectsAtTwoPoints) { // NOLINT
    Ray ray{Point{0., 0., 5.}, Vector{0., 0., 1.}};

    auto const intersections = intersect(Sphere{}, ray);

    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0].t, -6.);
    EXPECT_EQ(intersections[1].t, -4.);
}

TEST(SphereTest, SphereDefaultTransformation) { // NOLINT
    Sphere sphere;

    EXPECT_EQ(sphere.transformation.mat, Mat4d::identity());
    EXPECT_EQ(sphere.transformation.kind, Transformation::Kind::Identity);
}

TEST(SphereTest, SphereSetTransformation) { // NOLINT
    Sphere sphere{{Mat4d::translation(Vector{2., 3., 4.}), Transformation::Kind::Translation}};

    EXPECT_EQ(sphere.transformation.mat, (Mat4d::translation(Vec3d{2., 3., 4.})));
    EXPECT_EQ(sphere.transformation.kind, Transformation::Kind::Translation);
}

TEST(SphereTest, RayIntersectsScaledSphere) { // NOLINT
    Ray ray{Point{0., 0., -5.}, Vector{0., 0., 1.}};
    Sphere sphere{{Mat4d::scaling(Vector{2., 2., 2.}), Transformation::Kind::Scaling}};

    auto const intersections = intersect(sphere, ray);

    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0].t, 3.);
    EXPECT_EQ(intersections[1].t, 7.);
}

TEST(SphereTest, RayIntersectsTranslatedSphere) { // NOLINT
    Ray ray{Point{0., 0., -5.}, Vector{0., 0., 1.}};
    Sphere sphere{{Mat4d::translation(Vector{5., 0., 0.}), Transformation::Kind::Translation}};

    auto const intersections = intersect(sphere, ray);

    EXPECT_EQ(intersections.size(), 0);
}

TEST(SphereTest, SphereHasDefaultMaterial) {
    Sphere sphere;
    Material material;

    EXPECT_EQ(sphere.material, material);
}

TEST(SphereTest, SphereCanBeAssignedMaterial) {
    Sphere sphere;
    Material material{.ambient = 1.};

    sphere.material = material;

    EXPECT_EQ(sphere.material, material);
}

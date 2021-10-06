#include <cherry_blazer/intersection.hh>
#include <cherry_blazer/matrix_operations.hh>
#include <cherry_blazer/ray.hh>
#include <cherry_blazer/sphere.hh>
#include <cherry_blazer/point_operations.hh>

#include <gtest/gtest.h>

#include <memory>

using cherry_blazer::Mat4d;
using cherry_blazer::Matrix;
using cherry_blazer::Point;
using cherry_blazer::Ray;
using cherry_blazer::Sphere;
using cherry_blazer::Transformation;

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

    EXPECT_EQ(sphere.transformation.mat, nullptr);
    EXPECT_EQ(sphere.transformation.kind, Transformation::Kind::Identity);
}

TEST(SphereTest, SphereSetTransformation) { // NOLINT
    auto const t = Transformation{std::make_shared<Mat4d>(Mat4d::translation(Vec{2., 3., 4.})),
                                  Transformation::Kind::Translation};
    Sphere sphere{t};

    EXPECT_EQ(*sphere.transformation.mat, (Matrix<double, 4, 4>::translation(Vec{2., 3., 4.})));
    EXPECT_EQ(sphere.transformation.kind, Transformation::Kind::Translation);
}

TEST(SphereTest, RayIntersectsScaledSphere) { // NOLINT
    Ray ray{Point{0., 0., -5.}, Vec{0., 0., 1.}};
    auto const t = Transformation{std::make_shared<Mat4d>(inverse(Mat4d::scaling(Vec{2., 2., 2.}))),
                                  Transformation::Kind::Scaling};
    Sphere sphere{t};
    // inverted scaled point:  0 0 -2.5
    // inverted scaled vector: 0 0  0.5

    //    std::cout << "inversed scaling matrix:\n" << *t.mat << std::endl;

    auto const intersections = intersect(sphere, ray);

    //    std::cout << "intersections[0].sphere.mat:\n"
    //              << *intersections[0].object->transformation.mat << std::endl;
    //    std::cout << "intersections[1].sphere.mat:\n"
    //              << *intersections[1].object->transformation.mat << std::endl;

    EXPECT_EQ(intersections.size(), 2);
    EXPECT_EQ(intersections[0].t, 3.);
    EXPECT_EQ(intersections[1].t, 7.);
}

TEST(SphereTest, RayIntersectsTranslatedSphere) { // NOLINT
    Ray ray{Point{0., 0., -5.}, Vec{0., 0., 1.}};
    auto const t =
        Transformation{std::make_shared<Mat4d>(inverse(Mat4d::translation(Vec{5., 0., 0.}))),
                       Transformation::Kind::Translation};
    Sphere sphere{t};

    auto const intersections = intersect(sphere, ray);

    EXPECT_EQ(intersections.size(), 0);
}

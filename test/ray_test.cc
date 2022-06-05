#include <cherry_blazer/matrix_operations.hh>
#include <cherry_blazer/ray.hh>
#include <cherry_blazer/square_matrix.hh>
#include <cherry_blazer/transformation.hh>

#include <gtest/gtest.h>

using cherry_blazer::Mat4d;
using cherry_blazer::Matrix;
using cherry_blazer::Point;
using cherry_blazer::Point3d;
using cherry_blazer::Ray;
using cherry_blazer::Transformation;
using cherry_blazer::Vec3d;
using cherry_blazer::Vector;

TEST(RayTest, RayIsConstructible) { // NOLINT
    [[maybe_unused]] Ray const ray{Point{1., 2., 3.}, Vector{4., 5., 6.}};
}

TEST(RayTest, RayPosition) { // NOLINT
    Ray const ray{Point{2., 3., 4.}, Vector{1., 0., 0.}};

    auto const position1 = ray.position(0.);
    auto const position2 = ray.position(1.);
    auto const position3 = ray.position(-1.);
    auto const position4 = ray.position(2.5);

    EXPECT_EQ(position1, (Point3d{2., 3., 4.}));
    EXPECT_EQ(position2, (Point3d{3., 3., 4.}));
    EXPECT_EQ(position3, (Point3d{1., 3., 4.}));
    EXPECT_EQ(position4, (Point3d{4.5, 3., 4.}));
}

TEST(RayTest, RayTranslation) { // NOLINT
    Ray const ray{Point{1., 2., 3.}, Vector{0., 1., 0.}};
    Transformation const t{Mat4d::translation(Vector{3., 4., 5.}),
                           Transformation::Kind::Translation};

    auto const transformed_ray = transform(ray, t);

    EXPECT_EQ(transformed_ray.origin, (Point3d{4., 6., 8.}));
    EXPECT_EQ(transformed_ray.direction, (Vec3d{0., 1., 0.}));
}

TEST(RayTest, RayScaling) { // NOLINT
    Ray const ray{Point{1., 2., 3.}, Vector{0., 1., 0.}};
    Transformation const t{Mat4d::scaling(Vector{2., 3., 4.}), Transformation::Kind::Scaling};

    auto const transformed_ray = transform(ray, t);

    EXPECT_EQ(transformed_ray.origin, (Point3d{2., 6., 12.}));
    EXPECT_EQ(transformed_ray.direction, (Vec3d{0., 3., 0.}));
}

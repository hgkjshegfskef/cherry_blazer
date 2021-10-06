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

namespace {
#if __cpp_deduction_guides >= 201907
using Vec = cherry_blazer::Vec3d; // NOLINT(readability-identifier-naming)
#else
using Vec = cherry_blazer::Matrix<double, 3, 1>; // NOLINT(readability-identifier-naming)
#endif
} // namespace

TEST(RayTest, RayIsConstructible) { // NOLINT
    [[maybe_unused]] Ray const ray{Point{1., 2., 3.}, Vec{4., 5., 6.}};
}

TEST(RayTest, RayPosition) { // NOLINT
    Ray const ray{Point{2., 3., 4.}, Vec{1., 0., 0.}};

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
    Ray const ray{Point{1., 2., 3.}, Vec{0., 1., 0.}};
    auto const t = Transformation{std::make_shared<Mat4d>(Mat4d::translation(Vec{3., 4., 5.})),
                                  Transformation::Kind::Translation};

    auto const transformed_ray = transform(ray, t);

    EXPECT_EQ(transformed_ray.origin, (Point3d{4., 6., 8.}));
    EXPECT_EQ(transformed_ray.direction, (Vec3d{0., 1., 0.}));
}

TEST(RayTest, DISABLED_RayScaling) { // NOLINT
    Ray const ray{Point{1., 2., 3.}, Vec{0., 1., 0.}};
    auto const t = Transformation{std::make_shared<Mat4d>(Mat4d::scaling(Vec{2., 3., 4.})),
                                  Transformation::Kind::Scaling};

    auto const transformed_ray = transform(ray, t);

    EXPECT_EQ(transformed_ray.origin, (Point3d{2., 6., 12.}));
    EXPECT_EQ(transformed_ray.direction, (Vec3d{0., 3., 0.}));
}

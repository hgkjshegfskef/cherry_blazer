#include <cherry_blazer/lighting.hh>

#include <gtest/gtest.h>

#include <numbers>

using cherry_blazer::Color;
using cherry_blazer::Material;
using cherry_blazer::Point;
using cherry_blazer::Point3d;
using cherry_blazer::PointLight;
using cherry_blazer::Vector;

using namespace std::numbers;

class LightingTest : public ::testing::Test {
  protected:
    Material material;
    Point3d position{0., 0., 0.};
};

TEST_F(LightingTest, LightingWithEyeBetweenLightAndSurface) {
    Vector eye_vector{0., 0., -1.};
    Vector normal_vector{0., 0., -1.};
    PointLight light{Point{0., 0., -10.}, Color{1., 1., 1.}};
    Color expected{1.9, 1.9, 1.9};

    auto result = lighting(material, light, position, eye_vector, normal_vector);

    EXPECT_EQ(expected, result);
}

TEST_F(LightingTest, LightingWithEyeBetweenLightAndSurfaceAndEyeOffset45Deg) {
    Vector eye_vector{0., sqrt2_v<double> / 2., -sqrt2_v<double> / 2.};
    Vector normal_vector{0., 0., -1.};
    PointLight light{Point{0., 0., -10.}, Color{1., 1., 1.}};
    Color expected{1., 1., 1.};

    auto result = lighting(material, light, position, eye_vector, normal_vector);

    EXPECT_EQ(expected, result);
}

TEST_F(LightingTest, LightingWithEyeOppositeSurfaceAndEyeOffset45Deg) {
    Vector eye_vector{0., 0., -1.};
    Vector normal_vector{0., 0., -1.};
    PointLight light{Point{0., 10., -10.}, Color{1., 1., 1.}};
    Color expected{.7364, .7364, .7364};

    auto result = lighting(material, light, position, eye_vector, normal_vector);

    EXPECT_NEAR(result.r, expected.r, 1e-5);
    EXPECT_NEAR(result.g, expected.g, 1e-5);
    EXPECT_NEAR(result.b, expected.b, 1e-5);
}

TEST_F(LightingTest, LightingWithEyeInPathOfReflectionVector) {
    Vector eye_vector{0., -sqrt2_v<double> / 2., -sqrt2_v<double> / 2.};
    Vector normal_vector{0., 0., -1.};
    PointLight light{Point{0., 10., -10.}, Color{1., 1., 1.}};
    Color expected{1.6364, 1.6364, 1.6364};

    auto result = lighting(material, light, position, eye_vector, normal_vector);

    EXPECT_NEAR(result.r, expected.r, 1e-5);
    EXPECT_NEAR(result.g, expected.g, 1e-5);
    EXPECT_NEAR(result.b, expected.b, 1e-5);
}

TEST_F(LightingTest, LightingWithLightBehindSurface) {
    Vector eye_vector{0., 0., -1.};
    Vector normal_vector{0., 0., -1.};
    PointLight light{Point{0., 0., 10.}, Color{1., 1., 1.}};
    Color expected{.1, .1, .1};

    auto result = lighting(material, light, position, eye_vector, normal_vector);

    EXPECT_EQ(expected, result);
}

#include <cherry_blazer/point_light.hh>

#include <gtest/gtest.h>

using cherry_blazer::Color;
using cherry_blazer::Point;
using cherry_blazer::PointLight;

TEST(PointLightTest, PointLightCtor) {
    Color intensity{1., 1., 1.};
    Point position{0., 0., 0.};
    PointLight point_light{position, intensity};

    EXPECT_EQ(point_light.position, position);
    EXPECT_EQ(point_light.intensity, intensity);
}

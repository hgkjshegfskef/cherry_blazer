#include <cherry_blazer/material.hh>

#include <gtest/gtest.h>

using cherry_blazer::Color;
using cherry_blazer::Material;

TEST(MaterialTest, DefaultMaterial) {
    Material material;

    EXPECT_EQ(material.color, (Color{1., 1., 1.}));
    EXPECT_DOUBLE_EQ(material.ambient, .1);
    EXPECT_DOUBLE_EQ(material.diffuse, .9);
    EXPECT_DOUBLE_EQ(material.specular, .9);
    EXPECT_DOUBLE_EQ(material.shininess, 200.);
}

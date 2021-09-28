#include <cherry_blazer/sphere.hh>

#include <gtest/gtest.h>

using cherry_blazer::Sphere;

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

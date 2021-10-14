#include <cherry_blazer/intersection.hh>
#include <cherry_blazer/sphere.hh>

#include <gtest/gtest.h>

using cherry_blazer::Intersection;
using cherry_blazer::Sphere;

TEST(IntersectionTest, IntersectionIsConstructible) { // NOLINT
    [[maybe_unused]] Intersection intersection{5., Sphere{}};
}

TEST(IntersectionTest, HitTestAllIntersectionsHavePositivePlace) { // NOLINT
    Sphere sphere1;
    Sphere sphere2;
    std::vector<Intersection> intersections{{1., sphere1}, {2., sphere2}};

    auto first_hit = hit(intersections);

    EXPECT_EQ(first_hit->t, 1.);
    EXPECT_EQ(first_hit->object, sphere1);
}

TEST(IntersectionTest, HitTestSomeIntersectionsHaveNegativePlace) { // NOLINT
    Sphere sphere1;
    Sphere sphere2;
    std::vector<Intersection> intersections{{-1., sphere1}, {1., sphere2}};

    auto const first_hit = hit(intersections);

    EXPECT_EQ(first_hit->t, 1.);
    EXPECT_EQ(first_hit->object, sphere2);
}

TEST(IntersectionTest, HitTestAllIntersectionsHaveNegativePlace) { // NOLINT
    Sphere sphere1;
    Sphere sphere2;
    std::vector<Intersection> intersections{{-2., sphere1}, {-1., sphere2}};

    auto const first_hit = hit(intersections);

    EXPECT_EQ(first_hit, nullptr);
}

TEST(IntersectionTest, HitTestAlwaysLowestNonNegativePlace) { // NOLINT
    Sphere sphere1;
    Sphere sphere2;
    Sphere sphere3;
    Sphere sphere4;
    std::vector<Intersection> intersections{
        {5., sphere1}, {7., sphere2}, {-3., sphere3}, {2., sphere4}};

    auto const first_hit = hit(intersections);

    EXPECT_EQ(first_hit->t, 2.);
    EXPECT_EQ(first_hit->object, sphere4);
}

#include <cherry_blazer/intersection.hh>
#include <cherry_blazer/sphere.hh>

#include <gtest/gtest.h>

using cherry_blazer::Intersection;
using cherry_blazer::Sphere;

TEST(IntersectionTest, IntersectionIsConstructible) { // NOLINT
    [[maybe_unused]] Intersection intersection{5., std::make_shared<Sphere>(Sphere{})};
}

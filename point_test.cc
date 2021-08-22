#pragma clang diagnostic push
// Ignore googletest-related warnings:
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-non-const-global-variables"
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#pragma ide diagnostic ignored "cert-err58-cpp"
// It's ok to use magic numbers in tests:
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

#include <gtest/gtest.h>

#include "point.hh"
#include "vector.hh"

// point += vector (= point)
TEST(PointTest, PointPlusEqualsVector) {
    Point p{3, -2, 5};
    Vector v{-2, 3, 1};
    p += v;
    EXPECT_EQ(p, Point(1, 1, 6));
}

// point + vector = point
TEST(PointTest, PointPlusVector) {
    Point p{3, -2, 5};
    Vector v{-2, 3, 1};
    auto p2 = p + v;
    EXPECT_EQ(p2, Point(1, 1, 6));
}

#pragma clang diagnostic pop

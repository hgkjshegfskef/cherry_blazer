#pragma clang diagnostic push
// Ignore googletest-related warnings:
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-non-const-global-variables"
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#pragma ide diagnostic ignored "cert-err58-cpp"
// It's ok to use magic numbers in tests:
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

#include <gtest/gtest.h>

#include "point.hh"
#include "tuple.hh"
#include "vector.hh"

// tuple += tuple (= tuple)
TEST(TupleTest, TuplePlusEqualsTuple) {
    Tuple t1{3, -2, 5};
    Tuple t2{-2, 3, 1};
    t1 += t2;
    EXPECT_EQ(t1, Tuple(1, 1, 6));
}

// tuple + tuple = tuple
TEST(TupleTest, TuplePlusTuple) {
    Tuple t1{3, -2, 5};
    Tuple t2{-2, 3, 1};
    auto t3 = t1 + t2;
    EXPECT_EQ(t3, Tuple(1, 1, 6));
}

// point += vector (= point)
TEST(TupleTest, PointPlusEqualsVector) {
    Point p{3, -2, 5};
    Vector v{-2, 3, 1};
    p += v;
    EXPECT_EQ(p, Point(1, 1, 6));
}

// point + vector = point
TEST(TupleTest, PointPlusVector) {
    Point p{3, -2, 5};
    Vector v{-2, 3, 1};
    auto p2 = p + v;
    EXPECT_EQ(p2, Point(1, 1, 6));
}

// vector += vector (= vector)
TEST(TupleTest, VectorPlusEqualsVector) {
    Vector v1{3, -2, 5};
    Vector v2{-2, 3, 1};
    v1 += v2;
    EXPECT_EQ(v1, Vector(1, 1, 6));
}

// vector + vector = vector
TEST(TupleTest, VectorPlusVector) {
    Vector v1{3, -2, 5};
    Vector v2{-2, 3, 1};
    auto v3 = v1 + v2;
    EXPECT_EQ(v3, Vector(1, 1, 6));
}

#pragma clang diagnostic pop

// TODO: disallow comparisons of point,vector with tuple, get rid of 4th member?
// TODO: compile-time test that adding point and tuple is not allowed
// TODO: compile-time test that adding vector and tuple is not allowed
// TODO: compile-time test that adding two points is not allowed
// TODO: compile-time test that point doesn't compare to vector
// TODO: maybe explicit conversion operator to Tuple?

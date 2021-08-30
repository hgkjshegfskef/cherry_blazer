#include <gtest/gtest.h>

#include <sstream>

#include "point.hh"
#include "vector.hh"

// Everything marked as (=ERROR) is tested to not compile in point_test.cmake.

// Point is default constructable and zero-initialized.
TEST(PointTest, PointDefaultConstructable) { // NOLINT
    Point p;
    EXPECT_EQ(p, Point(0, 0, 0));
}

// -Point
TEST(PointTest, PointNegate) { // NOLINT
    Point p1{3, -2, 5};
    auto p2{-p1};
    EXPECT_EQ(p2, Point(-3, 2, -5));
}

// Point += Point (= ERROR)

// Point + Point (= ERROR)

// Point -= Point (= ERROR)

// Point - Point = Vector
TEST(PointTest, PointMinusPoint) { // NOLINT
    Point p1{3, 2, 1};
    Point p2{5, 6, 7};
    auto v = p1 - p2;
    EXPECT_EQ(v, Vector(-2, -4, -6));
}

// Point == Point
TEST(PointTest, PointComparedToPointEquals) { // NOLINT
    Point p1{3, -2, 5};
    Point p2{3, -2, 5};
    EXPECT_EQ(p1, p2);
}

// Point != Point
TEST(PointTest, PointComparedToPointDoesntEqual) { // NOLINT
    Point p1{3, -2, 5};
    Point p2{-2, 3, 1};
    EXPECT_NE(p1, p2);
}

// Point can be printed out.
TEST(PointTest, PointPrintOut) { // NOLINT
    Point p{1, 2, 3};
    std::stringstream ss;
    ss << p;
    EXPECT_EQ(ss.str(), std::string{"{1, 2, 3}"});
}

// Vector-related:

// Point += Vector
TEST(PointTest, PointPlusEqualsVector) { // NOLINT
    Point p{3, -2, 5};
    Vector v{-2, 3, 1};
    p += v;
    EXPECT_EQ(p, Point(1, 1, 6));
}

// Point + Vector = Point
TEST(PointTest, PointPlusVector) { // NOLINT
    Point p{3, -2, 5};
    Vector v{-2, 3, 1};
    auto p2 = p + v;
    EXPECT_EQ(p2, Point(1, 1, 6));
}

// Point -= Vector
TEST(PointTest, PointMinusEqualsVector) { // NOLINT
    Point p{3, 2, 1};
    Vector v{5, 6, 7};
    p -= v;
    EXPECT_EQ(p, Point(-2, -4, -6));
}

// Point - Vector = Point
TEST(PointTest, PointMinusVector) { // NOLINT
    Point p{3, 2, 1};
    Vector v{5, 6, 7};
    auto p2 = p - v;
    EXPECT_EQ(p2, Point(-2, -4, -6));
}

// Point == Vector (= ERROR)

// Point != Vector (= ERROR)

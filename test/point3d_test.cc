#include "point3d.hh"
#include "point_operations.hh"
#include "vector3d.hh"

#include <gtest/gtest.h>
#include <string>

namespace cherry_blazer {

// Everything marked as (=ERROR) is tested to not compile in point_test.cmake.

// Point is default constructable and zero-initialized.
TEST(Point3dTest, Point3dDefaultConstructable) { // NOLINT
    Point3d p{};
    EXPECT_EQ(p, (Point{0, 0, 0}));
}

// Point == Point
TEST(Point3dTest, Point3dComparedToPoint3dEquals) { // NOLINT
    Point p1{3, -2, 5};
    Point p2{3, -2, 5};
    EXPECT_EQ(p1, p2);
}

// Point != Point
TEST(Point3dTest, Point3dComparedToPoint3dDoesntEqual) { // NOLINT
    Point p1{3, -2, 5};
    Point p2{-2, 3, 1};
    EXPECT_NE(p1, p2);
}

// Point can be printed out.
TEST(Point3dTest, Point3dPrintOut) { // NOLINT
    Point p{1, 2, 3};
    std::stringstream ss;
    ss << p;
    EXPECT_EQ(ss.str(), std::string{"{1, 2, 3}"});
}

// -Point (= ERROR)

// scalar*Point (= ERROR)

// Point*scalar (= ERROR)

// Point/scalar (= ERROR)

// scalar/Point (= ERROR)

// Point += Point (= ERROR)

// Point + Point (= ERROR)

// Point -= Point (= ERROR)

// Point - Point = Vector
TEST(Point3dTest, Point3dMinusPoint3d) { // NOLINT
    Point p1{3, 2, 1};
    Point p2{5, 6, 7};
    Vector v = p1 - p2;
    EXPECT_EQ(v, (Vector{-2, -4, -6}));
}

// Vector-related:

// Point += Vector
TEST(Point3dTest, Point3dPlusEqualsVector3d) { // NOLINT
    Point p{3, -2, 5};
    Vector v{-2, 3, 1};
    p += v;
    EXPECT_EQ(p, (Point{1, 1, 6}));
}

// Point + Vector = Point
TEST(Point3dTest, Point3dPlusVector3d) { // NOLINT
    Point p{3, -2, 5};
    Vector v{-2, 3, 1};
    Point p2 = p + v;
    EXPECT_EQ(p2, (Point{1, 1, 6}));
}

// Point -= Vector
TEST(Point3dTest, Point3dMinusEqualsVector3d) { // NOLINT
    Point p{3, 2, 1};
    Vector v{5, 6, 7};
    p -= v;
    EXPECT_EQ(p, (Point{-2, -4, -6}));
}

// Point - Vector = Point
TEST(Point3dTest, Point3dMinusVector3d) { // NOLINT
    Point p{3, 2, 1};
    Vector v{5, 6, 7};
    Point p2 = p - v;
    EXPECT_EQ(p2, (Point{-2, -4, -6}));
}

// Point == Vector (= ERROR)

// Point != Vector (= ERROR)

} // namespace cherry_blazer

#include <gtest/gtest.h>

#include <sstream>

#include "point.hh"
#include "vector.hh"

// Everything marked as (=ERROR) is tested to not compile in vector_test.cmake.

// Vector is default constructable and zero-initialized.
TEST(VectorTest, VectorDefaultConstructable) { // NOLINT
    Vector v;
    EXPECT_EQ(v, Vector(0, 0, 0));
}

// Vector can be constructed from 2 Points
TEST(VectorTest, VectorCtorTwoPoints) { // NOLINT
    Point p1{3, -2, 5};
    Point p2{-2, 3, 1};
    Vector v{p1, p2};
    EXPECT_EQ(v, Vector(-5, 5, -4));
}

// -Vector
TEST(VectorTest, VectorNegate) { // NOLINT
    Vector v1{3, -2, 5};
    auto v2{-v1};
    EXPECT_EQ(v2, Vector(-3, 2, -5));
}

// Vector += Vector
TEST(VectorTest, VectorPlusEqualsVector) { // NOLINT
    Vector v1{3, -2, 5};
    Vector v2{-2, 3, 1};
    v1 += v2;
    EXPECT_EQ(v1, Vector(1, 1, 6));
}

// Vector + Vector = Vector
TEST(VectorTest, VectorPlusVector) { // NOLINT
    Vector v1{3, -2, 5};
    Vector v2{-2, 3, 1};
    auto v3 = v1 + v2;
    EXPECT_EQ(v3, Vector(1, 1, 6));
}

// Vector -= Vector
TEST(VectorTest, VectorMinusEqualsVector) { // NOLINT
    Vector v1{3, 2, 1};
    Vector v2{5, 6, 7};
    v1 -= v2;
    EXPECT_EQ(v1, Vector(-2, -4, -6));
}

// Vector - Vector = Vector
TEST(VectorTest, VectorMinusVector) { // NOLINT
    Vector v1{3, 2, 1};
    Vector v2{5, 6, 7};
    auto v3 = v1 - v2;
    EXPECT_EQ(v3, Vector(-2, -4, -6));
}

// Vector == Vector
TEST(VectorTest, VectorComparedToVectorEquals) { // NOLINT
    Vector v1{3, -2, 5};
    Vector v2{3, -2, 5};
    EXPECT_EQ(v1, v2);
}

// Vector != Vector
TEST(VectorTest, VectorComparedToVectorDoesntEqual) { // NOLINT
    Vector v1{3, -2, 5};
    Vector v2{-2, 3, 1};
    EXPECT_NE(v1, v2);
}

// Vector can be printed out.
TEST(VectorTest, VectorPrintOut) { // NOLINT
    Vector v{1, 2, 3};
    std::stringstream ss;
    ss << v;
    EXPECT_EQ(ss.str(), std::string{"{1, 2, 3}"});
}

// Point-related:

// Vector += Point (= ERROR)

// Vector + Point = Point
TEST(VectorTest, VectorPlusPoint) { // NOLINT
    Vector v{3, -2, 5};
    Point p{-2, 3, 1};
    auto p2 = v + p;
    EXPECT_EQ(p2, Point(1, 1, 6));
}

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Vector == Point (= ERROR)

// Vector != Point (= ERROR)

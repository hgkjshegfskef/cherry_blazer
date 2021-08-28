#include <gtest/gtest.h>

#include "point.hh"
#include "vector.hh"

// Vector is default constructable and zero-initialized.
TEST(VectorTest, VectorDefaultConstructable) { // NOLINT
    Vector v;
    EXPECT_EQ(v, Vector(0, 0, 0));
}

// Vector += Vector (= Vector)
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

// Vector + Point = Point
TEST(VectorTest, VectorPlusPoint) { // NOLINT
    Vector v{3, -2, 5};
    Point p{-2, 3, 1};
    auto p2 = v + p;
    EXPECT_EQ(p2, Point(1, 1, 6));
}

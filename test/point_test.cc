#include <gtest/gtest.h>

#include "point.hh"
#include "vector.hh"

// Point is default constructable and zero-initialized.
TEST(PointTest, PointDefaultConstructable) { // NOLINT
    Point p;
    EXPECT_EQ(p, Point(0, 0, 0));
}

// Point += Vector (= Point)
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

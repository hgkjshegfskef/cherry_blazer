#include "point.hh"
#include "vector.hh"

#include <cmath> // IWYU pragma: keep
#include <gtest/gtest.h>
#include <sstream> // IWYU pragma: keep
#include <string>

using cherry_blazer::Point;
using cherry_blazer::Vector;

// Everything marked as (=ERROR) is tested to not compile in vector_test.cmake.

// Vector is default constructable and zero-initialized.
TEST(VectorTest, VectorDefaultConstructable) { // NOLINT
    Vector v;
    EXPECT_EQ(v, Vector(0, 0, 0));
}

// Vector is copy constructable.
TEST(VectorTest, VectorCopyConstructable) { // NOLINT
    Vector v1;
    Vector v2{v1};
    EXPECT_EQ(v2, Vector(0, 0, 0));
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
    Vector v2{-v1};
    EXPECT_EQ(v2, Vector(-3, 2, -5));
}

// scalar*Vector
TEST(VectorTest, ScalarTimesVector) { // NOLINT
    Vector v1{1, -2, 3};
    double scalar = 3.5; // NOLINT(readability-magic-numbers)
    Vector v2 = scalar * v1;
    EXPECT_EQ(v2, Vector(3.5, -7, 10.5));
}

// Vector*scalar
TEST(VectorTest, VectorTimesScalar) { // NOLINT
    Vector v1{1, -2, 3};
    double scalar = 3.5; // NOLINT(readability-magic-numbers)
    Vector v2 = v1 * scalar;
    EXPECT_EQ(v2, Vector(3.5, -7, 10.5));
}

// Vector/scalar
TEST(VectorTest, VectorDividedByScalar) { // NOLINT
    Vector v1{1, -2, 3};
    double scalar = 2;
    Vector v2 = v1 / scalar;
    EXPECT_EQ(v2, Vector(0.5, -1, 1.5));
}

// scalar/Vector (= ERROR)

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
    Vector v3 = v1 + v2;
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
    Vector v3 = v1 - v2;
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

// Vector has magnitude.
TEST(VectorTest, VectorMagnitude) { // NOLINT
    Vector v1{1, 0, 0};
    Vector v2{0, 1, 0};
    Vector v3{0, 0, 1};
    Vector v4{1, 2, 3};
    Vector v5{-1, -2, -3};
    EXPECT_EQ(magnitude(v1), 1);
    EXPECT_EQ(magnitude(v2), 1);
    EXPECT_EQ(magnitude(v3), 1);
    EXPECT_DOUBLE_EQ(magnitude(v4), std::sqrt(14));
    EXPECT_DOUBLE_EQ(magnitude(v5), std::sqrt(14));
}

// Vector can be normalized.
TEST(VectorTest, VectorNormalize) { // NOLINT
    Vector v{4, 0, 0};
    EXPECT_EQ(normalize(v), Vector(1, 0, 0));
}

// Vectors can have a dot product.
TEST(VectorTest, VectorDotProduct) { // NOLINT
    Vector v1{1, 2, 3};
    Vector v2{2, 3, 4};
    EXPECT_EQ(dot(v1, v2), 20);
}

// Vectors can have a cross product.
TEST(VectorTest, VectorCrossProduct) { // NOLINT
    Vector v1{1, 2, 3};
    Vector v2{2, 3, 4};
    EXPECT_EQ(cross(v1, v2), Vector(-1, 2, -1));
    EXPECT_EQ(cross(v2, v1), Vector(1, -2, 1));
}

// Point-related:

// Vector += Point (= ERROR)

// Vector + Point = Point
TEST(VectorTest, VectorPlusPoint) { // NOLINT
    Vector v{3, -2, 5};
    Point p{-2, 3, 1};
    Point p2 = v + p;
    EXPECT_EQ(p2, Point(1, 1, 6));
}

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Vector == Point (= ERROR)

// Vector != Point (= ERROR)

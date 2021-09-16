#include "point3d.hh"
#include "point_properties.hh"
#include "vector3d.hh"
#include "vector_operations.hh"
#include "vector_properties.hh"

#include <gtest/gtest.h>
#include <string>

namespace cherry_blazer {

// Everything marked as (=ERROR) is tested to not compile in vector_test.cmake.

// Vector is default constructable and zero-initialized.
TEST(Vector3dTest, Vector3dDefaultConstructable) { // NOLINT
    Vector3d v{};
    EXPECT_EQ(v, (Vector3d{0, 0, 0}));
}

// Vector is copy constructable.
TEST(Vector3dTest, Vector3dCopyConstructable) { // NOLINT
    Vector3d v1{};
    Vector3d v2{v1};
    EXPECT_EQ(v2, (Vector3d{0, 0, 0}));
}

//// Vector can be constructed from 2 Points
// TEST(Vector3dTest, Vector3dCtorTwoPoints) { // NOLINT
//     Point p1{3, -2, 5};
//     Point p2{-2, 3, 1};
//     Vector3d v{p1, p2};
//     EXPECT_EQ(v, (Vector3d{-5, 5, -4}));
// }
//
// -Vector
TEST(Vector3dTest, Vector3dNegate) { // NOLINT
    Vector3d v1{3, -2, 5};
    Vector3d v2{-v1};
    EXPECT_EQ(v2, (Vector3d{-3, 2, -5}));
}

// scalar*Vector
TEST(Vector3dTest, ScalarTimesVector3d) { // NOLINT
    Vector3d v1{1, -2, 3};
    double scalar = 3.5; // NOLINT(readability-magic-numbers)
    Vector3d v2 = scalar * v1;
    EXPECT_EQ(v2, (Vector3d{3.5, -7, 10.5}));
}

// Vector*scalar
TEST(Vector3dTest, Vector3dTimesScalar) { // NOLINT
    Vector3d v1{1, -2, 3};
    double scalar = 3.5; // NOLINT(readability-magic-numbers)
    Vector3d v2 = v1 * scalar;
    EXPECT_EQ(v2, (Vector3d{3.5, -7, 10.5}));
}

// Vector/scalar
TEST(Vector3dTest, Vector3dDividedByScalar) { // NOLINT
    Vector3d v1{1, -2, 3};
    double scalar = 2;
    Vector3d v2 = v1 / scalar;
    EXPECT_EQ(v2, (Vector3d{0.5, -1, 1.5}));
}

// scalar/Vector (= ERROR)

// Vector += Vector
TEST(Vector3dTest, Vector3dPlusEqualsVector3d) { // NOLINT
    Vector3d v1{3, -2, 5};
    Vector3d v2{-2, 3, 1};
    v1 += v2;
    EXPECT_EQ(v1, (Vector3d{1, 1, 6}));
}

// Vector -= Vector
TEST(Vector3dTest, Vector3dMinusEqualsVector3d) { // NOLINT
    Vector3d v1{3, 2, 1};
    Vector3d v2{5, 6, 7};
    v1 -= v2;
    EXPECT_EQ(v1, (Vector3d{-2, -4, -6}));
}

// Vector + Vector = Vector
TEST(Vector3dTest, Vector3dPlusVector3d) { // NOLINT
    Vector3d v1{3, -2, 5};
    Vector3d v2{-2, 3, 1};
    Vector3d v3 = v1 + v2;
    EXPECT_EQ(v3, (Vector3d{1, 1, 6}));
}

// Vector - Vector = Vector
TEST(Vector3dTest, Vector3dMinusVector3d) { // NOLINT
    Vector3d v1{3, 2, 1};
    Vector3d v2{5, 6, 7};
    Vector3d v3 = v1 - v2;
    EXPECT_EQ(v3, (Vector3d{-2, -4, -6}));
}

// Vector == Vector
TEST(Vector3dTest, Vector3dComparedToVector3dEquals) { // NOLINT
    Vector3d v1{3, -2, 5};
    Vector3d v2{3, -2, 5};
    EXPECT_EQ(v1, v2);
}

// Vector != Vector
TEST(Vector3dTest, Vector3dComparedToVector3dDoesntEqual) { // NOLINT
    Vector3d v1{3, -2, 5};
    Vector3d v2{-2, 3, 1};
    EXPECT_NE(v1, v2);
}

// Vector can be printed out.
TEST(Vector3dTest, Vector3dPrintOut) { // NOLINT
    Vector3d v{1, 2, 3};
    std::stringstream ss;
    ss << v;
    EXPECT_EQ(ss.str(), std::string{"{1, 2, 3}"});
}

// Vector has magnitude.
TEST(Vector3dTest, Vector3dMagnitude) { // NOLINT
    Vector3d v1{1, 0, 0};
    Vector3d v2{0, 1, 0};
    Vector3d v3{0, 0, 1};
    Vector3d v4{1, 2, 3};
    Vector3d v5{-1, -2, -3};
    EXPECT_EQ(magnitude(v1), 1);
    EXPECT_EQ(magnitude(v2), 1);
    EXPECT_EQ(magnitude(v3), 1);
    EXPECT_DOUBLE_EQ(magnitude(v4), std::sqrt(14));
    EXPECT_DOUBLE_EQ(magnitude(v5), std::sqrt(14));
}

// Vector can be normalized.
TEST(Vector3dTest, Vector3dNormalize) { // NOLINT
    Vector3d v{4, 0, 0};
    EXPECT_EQ(normalize(v), (Vector3d{1, 0, 0}));
}

// Vectors can have a dot product.
TEST(Vector3dTest, Vector3dDotProduct) { // NOLINT
    Vector3d v1{1, 2, 3};
    Vector3d v2{2, 3, 4};
    EXPECT_EQ(dot(v1, v2), 20);
}

// Vectors can have a cross product.
TEST(Vector3dTest, Vector3dCrossProduct) { // NOLINT
    Vector3d v1{1, 2, 3};
    Vector3d v2{2, 3, 4};
    EXPECT_EQ(cross(v1, v2), (Vector3d{-1, 2, -1}));
    EXPECT_EQ(cross(v2, v1), (Vector3d{1, -2, 1}));
}

// Point-related:

// Vector += Point (= ERROR)

// Vector + Point = Point
TEST(Vector3dTest, Vector3dPlusPoint) { // NOLINT
    Vector3d v{3, -2, 5};
    Point3d p{-2, 3, 1};
    Point3d p2 = v + p;
    EXPECT_EQ(p2, (Point3d{1, 1, 6}));
}

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Vector == Point (= ERROR)

// Vector != Point (= ERROR)

} // namespace cherry_blazer

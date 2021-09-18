#include "config.hh"
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
TEST(Vec3dTest, Vec3dDefaultConstructable) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vec3d v{};
    EXPECT_EQ(v, (Vec3d{0, 0, 0}));
}

// Vector is copy constructable.
TEST(Vec3dTest, Vec3dCopyConstructable) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{v1};
    EXPECT_EQ(v2, (Vec3d{1, 2, 3}));
}

// Vector can be constructed from 2 Points
// TEST(Vec3dTest, Vec3dCtorTwoPoints) { // NOLINT
//     Point p1{3, -2, 5};
//     Point p2{-2, 3, 1};
//     Vec3d v{p1, p2};
//     EXPECT_EQ(v, (Vec3d{-5, 5, -4}));
// }

// -Vector
TEST(Vec3dTest, Vec3dNegate) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{-v1};
    EXPECT_EQ(v2, (Vec3d{-3, 2, -5}));
}

// scalar*Vector
TEST(Vec3dTest, ScalarTimesVec3d) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., -2., 3.};
    CHERRY_BLAZER_CONSTEXPR auto scalar{3.5};
    CHERRY_BLAZER_CONSTEXPR auto v2 = scalar * v1;
    EXPECT_EQ(v2, (Vec3d{3.5, -7, 10.5}));
}

// Vector*scalar
TEST(Vec3dTest, Vec3dTimesScalar) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., -2., 3.};
    CHERRY_BLAZER_CONSTEXPR auto scalar{3.5};
    CHERRY_BLAZER_CONSTEXPR auto v2 = v1 * scalar;
    EXPECT_EQ(v2, (Vec3d{3.5, -7, 10.5}));
}

// Vector/scalar
TEST(Vec3dTest, Vec3dDividedByScalar) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., -2., 3.};
    CHERRY_BLAZER_CONSTEXPR auto scalar{2.};
    CHERRY_BLAZER_CONSTEXPR auto v2 = v1 / scalar;
    EXPECT_EQ(v2, (Vec3d{0.5, -1, 1.5}));
}

// scalar/Vector (= ERROR)

// Vector += Vector
TEST(Vec3dTest, Vec3dPlusEqualsVec3d) { // NOLINT
    Vector v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{-2., 3., 1.};
    v1 += v2;
    EXPECT_EQ(v1, (Vec3d{1, 1, 6}));
}

// Vector -= Vector
TEST(Vec3dTest, Vec3dMinusEqualsVec3d) { // NOLINT
    Vector v1{3., 2., 1.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{5., 6., 7.};
    v1 -= v2;
    EXPECT_EQ(v1, (Vec3d{-2, -4, -6}));
}

// Vector + Vector = Vector
TEST(Vec3dTest, Vec3dPlusVec3d) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{-2., 3., 1.};
    CHERRY_BLAZER_CONSTEXPR Vector v3 = v1 + v2;
    EXPECT_EQ(v3, (Vec3d{1, 1, 6}));
}

// Vector - Vector = Vector
TEST(Vec3dTest, Vec3dMinusVec3d) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{3., 2., 1.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{5., 6., 7.};
    CHERRY_BLAZER_CONSTEXPR Vector v3 = v1 - v2;
    EXPECT_EQ(v3, (Vec3d{-2, -4, -6}));
}

// Vector == Vector
TEST(Vec3dTest, Vec3dComparedToVec3dEquals) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{3., -2., 5.};
    EXPECT_EQ(v1, v2);
}

// Vector != Vector
TEST(Vec3dTest, Vec3dComparedToVec3dDoesntEqual) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{-2., 3., 1.};
    EXPECT_NE(v1, v2);
}

// Vector can be printed out.
TEST(Vec3dTest, Vec3dPrintOut) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v{1., 2., 3.};
    std::stringstream ss;
    ss << v;
    EXPECT_EQ(ss.str(), std::string{"{1, 2, 3}"});
}

// Vector has magnitude.
TEST(Vec3dTest, Vec3dMagnitude) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., 0., 0.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{0., 1., 0.};
    CHERRY_BLAZER_CONSTEXPR Vector v3{0., 0., 1.};
    CHERRY_BLAZER_CONSTEXPR Vector v4{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Vector v5{-1., -2., -3.};
    EXPECT_EQ(magnitude(v1), 1);
    EXPECT_EQ(magnitude(v2), 1);
    EXPECT_EQ(magnitude(v3), 1);
    EXPECT_DOUBLE_EQ(magnitude(v4), std::sqrt(14));
    EXPECT_DOUBLE_EQ(magnitude(v5), std::sqrt(14));
}

// Vector can be normalized.
TEST(Vec3dTest, Vec3dNormalize) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v{4., 0., 0.};
    EXPECT_EQ(normalize(v), (Vec3d{1, 0, 0}));
}

// Vectors can have a dot product.
TEST(Vec3dTest, Vec3dDotProduct) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{2., 3., 4.};
    EXPECT_EQ(dot(v1, v2), 20);
}

// Vectors can have a cross product.
TEST(Vec3dTest, Vec3dCrossProduct) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{2., 3., 4.};
    EXPECT_EQ(cross(v1, v2), (Vec3d{-1, 2, -1}));
    EXPECT_EQ(cross(v2, v1), (Vec3d{1, -2, 1}));
}

// Point-related:

// Vector += Point (= ERROR)

// Vector + Point = Point
TEST(Vec3dTest, Vec3dPlusPoint) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Point p{-2., 3., 1.};
    CHERRY_BLAZER_CONSTEXPR Point p2 = v + p;
    EXPECT_EQ(p2, (Point3d{1, 1, 6}));
}

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Vector == Point (= ERROR)

// Vector != Point (= ERROR)

} // namespace cherry_blazer

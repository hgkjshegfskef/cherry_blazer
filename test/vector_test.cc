#include <cherry_blazer/config.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/vector.hh>
#include <cherry_blazer/vector_operations.hh>

#include <gtest/gtest.h>

// Everything marked as (=ERROR) is tested to not compile in vector_test.cmake.

// This can be removed when Clang supports CTAD aliases.
using cherry_blazer::Matrix;

using cherry_blazer::Point;
using cherry_blazer::Point3d;
using cherry_blazer::Vec3d;
using cherry_blazer::Vector;

TEST(VectorTest, VectorCtors) { // NOLINT
#if __cpp_deduction_guides >= 201907
    [[maybe_unused]] Vector<double, 2> vec1;
    [[maybe_unused]] Vector vec2{1., 2.};
    [[maybe_unused]] Vector vec3{1., 2., 3.};
    //    [[maybe_unused]] Vector vec4{1., 2., 3., 4.};     // OK: won't compile: 4 arguments
    //    [[maybe_unused]] Vector vec5{{1.}, {2.}};         // OK: won't compile: ambiguous
    [[maybe_unused]] Matrix mat1{{1., 2.}, {1., 2.}}; // OK: Matrix<2,2>
    [[maybe_unused]] Vector mat2{{1., 2.}, {1., 2.}}; // OK: Matrix<2,2>, but Vector is CTAD aliased
#endif
    [[maybe_unused]] Matrix<double, 2, 1> vec6;
    [[maybe_unused]] Matrix vec7{1., 2.};
    [[maybe_unused]] Matrix vec8{1., 2., 3.};
    //    [[maybe_unused]] Matrix vec9{1., 2., 3., 4.};      // OK: won't compile: 4 arguments
    //    [[maybe_unused]] Matrix vec10{{1.}, {2.}};         // OK: won't compile: ambiguous
    [[maybe_unused]] Matrix vec11{{1., 2.}, {1., 2.}}; // OK: Matrix<2,2>
}

TEST(VectorTest, Vector2dCtor) { // NOLINT
    Matrix vec{1., 2.};
    EXPECT_EQ(vec[0], 1.);
    EXPECT_EQ(vec[1], 2.);
    EXPECT_EQ(vec[2], 1.);
}

TEST(VectorTest, Vector3dCtor) { // NOLINT
    Matrix vec{1., 2., 3.};
    EXPECT_EQ(vec[0], 1.);
    EXPECT_EQ(vec[1], 2.);
    EXPECT_EQ(vec[2], 3.);
    EXPECT_EQ(vec[3], 1.);
}

// Vector is copy constructable.
TEST(VectorTest, VectorCopyConstructable) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2{v1};
    EXPECT_EQ(v2, (Vec3d{1., 2., 3.}));
}

// Vector can be constructed from 2 Points
// TEST(VectorTest, VectorCtorTwoPoints) { // NOLINT
//     Point p1{3, -2, 5};
//     Point p2{-2, 3, 1};
//     Vec3d v{p1, p2};
//     EXPECT_EQ(v, (Vec3d{-5, 5, -4}));
// }

// -Vector
TEST(VectorTest, VectorNegate) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2 = -v1;
    EXPECT_EQ(v2, (Vec3d{-3., 2., -5.}));
}

// scalar*Vector
TEST(VectorTest, ScalarTimesVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{1., -2., 3.};
    CHERRY_BLAZER_CONSTEXPR auto scalar{3.5};
    CHERRY_BLAZER_CONSTEXPR auto v2 = scalar * v1;
    EXPECT_EQ(v2, (Vec3d{3.5, -7., 10.5}));
}

// Vector*scalar
TEST(VectorTest, VectorTimesScalar) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{1., -2., 3.};
    CHERRY_BLAZER_CONSTEXPR auto scalar{3.5};
    CHERRY_BLAZER_CONSTEXPR auto v2 = v1 * scalar;
    EXPECT_EQ(v2, (Vec3d{3.5, -7., 10.5}));
}

// Vector/scalar
TEST(VectorTest, VectorDividedByScalar) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{1., -2., 3.};
    CHERRY_BLAZER_CONSTEXPR auto scalar{2.};
    CHERRY_BLAZER_CONSTEXPR auto v2 = v1 / scalar;
    EXPECT_EQ(v2, (Vec3d{0.5, -1., 1.5}));
}

// scalar/Vector (= ERROR)

// Vector += Vector
TEST(VectorTest, VectorPlusEqualsVector) { // NOLINT
    Matrix v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2{-2., 3., 1.};
    v1 += v2;
    EXPECT_EQ(v1, (Vec3d{1., 1., 6.}));
}

// Vector -= Vector
TEST(VectorTest, VectorMinusEqualsVector) { // NOLINT
    Matrix v1{3., 2., 1.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2{5., 6., 7.};
    v1 -= v2;
    EXPECT_EQ(v1, (Vec3d{-2., -4., -6.}));
}

// Vector + Vector = Vector
TEST(VectorTest, VectorPlusVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2{-2., 3., 1.};
    CHERRY_BLAZER_CONSTEXPR Matrix v3 = v1 + v2;
    EXPECT_EQ(v3, (Vec3d{1., 1., 6.}));
}

// Vector - Vector = Vector
TEST(VectorTest, VectorMinusVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{3., 2., 1.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2{5., 6., 7.};
    CHERRY_BLAZER_CONSTEXPR Matrix v3 = v1 - v2;
    EXPECT_EQ(v3, (Vec3d{-2., -4., -6.}));
}

// Vector == Vector
TEST(VectorTest, VectorComparedToVectorEquals) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2{3., -2., 5.};
    EXPECT_EQ(v1, v2);
}

// Vector != Vector
TEST(VectorTest, VectorComparedToVectorDoesntEqual) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2{-2., 3., 1.};
    EXPECT_NE(v1, v2);
}

// Vector can be printed out.
TEST(VectorTest, VectorPrintOut) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v{1., 2., 3.};
    std::stringstream ss;
    ss << v;
    EXPECT_EQ(ss.str(), std::string{"1\n2\n3"});
}

// Vector has magnitude.
TEST(VectorTest, VectorMagnitude) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{1., 0., 0.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2{0., 1., 0.};
    CHERRY_BLAZER_CONSTEXPR Matrix v3{0., 0., 1.};
    CHERRY_BLAZER_CONSTEXPR Matrix v4{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Matrix v5{-1., -2., -3.};
    CHERRY_BLAZER_CONSTEXPR auto const result1 = magnitude(v1);
    CHERRY_BLAZER_CONSTEXPR auto const result2 = magnitude(v2);
    CHERRY_BLAZER_CONSTEXPR auto const result3 = magnitude(v3);
    CHERRY_BLAZER_CONSTEXPR auto const result4 = magnitude(v4);
    CHERRY_BLAZER_CONSTEXPR auto const result5 = magnitude(v5);
    EXPECT_EQ(result1, 1);
    EXPECT_EQ(result2, 1);
    EXPECT_EQ(result3, 1);
    EXPECT_DOUBLE_EQ(result4, std::sqrt(14));
    EXPECT_DOUBLE_EQ(result5, std::sqrt(14));
}

// Vector can be normalized.
TEST(VectorTest, VectorNormalize) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v{4., 0., 0.};
    CHERRY_BLAZER_CONSTEXPR auto const result = normalize(v);
    EXPECT_EQ(result, (Vec3d{1., 0., 0.}));
}

// Vectors can have a dot product.
TEST(VectorTest, VectorDotProduct) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2{2., 3., 4.};
    CHERRY_BLAZER_CONSTEXPR auto const result = dot(v1, v2);
    EXPECT_EQ(result, 20);
}

// Vectors can have a cross product.
TEST(VectorTest, VectorCrossProduct) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Matrix v2{2., 3., 4.};
    CHERRY_BLAZER_CONSTEXPR auto const result1 = cross(v1, v2);
    CHERRY_BLAZER_CONSTEXPR auto const result2 = cross(v2, v1);
    EXPECT_EQ(result1, (Vec3d{-1., 2., -1.}));
    EXPECT_EQ(result2, (Vec3d{1., -2., 1.}));
}

// Point-related:

// Vector += Point (= ERROR)

// Vector + Point = Point
TEST(VectorTest, VectorPlusPoint) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Matrix v{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Point p{-2., 3., 1.};
    CHERRY_BLAZER_CONSTEXPR Point p2 = v + p;
    EXPECT_EQ(p2, (Point3d{1., 1., 6.}));
}

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Vector == Point (= ERROR)

// Vector != Point (= ERROR)

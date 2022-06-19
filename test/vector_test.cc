#include <cherry_blazer/config.hh>
#include <cherry_blazer/coord.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/vector.hh>
#include <cherry_blazer/vector_operations.hh>

#include <gtest/gtest.h>

// Everything marked as (=ERROR) is tested to not compile in vector_test.cmake.

using cherry_blazer::Coord;
using cherry_blazer::Point;
using cherry_blazer::Vector;

TEST(VectorTest, VectorCtors) { // NOLINT
    //    Vector<double, 1> vec1;                                     // OK: won't compile: 1D
    [[maybe_unused]] Vector<double, 2> vec1;                          // default, 2D
    [[maybe_unused]] CHERRY_BLAZER_CONSTEXPR Vector vec2{1., 2.};     // 2D, deduced
    [[maybe_unused]] CHERRY_BLAZER_CONSTEXPR Vector vec3{1., 2., 3.}; // 3D, deduced
    //    [[maybe_unused]] Vector vec4{1., 2., 3., 4.}; // OK: won't compile: 4 arguments
    //    [[maybe_unused]] Vector vec5{{1.}, {2.}};     // OK: won't compile: ambiguous
}

TEST(VectorTest, Vector2dDefaultCtorZero) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector<double, 2> vec{};

    EXPECT_DOUBLE_EQ(vec[Coord::X], 0.);
    EXPECT_DOUBLE_EQ(vec[Coord::Y], 0.);
    EXPECT_DOUBLE_EQ(vec[Coord::Z], 0.);
}

TEST(VectorTest, Vector3dDefaultCtorZero) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector<double, 3> vec{};

    EXPECT_DOUBLE_EQ(vec[Coord::X], 0.);
    EXPECT_DOUBLE_EQ(vec[Coord::Y], 0.);
    EXPECT_DOUBLE_EQ(vec[Coord::Z], 0.);
    EXPECT_DOUBLE_EQ(vec[Coord::W], 0.);
}

TEST(VectorTest, Vector2dCtor) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector vec{1., 2.};

    EXPECT_DOUBLE_EQ(vec[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(vec[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(vec[Coord::Z], 0.);
}

TEST(VectorTest, Vector3dCtor) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector vec{1., 2., 3.};

    EXPECT_DOUBLE_EQ(vec[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(vec[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(vec[Coord::Z], 3.);
    EXPECT_DOUBLE_EQ(vec[Coord::W], 0.);
}

// Vector is copy constructable.
TEST(VectorTest, VectorCopyConstructable) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{v1};

    EXPECT_DOUBLE_EQ(v2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 3.);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// Vector is copy assign constructable.
TEST(VectorTest, VectorCopyAssignConstructable) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Vector v2 = v1;

    EXPECT_DOUBLE_EQ(v2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 3.);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// Vector is copyable.
TEST(VectorTest, VectorCopyable) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., 2., 3.};
    Vector v2{4., 5., 6.};

    v2 = v1;

    EXPECT_DOUBLE_EQ(v2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 3.);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// Vector is move constructable.
TEST(VectorTest, VectorMoveConstructable) { // NOLINT
    Vector v1{1., 2., 3.};
    Vector v2{std::move(v1)};

    EXPECT_DOUBLE_EQ(v2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 3.);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// Vector is move assign constructable.
TEST(VectorTest, VectorMoveAssignConstructable) { // NOLINT
    Vector v1{1., 2., 3.};
    Vector v2 = std::move(v1);

    EXPECT_DOUBLE_EQ(v2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 3.);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// Vector is movable.
TEST(VectorTest, VectorMovable) { // NOLINT
    Vector v1{1., 2., 3.};
    Vector v2{4., 5., 6.};

    v2 = std::move(v1);

    EXPECT_DOUBLE_EQ(v2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 3.);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// Vector can be constructed from 2 Points
TEST(VectorTest, VectorCtorTwoPoints) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Point p2{-2., 3., 1.};
    CHERRY_BLAZER_CONSTEXPR Vector v{p1, p2};

    EXPECT_DOUBLE_EQ(v[Coord::X], -5.);
    EXPECT_DOUBLE_EQ(v[Coord::Y], 5.);
    EXPECT_DOUBLE_EQ(v[Coord::Z], -4.);
    EXPECT_DOUBLE_EQ(v[Coord::W], 0.);
}

// Vector has operator[] for unchecked indexing
TEST(VectorTest, VectorSubscriptOperator) {
    CHERRY_BLAZER_CONSTEXPR Vector vec{1., 2., 3.};

    auto const& x = vec[Coord::X];
    auto const& y = vec[Coord::Y];
    auto const& z = vec[Coord::Z];
    auto const& w = vec[Coord::W];

    EXPECT_DOUBLE_EQ(x, 1.);
    EXPECT_DOUBLE_EQ(y, 2.);
    EXPECT_DOUBLE_EQ(z, 3.);
    EXPECT_DOUBLE_EQ(w, 0.);
}

// Vector has get<> for compile-time checked indexing
TEST(VectorTest, VectorGetMember) {
    CHERRY_BLAZER_CONSTEXPR Vector vec{1., 2., 3.};

    auto const& x = vec.get<Coord::X>();
    auto const& y = vec.get<Coord::Y>();
    auto const& z = vec.get<Coord::Z>();
    // auto const& w = vec.get<Coord::W>(); // OK, won't compile: vector is 3D
    // Still, check the W coordinate through unchecked access:
    auto const& w = vec[Coord::W];

    EXPECT_DOUBLE_EQ(x, 1.);
    EXPECT_DOUBLE_EQ(y, 2.);
    EXPECT_DOUBLE_EQ(z, 3.);
    EXPECT_DOUBLE_EQ(w, 0.);
}

// Vector has generic get<> for compile-time checked indexing
TEST(VectorTest, VectorGetGeneric) {
    CHERRY_BLAZER_CONSTEXPR Vector vec{1., 2., 3.};

    auto const& x = get<Coord::X>(vec);
    auto const& y = get<Coord::Y>(vec);
    auto const& z = get<Coord::Z>(vec);
    // auto const& w = get<Coord::W>(vec); // OK, won't compile: vector is 3D
    // Still, check the W coordinate through unchecked access:
    auto const& w = vec[Coord::W];

    EXPECT_DOUBLE_EQ(x, 1.);
    EXPECT_DOUBLE_EQ(y, 2.);
    EXPECT_DOUBLE_EQ(z, 3.);
    EXPECT_DOUBLE_EQ(w, 0.);
}

// -Vector
TEST(VectorTest, VectorNegate) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v2 = -v1;

    EXPECT_DOUBLE_EQ(v2[Coord::X], -3.);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], -5.);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// scalar*Vector
TEST(VectorTest, ScalarTimesVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., -2., 3.};
    CHERRY_BLAZER_CONSTEXPR auto scalar{3.5};
    CHERRY_BLAZER_CONSTEXPR auto v2 = scalar * v1;

    EXPECT_DOUBLE_EQ(v2[Coord::X], 3.5);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], -7.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 10.5);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// Vector*scalar
TEST(VectorTest, VectorTimesScalar) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., -2., 3.};
    CHERRY_BLAZER_CONSTEXPR auto scalar{3.5};
    CHERRY_BLAZER_CONSTEXPR auto v2 = v1 * scalar;

    EXPECT_DOUBLE_EQ(v2[Coord::X], 3.5);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], -7.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 10.5);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// Vector/scalar
TEST(VectorTest, VectorDividedByScalar) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., -2., 3.};
    CHERRY_BLAZER_CONSTEXPR auto scalar{2.};
    CHERRY_BLAZER_CONSTEXPR auto v2 = v1 / scalar;

    EXPECT_DOUBLE_EQ(v2[Coord::X], 0.5);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], -1.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 1.5);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// scalar/Vector (= ERROR)

// Vector += Vector
TEST(VectorTest, VectorPlusEqualsVector) { // NOLINT
    Vector v1{3., -2., 5.};
    Vector v2{-2., 3., 1.};
    v2 += v1;

    EXPECT_DOUBLE_EQ(v2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], 1.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 6.);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// Vector -= Vector
TEST(VectorTest, VectorMinusEqualsVector) { // NOLINT
    Vector v1{3., 2., 1.};
    Vector v2{5., 6., 7.};
    v2 -= v1;

    EXPECT_DOUBLE_EQ(v2[Coord::X], 2.);
    EXPECT_DOUBLE_EQ(v2[Coord::Y], 4.);
    EXPECT_DOUBLE_EQ(v2[Coord::Z], 6.);
    EXPECT_DOUBLE_EQ(v2[Coord::W], 0.);
}

// Vector + Vector = Vector
TEST(VectorTest, VectorPlusVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{-2., 3., 1.};
    CHERRY_BLAZER_CONSTEXPR Vector v3 = v1 + v2;

    EXPECT_DOUBLE_EQ(v3[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(v3[Coord::Y], 1.);
    EXPECT_DOUBLE_EQ(v3[Coord::Z], 6.);
    EXPECT_DOUBLE_EQ(v3[Coord::W], 0.);
}

// Vector - Vector = Vector
TEST(VectorTest, VectorMinusVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{3., 2., 1.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{5., 6., 7.};
    CHERRY_BLAZER_CONSTEXPR Vector v3 = v1 - v2;

    EXPECT_DOUBLE_EQ(v3[Coord::X], -2.);
    EXPECT_DOUBLE_EQ(v3[Coord::Y], -4.);
    EXPECT_DOUBLE_EQ(v3[Coord::Z], -6.);
    EXPECT_DOUBLE_EQ(v3[Coord::W], 0.);
}

// Vector == Vector
TEST(VectorTest, VectorComparedToVectorEquals) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{3., -2., 5.};

    EXPECT_TRUE(v1 == v2);
}

// Vector != Vector
TEST(VectorTest, VectorComparedToVectorDoesntEqual) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{-2., 3., 1.};

    EXPECT_TRUE(v1 != v2);
}

// Vector can be printed out.
TEST(VectorTest, VectorPrintOut) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v{1., 2., 3.};
    std::stringstream ss;
    ss << v;
    EXPECT_EQ(ss.str(), std::string{"[1, 2, 3]"});
}

// Vector has magnitude.
TEST(VectorTest, VectorMagnitude) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., 0., 0.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{0., 1., 0.};
    CHERRY_BLAZER_CONSTEXPR Vector v3{0., 0., 1.};
    CHERRY_BLAZER_CONSTEXPR Vector v4{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Vector v5{-1., -2., -3.};
    CHERRY_BLAZER_CONSTEXPR auto const result1 = magnitude(v1);
    CHERRY_BLAZER_CONSTEXPR auto const result2 = magnitude(v2);
    CHERRY_BLAZER_CONSTEXPR auto const result3 = magnitude(v3);
    CHERRY_BLAZER_CONSTEXPR auto const result4 = magnitude(v4);
    CHERRY_BLAZER_CONSTEXPR auto const result5 = magnitude(v5);
    EXPECT_DOUBLE_EQ(result1, 1);
    EXPECT_DOUBLE_EQ(result2, 1);
    EXPECT_DOUBLE_EQ(result3, 1);
    EXPECT_DOUBLE_EQ(result4, std::sqrt(14));
    EXPECT_DOUBLE_EQ(result5, std::sqrt(14));
}

// Vector can be normalized.
TEST(VectorTest, VectorNormalize) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v{4., 0., 0.};
    CHERRY_BLAZER_CONSTEXPR auto const result = normalize(v);

    EXPECT_DOUBLE_EQ(result[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(result[Coord::Y], 0.);
    EXPECT_DOUBLE_EQ(result[Coord::Z], 0.);
    EXPECT_DOUBLE_EQ(result[Coord::W], 0.);
}

// Vectors can have a dot product.
TEST(VectorTest, VectorDotProduct) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{2., 3., 4.};
    CHERRY_BLAZER_CONSTEXPR auto const result = dot(v1, v2);
    EXPECT_DOUBLE_EQ(result, 20.);
}

// Vectors can have a cross product.
TEST(VectorTest, VectorCrossProduct) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Vector v2{2., 3., 4.};

    CHERRY_BLAZER_CONSTEXPR auto const result1 = cross(v1, v2);

    EXPECT_DOUBLE_EQ(result1[Coord::X], -1.);
    EXPECT_DOUBLE_EQ(result1[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(result1[Coord::Z], -1.);
    EXPECT_DOUBLE_EQ(result1[Coord::W], 0.);

    CHERRY_BLAZER_CONSTEXPR auto const result2 = cross(v2, v1);

    EXPECT_DOUBLE_EQ(result2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(result2[Coord::Y], -2.);
    EXPECT_DOUBLE_EQ(result2[Coord::Z], 1.);
    EXPECT_DOUBLE_EQ(result2[Coord::W], 0.);
}

// Point-related:

// Vector += Point (= ERROR)

// Vector + Point = Point
TEST(VectorTest, VectorPlusPoint) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Vector v{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Point p{-2., 3., 1.};
    CHERRY_BLAZER_CONSTEXPR Point p2 = v + p;

    EXPECT_DOUBLE_EQ(p2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(p2[Coord::Y], 1.);
    EXPECT_DOUBLE_EQ(p2[Coord::Z], 6.);
    EXPECT_DOUBLE_EQ(p2[Coord::W], 1.);
}

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Vector == Point (= ERROR)

// Vector != Point (= ERROR)

#include <cherry_blazer/config.hh>
#include <cherry_blazer/coord.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/point_operations.hh>
#include <cherry_blazer/vector.hh>

#include <gtest/gtest.h>

#include <string>

using cherry_blazer::Coord;
using cherry_blazer::Point;
using cherry_blazer::Vector;

// Everything marked as (=ERROR) is tested to not compile in point_test.cmake.

TEST(PointTest, Point2dCtor) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point point{1., 2.};

    EXPECT_DOUBLE_EQ(point[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(point[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(point[Coord::Z], 1.);
}

TEST(PointTest, Point3dCtor) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point point{1., 2., 3.};

    EXPECT_DOUBLE_EQ(point[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(point[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(point[Coord::Z], 3.);
    EXPECT_DOUBLE_EQ(point[Coord::W], 1.);
}

TEST(PointTest, PointCopyConstructable) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Point p2{p1};

    EXPECT_DOUBLE_EQ(p2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(p2[Coord::Y], 2.);
    EXPECT_DOUBLE_EQ(p2[Coord::Z], 3.);
    EXPECT_DOUBLE_EQ(p2[Coord::W], 1.);
}

// Point has operator[] for unchecked indexing
TEST(PointTest, PointSubscriptOperator) {
    CHERRY_BLAZER_CONSTEXPR Point point{1., 2., 3.};

    auto const& x = point[Coord::X];
    auto const& y = point[Coord::Y];
    auto const& z = point[Coord::Z];
    auto const& w = point[Coord::W];

    EXPECT_DOUBLE_EQ(x, 1.);
    EXPECT_DOUBLE_EQ(y, 2.);
    EXPECT_DOUBLE_EQ(z, 3.);
    EXPECT_DOUBLE_EQ(w, 1.);
}

// Point has get<> for compile-time checked indexing
TEST(PointTest, PointGetMember) {
    CHERRY_BLAZER_CONSTEXPR Point point{1., 2., 3.};

    auto const& x = point.get<Coord::X>();
    auto const& y = point.get<Coord::Y>();
    auto const& z = point.get<Coord::Z>();
    // auto const& w = vec.get<Coord::W>(); // OK, won't compile: vector is 3D
    // Still, check the W coordinate through unchecked access:
    auto const& w = point[Coord::W];

    EXPECT_DOUBLE_EQ(x, 1.);
    EXPECT_DOUBLE_EQ(y, 2.);
    EXPECT_DOUBLE_EQ(z, 3.);
    EXPECT_DOUBLE_EQ(w, 1.);
}

// Point has generic get<> for compile-time checked indexing
TEST(PointTest, PointGetGeneric) {
    CHERRY_BLAZER_CONSTEXPR Point point{1., 2., 3.};

    auto const& x = get<Coord::X>(point);
    auto const& y = get<Coord::Y>(point);
    auto const& z = get<Coord::Z>(point);
    // auto const& w = get<Coord::W>(vec); // OK, won't compile: vector is 3D
    // Still, check the W coordinate through unchecked access:
    auto const& w = point[Coord::W];

    EXPECT_DOUBLE_EQ(x, 1.);
    EXPECT_DOUBLE_EQ(y, 2.);
    EXPECT_DOUBLE_EQ(z, 3.);
    EXPECT_DOUBLE_EQ(w, 1.);
}

// Point == Point
TEST(PointTest, PointComparedToPointEquals) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Point p2{3., -2., 5.};
    EXPECT_TRUE(p1 == p2);
}

// Point != Point
TEST(PointTest, PointComparedToPointDoesntEqual) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Point p2{-2., 3., 1.};
    EXPECT_TRUE(p1 != p2);
}

TEST(PointTest, PointPrintOut) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p{1., 2., 3.};
    std::stringstream ss;
    ss << p;
    EXPECT_EQ(ss.str(), std::string{"[1, 2, 3]"});
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
TEST(PointTest, PointMinusPoint) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p1{3., 2., 1.};
    CHERRY_BLAZER_CONSTEXPR Point p2{5., 6., 7.};
    CHERRY_BLAZER_CONSTEXPR Vector v = p1 - p2;

    EXPECT_DOUBLE_EQ(v[Coord::X], -2.);
    EXPECT_DOUBLE_EQ(v[Coord::Y], -4.);
    EXPECT_DOUBLE_EQ(v[Coord::Z], -6.);
    EXPECT_DOUBLE_EQ(v[Coord::W], 0.);
}

// Vector-related:

// Point += Vector
TEST(PointTest, PointPlusEqualsVector) { // NOLINT
    Point p{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v{-2., 3., 1.};
    p += v;

    EXPECT_DOUBLE_EQ(p[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(p[Coord::Y], 1.);
    EXPECT_DOUBLE_EQ(p[Coord::Z], 6.);
    EXPECT_DOUBLE_EQ(p[Coord::W], 1.);
}

// Point + Vector = Point
TEST(PointTest, PointPlusVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Vector v{-2., 3., 1.};
    CHERRY_BLAZER_CONSTEXPR Point p2 = p + v;

    EXPECT_DOUBLE_EQ(p2[Coord::X], 1.);
    EXPECT_DOUBLE_EQ(p2[Coord::Y], 1.);
    EXPECT_DOUBLE_EQ(p2[Coord::Z], 6.);
    EXPECT_DOUBLE_EQ(p2[Coord::W], 1.);
}

// Point -= Vector
TEST(PointTest, PointMinusEqualsVector) { // NOLINT
    Point p{3., 2., 1.};
    CHERRY_BLAZER_CONSTEXPR Vector v{5., 6., 7.};
    p -= v;

    EXPECT_DOUBLE_EQ(p[Coord::X], -2.);
    EXPECT_DOUBLE_EQ(p[Coord::Y], -4.);
    EXPECT_DOUBLE_EQ(p[Coord::Z], -6.);
    EXPECT_DOUBLE_EQ(p[Coord::W], 1.);
}

// Point - Vector = Point
TEST(PointTest, PointMinusVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p{3., 2., 1.};
    CHERRY_BLAZER_CONSTEXPR Vector v{5., 6., 7.};
    CHERRY_BLAZER_CONSTEXPR Point p2 = p - v;

    EXPECT_DOUBLE_EQ(p2[Coord::X], -2.);
    EXPECT_DOUBLE_EQ(p2[Coord::Y], -4.);
    EXPECT_DOUBLE_EQ(p2[Coord::Z], -6.);
    EXPECT_DOUBLE_EQ(p2[Coord::W], 1.);
}

// Point == Vector (= ERROR)

// Point != Vector (= ERROR)

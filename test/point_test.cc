#include <cherry_blazer/config.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/point_operations.hh>
#include <cherry_blazer/vector.hh>

#include <gtest/gtest.h>

#include <string>

using cherry_blazer::Matrix;
using cherry_blazer::Point;
using cherry_blazer::Point3d;
using cherry_blazer::Vec3d;
using cherry_blazer::Vector;

// Everything marked as (=ERROR) is tested to not compile in point_test.cmake.

TEST(PointTest, Point2dCtor) { // NOLINT
    Point point{1., 2.};
    EXPECT_EQ(point[0], 1.);
    EXPECT_EQ(point[1], 2.);
    EXPECT_EQ(point[2], 1.);
}

TEST(PointTest, Point3dCtor) { // NOLINT
    Point point{1., 2., 3.};
    EXPECT_EQ(point[0], 1.);
    EXPECT_EQ(point[1], 2.);
    EXPECT_EQ(point[2], 3.);
    EXPECT_EQ(point[3], 1.);
}

TEST(PointTest, PointCopyConstructable) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p1{1., 2., 3.};
    CHERRY_BLAZER_CONSTEXPR Point p2{p1};
    EXPECT_EQ(p2, (Point3d{1., 2., 3.}));
}

// Point == Point
TEST(PointTest, PointComparedToPointEquals) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Point p2{3., -2., 5.};
    EXPECT_EQ(p1, p2);
}

// Point != Point
TEST(PointTest, PointComparedToPointDoesntEqual) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p1{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Point p2{-2., 3., 1.};
    EXPECT_NE(p1, p2);
}

TEST(PointTest, PointPrintOut) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p{1., 2., 3.};
    std::stringstream ss;
    ss << p;
    EXPECT_EQ(ss.str(), std::string{"1\n2\n3"});
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
    CHERRY_BLAZER_CONSTEXPR auto v = p1 - p2;
    EXPECT_EQ(v, (Vec3d{-2., -4., -6.}));
}

// Vector-related:

// Point += Vector
TEST(PointTest, PointPlusEqualsVector) { // NOLINT
    Point p{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Matrix v{-2., 3., 1.};
    p += v;
    EXPECT_EQ(p, (Point3d{1., 1., 6.}));
}

// Point + Vector = Point
TEST(PointTest, PointPlusVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p{3., -2., 5.};
    CHERRY_BLAZER_CONSTEXPR Matrix v{-2., 3., 1.};
    CHERRY_BLAZER_CONSTEXPR Point p2 = p + v;
    EXPECT_EQ(p2, (Point3d{1., 1., 6.}));
}

// Point -= Vector
TEST(PointTest, PointMinusEqualsVector) { // NOLINT
    Point p{3., 2., 1.};
    CHERRY_BLAZER_CONSTEXPR Matrix v{5., 6., 7.};
    p -= v;
    EXPECT_EQ(p, (Point3d{-2., -4., -6.}));
}

// Point - Vector = Point
TEST(PointTest, PointMinusVector) { // NOLINT
    CHERRY_BLAZER_CONSTEXPR Point p{3., 2., 1.};
    CHERRY_BLAZER_CONSTEXPR Matrix v{5., 6., 7.};
    CHERRY_BLAZER_CONSTEXPR Point p2 = p - v;
    EXPECT_EQ(p2, (Point3d{-2., -4., -6.}));
}

// Point == Vector (= ERROR)

// Point != Vector (= ERROR)

#ifndef CHERRY_BLAZER_SRC_POINT_HH_
#define CHERRY_BLAZER_SRC_POINT_HH_

#include "tuple.hh"
#include "vector.hh"

#include <type_traits>

struct Point : Tuple {
    Point() = default;
    using Tuple::Tuple;

    // Point += Point (= ERROR)
    Point& operator+=(Point const& rhs) = delete;

    // Point -= Point (= ERROR)
    Point& operator-=(Point const& rhs) = delete;

    // Point + Point (= ERROR)
    Point operator+(Point const& rhs) const = delete;

    // Vector-related operations:

    // Point cannot be compared to Vector.
    bool operator==(Vector const& rhs) const = delete;
    bool operator!=(Vector const& rhs) const = delete;
};

static_assert(
    std::is_trivially_copyable_v<Point>,
    "Being trivially copyable means that compiler can optimize a bit better when this struct is "
    "used. For example, it can memcpy it instead of calling copy constructor. Or it can pass the "
    "struct inside a register, instead of passing a pointer to the struct. Considering how often "
    "this struct will be used in the project, it is preferable to keep it trivially copyable.");

// Point*scalar (= ERROR)
Point operator*(Point const& p, double scalar) = delete;

// scalar*Point (= ERROR)
Point operator*(double scalar, Point const& p) = delete;

// Point/scalar (= ERROR)
Point operator/(Point const& p, double scalar) = delete;

// scalar*Point (= ERROR)
Point operator/(double scalar, Point const& p) = delete;

// Vector-related operations:

// Point += Vector
Point& operator+=(Point& lhs, Vector const& rhs);

// Point -= Vector
Point& operator-=(Point& lhs, Vector const& rhs);

// Point + Vector = Point
Point operator+(Point lhs, Vector const& rhs);

// Point - Point = Vector (pointing from rhs to lhs)
Vector operator-(Point const& lhs, Point const& rhs);

// Point - Vector = Point (move backward by vector)
Point operator-(Point lhs, Vector const& rhs);

#endif // CHERRY_BLAZER_SRC_POINT_HH_

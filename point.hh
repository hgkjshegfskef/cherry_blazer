#ifndef CHERRY_BLAZER__POINT_HH_
#define CHERRY_BLAZER__POINT_HH_

#include <type_traits>

#include "tuple.hh"
#include "vector.hh"

struct Point : Tuple {
    Point() = default;
    using Tuple::Tuple;

    // Point += Vector (= Point)
    Point& operator+=(Vector const& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    // Point + Vector = Point
    friend Point operator+(Point lhs, Vector const& rhs) {
        lhs += rhs;
        return lhs;
    }

    // Point + Point = ERROR
    Point& operator+=(Point const& rhs) = delete;
    // operator+ is implemented as member to delete the operation
    Point operator+(Point const& rhs) const = delete;
};

static_assert(
    std::is_trivially_copyable_v<Point>,
    "Being trivially copyable means that compiler can optimize a bit better when this struct is "
    "used. For example, it can memcpy it instead of calling copy constructor. Or it can pass the "
    "struct inside a register, instead of passing a pointer to the struct. Considering how often "
    "this struct will be used in the project, it is preferable to keep it trivially copyable.");

#endif // CHERRY_BLAZER__POINT_HH_

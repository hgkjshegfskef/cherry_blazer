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
static_assert(std::is_trivially_copyable_v<Point>);

#endif // CHERRY_BLAZER__POINT_HH_

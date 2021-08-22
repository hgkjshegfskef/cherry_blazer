#ifndef CHERRY_BLAZER__POINT_HH_
#define CHERRY_BLAZER__POINT_HH_

#include <type_traits>

#include "tuple.hh"
#include "vector.hh"

struct Point : Tuple {
    Point() = default;
    using Tuple::Tuple;

    // point += vector (= point)
    Point& operator+=(Vector const& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    // point + vector = point
    friend Point operator+(Point lhs, Vector const& rhs) {
        lhs += rhs;
        return lhs;
    }

    // point + point = makes no sense
    Point& operator+=(Point const& rhs) = delete;
    Point operator+(Point const& rhs) const = delete;
};
static_assert(std::is_trivially_copyable_v<Point>);

#endif // CHERRY_BLAZER__POINT_HH_

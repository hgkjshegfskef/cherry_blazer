#ifndef CHERRY_BLAZER__POINT_HH_
#define CHERRY_BLAZER__POINT_HH_

#include <type_traits>

#include "tuple.hh"
#include "vector.hh"

struct Point : Tuple {
    Point() = default;
    // Tuple.w == 1 is a point
    Point(double x_, double y_, double z_) : Tuple{x_, y_, z_, 1.0} {}

    // point += vector (= point)
    Point& operator+=(Vector const& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
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

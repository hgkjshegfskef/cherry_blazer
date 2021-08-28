#include "point.hh"

// Point += Vector (= Point)
Point& Point::operator+=(const Vector& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

// Point + Vector = Point
Point operator+(Point lhs, const Vector& rhs) {
    lhs += rhs;
    return lhs;
}

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

Point& Point::operator-=(const Vector& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vector operator-(const Point& lhs, const Point& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

Point operator-(Point lhs, Vector const& rhs) {
    lhs -= rhs;
    return lhs;
}

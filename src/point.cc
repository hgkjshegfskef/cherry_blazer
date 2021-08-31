#include "point.hh"

namespace cherry_blazer {

Point& operator+=(Point& lhs, const Vector& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

Point& operator-=(Point& lhs, const Vector& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

Point operator+(Point lhs, const Vector& rhs) {
    lhs += rhs;
    return lhs;
}

Vector operator-(const Point& lhs, const Point& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

Point operator-(Point lhs, Vector const& rhs) {
    lhs -= rhs;
    return lhs;
}

} // namespace cherry_blazer

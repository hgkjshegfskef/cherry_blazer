#include "point.hh"

namespace cherry_blazer {

Point& operator+=(Point& lhs, Vector const& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

Point& operator-=(Point& lhs, Vector const& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

Point operator+(Point lhs, Vector const& rhs) {
    lhs += rhs;
    return lhs;
}

Vector operator-(Point const& lhs, Point const& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

Point operator-(Point lhs, Vector const& rhs) {
    lhs -= rhs;
    return lhs;
}

} // namespace cherry_blazer

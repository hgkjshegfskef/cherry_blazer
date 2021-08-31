#include "point.hh"

#include "util.hh"
#include "vector.hh"

#include <cmath>
#include <iomanip>
#include <limits>

namespace cherry_blazer {

Point::Point(double x, double y, double z) : x{x}, y{y}, z{z} {}

Point& operator+=(Point& lhs, Vector const& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

Point operator+(Point lhs, Vector const& rhs) { return lhs += rhs; }

Point& operator-=(Point& lhs, Vector const& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

Point operator-(Point lhs, Vector const& rhs) { return lhs -= rhs; }

Vector operator-(Point const& lhs, Point const& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

bool operator==(Point const& lhs, Point const& rhs) {
    // floating-point comparison through epsilon
    return almost_equal(lhs.x, rhs.x) && almost_equal(lhs.y, rhs.y) && almost_equal(lhs.z, rhs.z);
}

bool operator!=(Point const& lhs, Point const& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& os, Point const& v) {
    return os << std::setprecision(std::numeric_limits<double>::max_digits10) << "{" << v.x << ", "
              << v.y << ", " << v.z << "}";
}

} // namespace cherry_blazer

#include "tuple.hh"

#include "util.hh"

#include <iomanip>
#include <limits>

namespace cherry_blazer {

Tuple::Tuple(double x, double y, double z) : x{x}, y{y}, z{z} {}

Tuple operator-(Tuple const& t) { return {-t.x, -t.y, -t.z}; }

Tuple operator*(Tuple const& t, double scalar) {
    return {t.x * scalar, t.y * scalar, t.z * scalar};
}

Tuple operator*(double scalar, Tuple const& t) { return t * scalar; }

Tuple operator/(Tuple const& t, double scalar) {
    return {t.x / scalar, t.y / scalar, t.z / scalar};
}

Tuple& operator+=(Tuple& lhs, Tuple const& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

Tuple& operator-=(Tuple& lhs, Tuple const& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

Tuple operator+(Tuple lhs, Tuple const& rhs) {
    lhs += rhs;
    return lhs;
}

Tuple operator-(Tuple lhs, Tuple const& rhs) {
    lhs -= rhs;
    return lhs;
}

bool operator==(Tuple const& lhs, Tuple const& rhs) {
    // floating-point comparison through epsilon
    return almost_equal(lhs.x, rhs.x) && almost_equal(lhs.y, rhs.y) && almost_equal(lhs.z, rhs.z);
}

bool operator!=(Tuple const& lhs, Tuple const& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& os, Tuple const& t) {
    return os << std::setprecision(std::numeric_limits<double>::max_digits10) << "{" << t.x << ", "
              << t.y << ", " << t.z << "}";
}

} // namespace cherry_blazer

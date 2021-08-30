#include "tuple.hh"

#include "util.hh"

Tuple::Tuple(double x, double y, double z) : x{x}, y{y}, z{z} {}

Tuple operator-(Tuple const& t) { return {-t.x, -t.y, -t.z}; }

Tuple& operator+=(Tuple& lhs, const Tuple& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

Tuple& operator-=(Tuple& lhs, const Tuple& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

Tuple operator+(Tuple lhs, const Tuple& rhs) {
    lhs += rhs;
    return lhs;
}

Tuple operator-(Tuple lhs, const Tuple& rhs) {
    lhs -= rhs;
    return lhs;
}

bool operator==(const Tuple& lhs, const Tuple& rhs) {
    // floating-point comparison through epsilon
    return almost_equal(lhs.x, rhs.x) && almost_equal(lhs.y, rhs.y) && almost_equal(lhs.z, rhs.z);
}

bool operator!=(const Tuple& lhs, const Tuple& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& os, const Tuple& t) {
    return os << "{" << t.x << ", " << t.y << ", " << t.z << "}";
}

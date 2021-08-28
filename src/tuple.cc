#include "tuple.hh"

Tuple::Tuple(double x_, double y_, double z_) : x{x_}, y{y_}, z{z_} {}

// Tuple += Tuple (= Tuple)
Tuple& Tuple::operator+=(const Tuple& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Tuple operator+(Tuple lhs, const Tuple& rhs) {
    lhs += rhs;
    return lhs;
}

bool operator==(const Tuple& lhs, const Tuple& rhs) {
    // floating-point comparison through epsilon
    return almost_equal(lhs.x, rhs.x) && almost_equal(lhs.y, rhs.y) && almost_equal(lhs.z, rhs.z);
}

bool operator!=(const Tuple& lhs, const Tuple& rhs) { return !(lhs == rhs); }
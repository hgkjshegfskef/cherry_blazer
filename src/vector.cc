#include "vector.hh"

#include "point.hh"

#include <cmath>

Vector::Vector(const Point& begin, const Point& end) {
    // Cannot initialize members declared by Tuple, because the Tuple constructor has already
    // initialized them. Thus, reassign:
    x = end.x - begin.x;
    y = end.y - begin.y;
    z = end.z - begin.z;
}

double magnitude(const Vector& v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

Vector normalize(const Vector& v) {
    auto mag = magnitude(v);
    return {v.x / mag, v.y / mag, v.z / mag};
}

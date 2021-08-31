#include "vector.hh"

#include "point.hh"

#include <cmath>

namespace cherry_blazer {

Vector::Vector(Point const& begin, Point const& end) {
    // Cannot initialize members declared by Tuple, because the Tuple constructor has already
    // initialized them. Thus, reassign:
    x = end.x - begin.x;
    y = end.y - begin.y;
    z = end.z - begin.z;
}

double magnitude(Vector const& v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

Vector normalize(Vector const& v) {
    auto mag = magnitude(v);
    return {v.x / mag, v.y / mag, v.z / mag};
}

double dot(Vector const& v1, Vector const& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

Vector cross(Vector const& v1, Vector const& v2) {
    return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

} // namespace cherry_blazer

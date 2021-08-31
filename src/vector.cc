#include "vector.hh"

#include "point.hh"
#include "util.hh"

#include <cmath>
#include <iomanip>
#include <limits>

namespace cherry_blazer {

Vector::Vector(double x, double y, double z) : x{x}, y{y}, z{z} {}

Vector::Vector(Point const& begin, Point const& end)
    : x{end.x - begin.x}, y{end.y - begin.y}, z{end.z - begin.z} {}

Vector operator-(Vector const& v) { return {-v.x, -v.y, -v.z}; }

Vector operator*(Vector const& v, double scalar) {
    return {v.x * scalar, v.y * scalar, v.z * scalar};
}

Vector operator*(double scalar, Vector const& v) { return v * scalar; }

Vector operator/(Vector const& v, double scalar) {
    return {v.x / scalar, v.y / scalar, v.z / scalar};
}

Vector& operator+=(Vector& lhs, Vector const& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

Vector& operator-=(Vector& lhs, Vector const& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

Vector operator+(Vector lhs, Vector const& rhs) { return lhs += rhs; }

Vector operator-(Vector lhs, Vector const& rhs) { return lhs -= rhs; }

bool operator==(Vector const& lhs, Vector const& rhs) {
    // floating-point comparison through epsilon
    return almost_equal(lhs.x, rhs.x) && almost_equal(lhs.y, rhs.y) && almost_equal(lhs.z, rhs.z);
}

bool operator!=(Vector const& lhs, Vector const& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& os, Vector const& v) {
    return os << std::setprecision(std::numeric_limits<double>::max_digits10) << "{" << v.x << ", "
              << v.y << ", " << v.z << "}";
}

Point operator+(Vector const& lhs, Point rhs) { return rhs += lhs; }

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

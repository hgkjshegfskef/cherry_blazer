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

double Vector::magnitude() const { return std::sqrt(x * x + y * y + z * z); }

Vector Vector::normalize() const {
    auto magnitude = this->magnitude();
    return {x / magnitude, y / magnitude, z / magnitude};
}

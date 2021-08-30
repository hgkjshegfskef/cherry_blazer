#include "vector.hh"
#include "point.hh"

// Vector += Vector (= Vector)
Vector& Vector::operator+=(const Vector& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vector& Vector::operator-=(const Vector& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vector::Vector(const Point& begin, const Point& end) {
    // Cannot initialize members declared by Tuple, because the Tuple constructor has already
    // initialized them. Thus, reassign:
    x = end.x - begin.x;
    y = end.y - begin.y;
    z = end.z - begin.z;
}

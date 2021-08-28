#include "vector.hh"

// Vector += Vector (= Vector)
Vector& Vector::operator+=(const Vector& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

// Vector + Vector = Vector
Vector operator+(Vector lhs, const Vector& rhs) {
    lhs += rhs;
    return lhs;
}

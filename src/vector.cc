#include "vector.hh"
#include "point.hh"

Vector::Vector(const Point& begin, const Point& end) {
    // Cannot initialize members declared by Tuple, because the Tuple constructor has already
    // initialized them. Thus, reassign:
    x = end.x - begin.x;
    y = end.y - begin.y;
    z = end.z - begin.z;
}

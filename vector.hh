#ifndef CHERRY_BLAZER__VECTOR_HH_
#define CHERRY_BLAZER__VECTOR_HH_

#include <type_traits>

#include "tuple.hh"

// Point and Vector are siblings. Due to circular inclusion problem one of them has to be
// forward-declared. Due to `Point += Vector (= Point)` being implemented in point.hh, which uses
// non-static Vector members, point.hh requires full Vector definition. Therefore, Point is
// forward-declared here, and not the other way around.
struct Point;

struct Vector : Tuple {
    Vector() = default;
    using Tuple::Tuple;

    // Vector::operator+=(Vector const&) is inherited from Tuple and works due to Liskov
    // substitution principle. However, in order to disallow Vector += Point (= ERROR), the
    // operator+=(Point const&) has to be deleted. In which case operator+=(Vector const&) stops
    // working. Therefore, it has to be explicitly implemented:
    Vector& operator+=(Vector const& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    // Vector + Vector = Vector
    friend Vector operator+(Vector lhs, Vector const& rhs) {
        lhs += rhs;
        return lhs;
    }

    // Point-related operations:

    // Vector += Point (= ERROR)
    Vector& operator+=(Point const& rhs) = delete;

    // Vector + Point is defined in point.hh
};
static_assert(std::is_trivially_copyable_v<Vector>);

#endif // CHERRY_BLAZER__VECTOR_HH_

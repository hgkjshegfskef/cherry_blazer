#ifndef CHERRY_BLAZER__VECTOR_HH_
#define CHERRY_BLAZER__VECTOR_HH_

#include <type_traits>

#include "tuple.hh"

struct Vector : Tuple {
    Vector() = default;
    using Tuple::Tuple;

    // vector += vector (= vector)
    Vector& operator+=(Vector const& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    // vector + vector = vector
    friend Vector operator+(Vector lhs, Vector const& rhs) {
        lhs += rhs;
        return lhs;
    }
};
static_assert(std::is_trivially_copyable_v<Vector>);

#endif // CHERRY_BLAZER__VECTOR_HH_

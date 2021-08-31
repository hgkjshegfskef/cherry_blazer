#ifndef CHERRY_BLAZER_SRC_VECTOR_HH_
#define CHERRY_BLAZER_SRC_VECTOR_HH_

#include "tuple.hh"

#include <type_traits>

// Point and Vector are siblings. Due to circular inclusion problem one of them has to be
// forward-declared. Due to `Point += Vector (= Point)` being implemented in point.hh, which uses
// non-static Vector members, point.hh requires full Vector definition. Therefore, Point is
// forward-declared here, and not the other way around.
struct Point;

struct Vector : Tuple {
    Vector() = default;
    Vector(Point const& begin, Point const& end);
    using Tuple::Tuple;

    // Point-related operations:

    // Vector += Point (= ERROR)
    Vector& operator+=(Point const& rhs) = delete;
    // Vector + Point is defined in point.hh

    // Vector -= Point (= ERROR)
    Vector& operator-=(Point const& rhs) = delete;
    // Vector - Point = ERROR
    Vector operator-(Point const& rhs) const = delete;

    // Vector cannot be compared to Point. Explicit deletion is required due to inherited operators
    // from Tuple.
    bool operator==(Point const& rhs) const = delete;
    bool operator!=(Point const& rhs) const = delete;

    [[nodiscard]] double magnitude() const;
    [[nodiscard]] Vector normalize() const;
};

static_assert(
    std::is_trivially_copyable_v<Vector>,
    "Being trivially copyable means that compiler can optimize a bit better when this struct is "
    "used. For example, it can memcpy it instead of calling copy constructor. Or it can pass the "
    "struct inside a register, instead of passing a pointer to the struct. Considering how often "
    "this struct will be used in the project, it is preferable to keep it trivially copyable.");

#endif // CHERRY_BLAZER_SRC_VECTOR_HH_

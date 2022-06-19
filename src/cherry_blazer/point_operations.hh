#pragma once

#include "point.hh"
#include "vector.hh"

#include <boost/assert.hpp>

#include <cstddef>

namespace cherry_blazer {

// TODO: fix compile-time tests

// -Point (= ERROR)

// Point*scalar (= ERROR)

// scalar*Point (= ERROR)

// Point/scalar (= ERROR)

// scalar/Point (= ERROR)

// Point += Point (= ERROR)

// Point + Point (= ERROR)

// Point -= Point (= ERROR)

// Point - Point = Vector (pointing from rhs to lhs)
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator-(Point<Precision, Dimension> const& lhs,
                                       Point<Precision, Dimension> const& rhs) noexcept {
    Vector<Precision, Dimension> result;

    for (std::size_t i = 0; i < Dimension; ++i) {
        result[i] = lhs[i] - rhs[i];
    }

    // Last coordinate of the Vector is 0.
    result[Dimension] = static_cast<Precision>(0);

    return result;
}

// Vector-related operations:

// Point += Vector
template <typename Precision, std::size_t Dimension>
constexpr auto& operator+=(Point<Precision, Dimension>& lhs,
                           Vector<Precision, Dimension> const& rhs) noexcept {
    // Implemented in Vector and here, to omit unnecessary inclusion of whole vector_operations.hh
    for (std::size_t i = 0; i < Dimension; ++i) {
        lhs[i] += rhs[i];
    }

    return lhs;
}

// Point + Vector = Point
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator+(Point<Precision, Dimension> lhs,
                                       Vector<Precision, Dimension> const& rhs) noexcept {
    return lhs += rhs;
}

// Point -= Vector
template <typename Precision, std::size_t Dimension>
constexpr auto& operator-=(Point<Precision, Dimension>& lhs,
                           Vector<Precision, Dimension> const& rhs) noexcept {
    for (std::size_t i = 0; i < Dimension; ++i) {
        lhs[i] -= rhs[i];
    }

    return lhs;
}

// Point - Vector = Point (move backward by vector)
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator-(Point<Precision, Dimension> lhs,
                                       Vector<Precision, Dimension> const& rhs) noexcept {
    return lhs -= rhs;
}

} // namespace cherry_blazer

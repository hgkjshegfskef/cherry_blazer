#pragma once
/// \file

#include "point.hh"
#include "vector.hh"

#include <cmath>
#include <iostream>

namespace cherry_blazer {

/// \brief -Vector.
/// Unary minus to negate the Vector.
/// \return Vector.
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator-(Vector<Precision, Dimension> const& v) noexcept {
    Vector<Precision, Dimension> result;

    for (std::size_t i = 0; i < Dimension; ++i) {
        result[i] = -v[i];
    }

    // Last coordinate of the Vector is 0.
    result[Dimension] = static_cast<Precision>(0);

    return result;
}

/// \brief Vector * scalar.
/// Multiply Vector by a scalar.
/// \return Vector.
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator*(Vector<Precision, Dimension> const& v,
                                       Precision scalar) noexcept {
    Vector<Precision, Dimension> result;

    for (std::size_t i = 0; i < Dimension; ++i) {
        result[i] = v[i] * scalar;
    }

    // Last coordinate of the Vector is 0.
    result[Dimension] = static_cast<Precision>(0);

    return result;
}

/// \brief scalar * Vector.
/// Multiply Vector by a scalar.
/// \return Vector.
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator*(Precision scalar,
                                       Vector<Precision, Dimension> const& v) noexcept {
    return v * scalar;
}

/// \brief Vector / scalar.
/// Divide Vector by a scalar.
/// \return Vector.
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator/(Vector<Precision, Dimension> const& v,
                                       Precision scalar) noexcept {
    Vector<Precision, Dimension> result;

    for (std::size_t i = 0; i < Dimension; ++i) {
        result[i] = v[i] / scalar;
    }

    // Last coordinate of the Vector is 0.
    result[Dimension] = static_cast<Precision>(0);

    return result;
}

// scalar/Vector (= ERROR)

/// \brief Vector += Vector (= Vector).
/// Add Vector to Vector.
/// \return Reference to Vector.
template <typename Precision, std::size_t Dimension>
constexpr auto& operator+=(Vector<Precision, Dimension>& lhs,
                           Vector<Precision, Dimension> const& rhs) noexcept {
    for (std::size_t i = 0; i < Dimension; ++i) {
        lhs[i] += rhs[i];
    }

    return lhs;
}

/// \brief Vector -= Vector (= Vector).
/// Subtract Vector from Vector.
/// \return Reference to Vector.
template <typename Precision, std::size_t Dimension>
constexpr auto& operator-=(Vector<Precision, Dimension>& lhs,
                           Vector<Precision, Dimension> const& rhs) noexcept {
    for (std::size_t i = 0; i < Dimension; ++i) {
        lhs[i] -= rhs[i];
    }

    return lhs;
}

/// \brief Vector + Vector = Vector.
/// Add Vector to Vector.
/// \return Vector.
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator+(Vector<Precision, Dimension> lhs,
                                       Vector<Precision, Dimension> const& rhs) noexcept {
    return lhs += rhs;
}

/// \brief Vector - Vector = Vector.
/// Subtract Vector from Vector.
/// \return Vector.
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator-(Vector<Precision, Dimension> lhs,
                                       Vector<Precision, Dimension> const& rhs) noexcept {
    return lhs -= rhs;
}

// Point-related operations:

// Vector += Point (= ERROR)

/// \brief Vector + Point = Point.
/// Add Vector and Point.
/// \return Point.
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator+(Vector<Precision, Dimension> const& lhs,
                                       Point<Precision, Dimension> rhs) noexcept {
    // Implemented in Point and here, to omit unnecessary inclusion of whole point_operations.hh
    for (std::size_t i = 0; i < Dimension; ++i) {
        rhs[i] += lhs[i];
    }

    return rhs;
}

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Freestanding operations:
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto magnitude(Vector<Precision, Dimension> const& v) noexcept {
    Precision result{};

    for (std::size_t i = 0; i < Dimension; ++i) {
        result += v[i] * v[i];
    }

    return std::sqrt(result); // TODO: not constexpr
}

template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto normalize(Vector<Precision, Dimension> const& v) noexcept {
    Vector<Precision, Dimension> result;

    /*constexpr*/ auto const mag = magnitude(v);
    for (std::size_t i = 0; i < Dimension; ++i) {
        result[i] = v[i] / mag;
    }

    // Last coordinate of the Vector is 0.
    result[Dimension] = static_cast<Precision>(0);

    return result;
}

template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto dot(Vector<Precision, Dimension> const& v1,
                                 Vector<Precision, Dimension> const& v2) noexcept {
    Precision result{};

    for (std::size_t i = 0; i < Dimension; ++i) {
        result += v1[i] * v2[i];
    }

    return result;
}

template <typename Precision, std::size_t Dimension, typename = std::enable_if_t<Dimension == 3>>
[[nodiscard]] constexpr auto cross(Vector<Precision, Dimension> const& v1,
                                   Vector<Precision, Dimension> const& v2) noexcept {
    return Vector{v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2],
                  v1[0] * v2[1] - v1[1] * v2[0]};
}

} // namespace cherry_blazer

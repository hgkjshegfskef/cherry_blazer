#ifndef CHERRY_BLAZER_SRC_VECTOR_OPERATIONS_HH_
#define CHERRY_BLAZER_SRC_VECTOR_OPERATIONS_HH_

#include "point.hh"
#include "vector.hh"

#include <cmath>

namespace cherry_blazer {

// -Vector
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto operator-(Vector<T, D> const& v) noexcept {
    Vector<T, D> result;
    for (auto i{0U}; i < D; ++i)
        result[i] = -v[i];
    return result;
}

// Vector*scalar
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto operator*(Vector<T, D> const& v, T scalar) noexcept {
    Vector<T, D> result;
    for (auto i{0U}; i < D; ++i)
        result[i] = v[i] * scalar;
    return result;
}

// scalar*Vector
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto operator*(T scalar, Vector<T, D> const& v) noexcept {
    return v * scalar;
}

// Vector/scalar
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto operator/(Vector<T, D> const& v, T scalar) noexcept {
    Vector<T, D> result;
    for (auto i{0U}; i < D; ++i)
        result[i] = v[i] / scalar;
    return result;
}

// scalar/Vector (= ERROR)

// Vector += Vector (= Vector)
template <typename T, std::size_t D>
constexpr auto& operator+=(Vector<T, D>& lhs, Vector<T, D> const& rhs) noexcept {
    for (auto i{0U}; i < D; ++i)
        lhs[i] += rhs[i];
    return lhs;
}

// Vector -= Vector (= Vector)
template <typename T, std::size_t D>
constexpr auto& operator-=(Vector<T, D>& lhs, Vector<T, D> const& rhs) noexcept {
    for (auto i{0U}; i < D; ++i)
        lhs[i] -= rhs[i];
    return lhs;
}

// Vector + Vector = Vector
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto operator+(Vector<T, D> lhs, Vector<T, D> const& rhs) noexcept {
    return lhs += rhs;
}

// Vector - Vector = Vector
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto operator-(Vector<T, D> lhs, Vector<T, D> const& rhs) noexcept {
    return lhs -= rhs;
}

// Point-related operations:

// Vector += Point (= ERROR)

// Vector + Point = Point
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto operator+(Vector<T, D> const& lhs, Point<T, D> rhs) noexcept {
    // Implemented in Point and here, to omit unnecessary inclusion of whole point_operations.hh
    for (auto i{0U}; i < D; ++i)
        rhs[i] += lhs[i];
    return rhs;
}

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Freestanding operations:
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto magnitude(Vector<T, D> const& v) noexcept {
    T result{};
    for (auto i{0U}; i < D; ++i)
        result += v[i] * v[i];
    return std::sqrt(result);
}

template <typename T, std::size_t D>
[[nodiscard]] constexpr auto normalize(Vector<T, D> const& v) noexcept {
    Vector<T, D> result;
    auto mag = magnitude(v);
    for (auto i{0U}; i < D; ++i)
        result[i] = v[i] / mag;
    return result;
}

template <typename T, std::size_t D>
[[nodiscard]] constexpr auto dot(Vector<T, D> const& v1, Vector<T, D> const& v2) noexcept {
    T result{};
    for (auto i{0U}; i < D; ++i)
        result += v1[i] * v2[i];
    return result;
}

template <typename T, std::size_t D>
[[nodiscard]] constexpr std::enable_if_t<D == 3, Vector<T, D>>
cross(Vector<T, D> const& v1, Vector<T, D> const& v2) noexcept {
    return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR_OPERATIONS_HH_

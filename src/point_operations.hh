#ifndef CHERRY_BLAZER_SRC_POINT_OPERATIONS_HH_
#define CHERRY_BLAZER_SRC_POINT_OPERATIONS_HH_

#include "point.hh"
#include "vector.hh"

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
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto operator-(Point<T, D> const& lhs, Point<T, D> const& rhs) noexcept {
    Vector<T, D> result;
    for (auto i{0U}; i < D; ++i)
        result[i] = lhs[i] - rhs[i];
    return result;
}

// Vector-related operations:

// Point += Vector
template <typename T, std::size_t D>
constexpr auto& operator+=(Point<T, D>& lhs, Vector<T, D> const& rhs) noexcept {
    for (auto i{0U}; i < D; ++i)
        lhs[i] += rhs[i];
    return lhs;
}

// Point + Vector = Point
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto operator+(Point<T, D> lhs, Vector<T, D> const& rhs) noexcept {
    return lhs += rhs;
}

// Point -= Vector
template <typename T, std::size_t D>
constexpr auto& operator-=(Point<T, D>& lhs, Vector<T, D> const& rhs) noexcept {
    for (auto i{0U}; i < D; ++i)
        lhs[i] -= rhs[i];
    return lhs;
}

// Point - Vector = Point (move backward by vector)
template <typename T, std::size_t D>
[[nodiscard]] constexpr auto operator-(Point<T, D> lhs, Vector<T, D> const& rhs) noexcept {
    return lhs -= rhs;
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT_OPERATIONS_HH_

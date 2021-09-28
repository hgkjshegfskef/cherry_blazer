#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_POINT_OPERATIONS_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_POINT_OPERATIONS_HH_

#include "point.hh"

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

    for (auto i{0U}; i < Dimension; ++i)
        result[i] = lhs[i] - rhs[i];

    result[Dimension] = static_cast<Precision>(1);

    BOOST_VERIFY(result[Dimension] == static_cast<Precision>(1));
    return result;
}

// Vector-related operations:

// Point += Vector
template <typename Precision, std::size_t Dimension>
constexpr auto& operator+=(Point<Precision, Dimension>& lhs,
                           Vector<Precision, Dimension> const& rhs) noexcept {
    // Implemented in Vector and here, to omit unnecessary inclusion of whole vector_operations.hh
    for (auto i{0U}; i < Dimension; ++i)
        lhs[i] += rhs[i];

    BOOST_VERIFY(lhs[Dimension] == static_cast<Precision>(1));
    return lhs;
}

// Point + Vector = Point
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator+(Point<Precision, Dimension> lhs,
                                       Vector<Precision, Dimension> const& rhs) noexcept {
    lhs += rhs;

    BOOST_VERIFY(lhs[Dimension] == static_cast<Precision>(1));
    return lhs;
}

// Point -= Vector
template <typename Precision, std::size_t Dimension>
constexpr auto& operator-=(Point<Precision, Dimension>& lhs,
                           Vector<Precision, Dimension> const& rhs) noexcept {
    for (auto i{0U}; i < Dimension; ++i)
        lhs[i] -= rhs[i];

    BOOST_VERIFY(lhs[Dimension] == static_cast<Precision>(1));
    return lhs;
}

// Point - Vector = Point (move backward by vector)
template <typename Precision, std::size_t Dimension>
[[nodiscard]] constexpr auto operator-(Point<Precision, Dimension> lhs,
                                       Vector<Precision, Dimension> const& rhs) noexcept {
    lhs -= rhs;

    BOOST_VERIFY(lhs[Dimension] == static_cast<Precision>(1));
    return lhs;
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_POINT_OPERATIONS_HH_

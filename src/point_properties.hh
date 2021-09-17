#ifndef CHERRY_BLAZER_SRC_POINT_PROPERTIES_HH_
#define CHERRY_BLAZER_SRC_POINT_PROPERTIES_HH_

#include "point.hh"
#include "util.hh"

#include <cstddef>
#include <ostream>

namespace cherry_blazer {

// TODO: this is essentially a copy-paste of vector_properties

// Basic properties such as comparison operators are defined here, so that it is not required to
// carry the whole set of templates of overload operators.

// Points can be compared for equality.
template <typename T, std::size_t D>
constexpr auto operator==(Point<T, D> const& lhs, Point<T, D> const& rhs) noexcept {
    for (auto i{0U}; i < D; ++i) {
        if (!almost_equal(lhs[i], rhs[i]))
            return false;
    }
    return true;
}

// Points can be compared for inequality.
template <typename T, std::size_t D>
constexpr auto operator!=(Point<T, D> const& lhs, Point<T, D> const& rhs) noexcept {
    return !(lhs == rhs);
}

template <typename T, std::size_t D>
constexpr auto& operator<<(std::ostream& os, Point<T, D> const& p) noexcept {
    os << "{";
    for (auto i{0U}; i < D - 1; ++i)
        os << p[i] << ", ";
    return os << p[D - 1] << "}";
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT_PROPERTIES_HH_

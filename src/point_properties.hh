#ifndef CHERRY_BLAZER_SRC_POINT_PROPERTIES_HH_
#define CHERRY_BLAZER_SRC_POINT_PROPERTIES_HH_

#include "point.hh"
#include "util.hh"

#include <boost/pfr/core.hpp>
#include <ostream>
#include <utility>

namespace cherry_blazer {

// Basic properties such as comparison operators are defined here, so that it is not required to
// carry the whole set of templates of overload operators.

// Points can be compared for equality.
template <std::size_t D>
constexpr bool operator==(Point<D> const& lhs, Point<D> const& rhs) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>) {
        // floating-point comparison through epsilon
        return (almost_equal(boost::pfr::get<Nth>(lhs), boost::pfr::get<Nth>(rhs)) && ...);
    }
    (std::make_index_sequence<D>{});
}

// Points can be compared for inequality.
template <std::size_t D>
constexpr bool operator!=(Point<D> const& lhs, Point<D> const& rhs) noexcept {
    return !(lhs == rhs);
}

template <std::size_t D>
constexpr std::ostream& operator<<(std::ostream& os, Point<D> const& p) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>)->decltype(auto) {
        os << "{";
        ((os << (Nth == 0 ? "" : ", ") << boost::pfr::get<Nth>(p)), ...);
        return os << "}";
    }
    (std::make_index_sequence<D>{});
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT_PROPERTIES_HH_

#ifndef CHERRY_BLAZER_SRC_VECTOR_PROPERTIES_HH_
#define CHERRY_BLAZER_SRC_VECTOR_PROPERTIES_HH_

#include "util.hh"
#include "vector.hh"

#include <boost/pfr/core.hpp>
#include <ostream>
#include <utility>

namespace cherry_blazer {

// Basic properties such as comparison operators are defined here, so that it is not required to
// carry the whole set of templates of overload operators.

// Vectors can be compared for equality.
template <std::size_t D>
constexpr bool operator==(Vector<D> const& lhs, Vector<D> const& rhs) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>) {
        // floating-point comparison through epsilon
        return (almost_equal(boost::pfr::get<Nth>(lhs), boost::pfr::get<Nth>(rhs)) && ...);
    }
    (std::make_index_sequence<D>{});
}

// Vectors can be compared for inequality.
template <std::size_t D>
constexpr bool operator!=(Vector<D> const& lhs, Vector<D> const& rhs) noexcept {
    return !(lhs == rhs);
}

template <std::size_t D>
constexpr std::ostream& operator<<(std::ostream& os, Vector<D> const& v) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>)->decltype(auto) {
        os << "{";
        ((os << (Nth == 0 ? "" : ", ") << boost::pfr::get<Nth>(v)), ...);
        return os << "}";
    }
    (std::make_index_sequence<D>{});
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR_PROPERTIES_HH_

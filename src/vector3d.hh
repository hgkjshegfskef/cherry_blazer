#ifndef CHERRY_BLAZER_SRC_VECTOR3D_HH_
#define CHERRY_BLAZER_SRC_VECTOR3D_HH_

#include "util.hh"
#include "vector.hh"

#include <boost/pfr/core.hpp>
#include <ostream>
#include <type_traits>
#include <utility>

namespace cherry_blazer {

template <> struct Vector<3> {
    double x;
    double y;
    double z;
};

using Vector3d = Vector<3>; // NOLINT(readability-identifier-naming)

static_assert(std::is_aggregate_v<Vector3d>, "Must be an aggregate for Boost.PFR to work.");

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

#endif // CHERRY_BLAZER_SRC_VECTOR3D_HH_

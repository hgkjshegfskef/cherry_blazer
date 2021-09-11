#ifndef CHERRY_BLAZER_SRC_POINT_OPERATIONS_HH_
#define CHERRY_BLAZER_SRC_POINT_OPERATIONS_HH_

#include "util.hh"
#include "vector.hh"

#include <boost/pfr/core.hpp>
#include <boost/pfr/tuple_size.hpp>
#include <cstddef>
#include <ostream>
#include <utility>

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

// Throughout this file we use lambda with a template parameter list taking an index sequence and
// doing a fold expression over `operator,' to implement operations on N-sized types.

// Point - Point = Vector (pointing from rhs to lhs)
template <std::size_t D>
[[nodiscard]] constexpr Vector<D> operator-(Point<D> const& lhs, Point<D> const& rhs) noexcept {
    Vector<D> v;
    [&]<std::size_t... Nth>(std::index_sequence<Nth...>) {
        ((boost::pfr::get<Nth>(v) = boost::pfr::get<Nth>(lhs) - boost::pfr::get<Nth>(rhs)), ...);
    }
    (std::make_index_sequence<D>{});
    return v;
}

// Vector-related operations:

// Point += Vector
template <std::size_t D>
constexpr Point<D>& operator+=(Point<D>& lhs, Vector<D> const& rhs) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>)->decltype(auto) {
        ((boost::pfr::get<Nth>(lhs) += boost::pfr::get<Nth>(rhs)), ...);
        return lhs;
    }
    (std::make_index_sequence<D>{});
}

// Point + Vector = Point
template <std::size_t D>
[[nodiscard]] constexpr Point<D> operator+(Point<D> lhs, Vector<D> const& rhs) noexcept {
    return lhs += rhs;
}

// Point -= Vector
template <std::size_t D>
constexpr Point<D>& operator-=(Point<D>& lhs, Vector<D> const& rhs) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>)->decltype(auto) {
        ((boost::pfr::get<Nth>(lhs) -= boost::pfr::get<Nth>(rhs)), ...);
        return lhs;
    }
    (std::make_index_sequence<D>{});
}

// Point - Vector = Point (move backward by vector)
template <std::size_t D>
[[nodiscard]] constexpr Point<D> operator-(Point<D> lhs, Vector<D> const& rhs) noexcept {
    return lhs -= rhs;
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT_OPERATIONS_HH_

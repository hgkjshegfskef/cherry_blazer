#ifndef CHERRY_BLAZER_SRC_VECTOR_OPERATIONS_HH_
#define CHERRY_BLAZER_SRC_VECTOR_OPERATIONS_HH_

#include "point.hh"

#include <boost/pfr/core.hpp>
#include <cmath>
#include <ostream>

namespace cherry_blazer {

// -Vector
template <std::size_t D> [[nodiscard]] constexpr Vector<D> operator-(Vector<D> const& v) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>) {
        return Vector<D>{(-boost::pfr::get<Nth>(v))...};
    }
    (std::make_index_sequence<D>{});
}

// Vector*scalar
template <std::size_t D>
[[nodiscard]] constexpr Vector<D> operator*(Vector<D> const& v, double scalar) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>) {
        return Vector<D>{(boost::pfr::get<Nth>(v)) * scalar...};
    }
    (std::make_index_sequence<D>{});
}

// scalar*Vector
template <std::size_t D>
[[nodiscard]] constexpr Vector<D> operator*(double scalar, Vector<D> const& v) noexcept {
    return v * scalar;
}

// Vector/scalar
template <std::size_t D>
[[nodiscard]] constexpr Vector<D> operator/(Vector<D> const& v, double scalar) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>) {
        return Vector<D>{(boost::pfr::get<Nth>(v)) / scalar...};
    }
    (std::make_index_sequence<D>{});
}

// scalar/Vector (= ERROR)

// Vector += Vector (= Vector)
template <std::size_t D>
constexpr Vector<D>& operator+=(Vector<D>& lhs, Vector<D> const& rhs) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>)->decltype(auto) {
        ((boost::pfr::get<Nth>(lhs) += boost::pfr::get<Nth>(rhs)), ...);
        return lhs;
    }
    (std::make_index_sequence<D>{});
}

// Vector -= Vector (= Vector)
template <std::size_t D>
constexpr Vector<D>& operator-=(Vector<D>& lhs, Vector<D> const& rhs) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>)->decltype(auto) {
        ((boost::pfr::get<Nth>(lhs) -= boost::pfr::get<Nth>(rhs)), ...);
        return lhs;
    }
    (std::make_index_sequence<D>{});
}

// Vector + Vector = Vector
template <std::size_t D>
[[nodiscard]] constexpr Vector<D> operator+(Vector<D> lhs, Vector<D> const& rhs) noexcept {
    return lhs += rhs;
}

// Vector - Vector = Vector
template <std::size_t D>
[[nodiscard]] constexpr Vector<D> operator-(Vector<D> lhs, Vector<D> const& rhs) noexcept {
    return lhs -= rhs;
}

// Point-related operations:

// Vector += Point (= ERROR)

// Vector + Point = Point
template <std::size_t D>
[[nodiscard]] constexpr Point<D> operator+(Vector<D> const& lhs, Point<D> rhs) noexcept {
    // Implemented in Point and here, to omit unnecessary inclusion of whole point_operations.hh
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>) {
        ((boost::pfr::get<Nth>(rhs) += boost::pfr::get<Nth>(lhs)), ...);
        return rhs;
    }
    (std::make_index_sequence<D>{});
}

// Vector -= Point (= ERROR)

// Vector - Point (= ERROR)

// Freestanding operations:
template <std::size_t D> [[nodiscard]] constexpr double magnitude(Vector<D> const& v) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>) {
        return std::sqrt(((boost::pfr::get<Nth>(v) * boost::pfr::get<Nth>(v)) + ...));
    }
    (std::make_index_sequence<D>{});
}

template <std::size_t D> [[nodiscard]] constexpr Vector<D> normalize(Vector<D> const& v) noexcept {
    auto mag = magnitude(v);
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>) {
        return Vector<D>{(boost::pfr::get<Nth>(v) / mag)...};
    }
    (std::make_index_sequence<D>{});
}

template <std::size_t D>
[[nodiscard]] constexpr std::enable_if_t<D == 3, double> dot(Vector<D> const& v1,
                                                             Vector<D> const& v2) noexcept {
    return [&]<std::size_t... Nth>(std::index_sequence<Nth...>) {
        return ((boost::pfr::get<Nth>(v1) * boost::pfr::get<Nth>(v2)) + ...);
    }
    (std::make_index_sequence<D>{});
}

template <std::size_t D>
[[nodiscard]] constexpr std::enable_if_t<D == 3, Vector<D>> cross(Vector<D> const& v1,
                                                                  Vector<D> const& v2) noexcept {
    return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR_OPERATIONS_HH_

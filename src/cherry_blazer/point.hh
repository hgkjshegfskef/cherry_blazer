#pragma once

#include "vector.hh"

#include <boost/assert.hpp>

#include <cstddef>
#include <ostream>

namespace cherry_blazer {

template <typename Precision, std::size_t Dimension> class Point {

  private:
    Vector<Precision, Dimension> vec_;

    using underlying_type = decltype(vec_);

  public:
    static inline constexpr std::size_t size = Dimension;

    using value_type = Precision;
    using reference = typename underlying_type::reference;
    using const_reference = typename underlying_type::const_reference;

    Point() = default;

    template <typename... PointComponents,
              typename = std::enable_if_t<
                  // Do not shadow Point's copy and move ctors.
                  not(sizeof...(PointComponents) == 1 and
                      std::is_same_v<std::common_type_t<PointComponents...>,
                                     std::decay_t<Point<Precision, Dimension>>>)>>
    constexpr explicit Point(PointComponents&&... components) noexcept
        : vec_{std::forward<PointComponents>(components)...} {
        // Last coordinate of the Point is 1.
        vec_[Dimension] = static_cast<Precision>(1);
    }

    constexpr typename underlying_type::reference at(typename underlying_type::size_type pos) {
        return vec_.at(pos);
    }
    [[nodiscard]] constexpr typename underlying_type::const_reference
    at(typename underlying_type::size_type pos) const {
        return vec_.at(pos);
    }

    constexpr typename underlying_type::reference
    operator[](typename underlying_type::size_type pos) noexcept {
        // TODO: replace with https://github.com/gpakosz/PPK_ASSERT
        BOOST_VERIFY(pos < Dimension + 1);
        return vec_[pos];
    }
    [[nodiscard]] constexpr typename underlying_type::const_reference
    operator[](typename underlying_type::size_type pos) const noexcept {
        BOOST_VERIFY(pos < Dimension + 1);
        return vec_[pos];
    }

    template <typename underlying_type::size_type Idx>
    constexpr typename underlying_type::reference get() noexcept {
        static_assert(0 <= Idx && Idx < Dimension, "Point index is out of bounds.");
        return vec_.template get<Idx>();
    }
    template <typename underlying_type::size_type Idx>
    [[nodiscard]] constexpr typename underlying_type::const_reference get() const noexcept {
        static_assert(0 <= Idx && Idx < Dimension, "Point index is out of bounds.");
        return vec_.template get<Idx>();
    }

    friend constexpr auto operator==(Point const& lhs, Point const& rhs) noexcept {
        return lhs.vec_ == rhs.vec_;
    }

    friend constexpr auto operator!=(Point const& lhs, Point const& rhs) noexcept {
        return !(lhs == rhs);
    }

    friend constexpr auto& operator<<(std::ostream& os, Point const& p) noexcept {
        return os << p.vec_;
    }
};

template <std::size_t Idx, typename Precision, std::size_t Dimension>
constexpr typename Point<Precision, Dimension>::reference
get(Point<Precision, Dimension>& point) noexcept {
    return point.template get<Idx>();
}

template <std::size_t Idx, typename Precision, std::size_t Dimension>
constexpr typename Point<Precision, Dimension>::const_reference
get(Point<Precision, Dimension> const& point) noexcept {
    return point.template get<Idx>();
}

template <typename... VectorComponents>
Point(VectorComponents...)
    -> Point<typename std::common_type_t<VectorComponents...>, sizeof...(VectorComponents)>;

using Point2f = Point<float, 2>;  // NOLINT(readability-identifier-naming)
using Point3f = Point<float, 3>;  // NOLINT(readability-identifier-naming)
using Point2d = Point<double, 2>; // NOLINT(readability-identifier-naming)
using Point3d = Point<double, 3>; // NOLINT(readability-identifier-naming)

extern template class Point<float, 3>;
extern template class Point<double, 3>;

} // namespace cherry_blazer

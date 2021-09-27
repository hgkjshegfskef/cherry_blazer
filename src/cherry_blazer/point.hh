#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_POINT_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_POINT_HH_

#include "vector.hh"

#include <boost/assert.hpp>

#include <cstddef>

namespace cherry_blazer {

template <typename Precision, std::size_t Dimension> class Point {

  private:
    Vector<Precision, Dimension> vec_;

    using underlying_type = decltype(vec_);

  public:
    static inline constexpr std::size_t size = Dimension;

    using value_type = Precision;

    Point() = default;

    template <typename... VectorComponents>
    constexpr Point(VectorComponents... components) : vec_{components...} {}

    constexpr typename underlying_type::reference at(typename underlying_type::size_type pos) {
        return vec_.at(pos);
    }
    [[nodiscard]] constexpr typename underlying_type::reference
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

template <typename... VectorComponents>
Point(VectorComponents...)
    -> Point<typename std::common_type_t<VectorComponents...>, sizeof...(VectorComponents)>;

using Point2f = Point<float, 2>;  // NOLINT(readability-identifier-naming)
using Point3f = Point<float, 3>;  // NOLINT(readability-identifier-naming)
using Point2d = Point<double, 2>; // NOLINT(readability-identifier-naming)
using Point3d = Point<double, 3>; // NOLINT(readability-identifier-naming)

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_POINT_HH_

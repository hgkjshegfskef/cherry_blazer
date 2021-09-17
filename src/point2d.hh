#ifndef CHERRY_BLAZER_SRC_POINT2D_HH_
#define CHERRY_BLAZER_SRC_POINT2D_HH_

#include "point.hh"

#include <exception>
#include <type_traits>

namespace cherry_blazer {

template <typename T> struct Point<T, 2> {
    static_assert(std::is_floating_point_v<T>);

    T x, y;

    constexpr T& operator[](size_t index) noexcept {
        switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        default:
            std::terminate();
        }
    }

    constexpr T operator[](size_t index) const noexcept {
        switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        default:
            std::terminate();
        }
    }
};

using Point2f = Point<float, 2>;  // NOLINT(readability-identifier-naming)
using Point2d = Point<double, 2>; // NOLINT(readability-identifier-naming)

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT2D_HH_

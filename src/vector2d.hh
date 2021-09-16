#ifndef CHERRY_BLAZER_SRC_VECTOR2D_HH_
#define CHERRY_BLAZER_SRC_VECTOR2D_HH_

#include "vector.hh"

#include <array>
#include <exception>

namespace cherry_blazer {

template <typename T> struct Vector<T, 2> {
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

using Vec2f = Vector<float, 2>;  // NOLINT(readability-identifier-naming)
using Vec2d = Vector<double, 2>; // NOLINT(readability-identifier-naming)

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR2D_HH_

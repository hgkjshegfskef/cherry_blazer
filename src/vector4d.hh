#ifndef CHERRY_BLAZER_SRC_VECTOR4D_HH_
#define CHERRY_BLAZER_SRC_VECTOR4D_HH_

#include "vector.hh"

#include <exception>
#include <type_traits>

namespace cherry_blazer {

template <typename T> struct Vector<T, 4> {
    static_assert(std::is_floating_point_v<T>);

    T x, y, z, w;

    constexpr T& operator[](size_t index) noexcept {
        switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
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
        case 2:
            return z;
        case 3:
            return w;
        default:
            std::terminate();
        }
    }
};

using Vec4f = Vector<float, 4>;  // NOLINT(readability-identifier-naming)
using Vec4d = Vector<double, 4>; // NOLINT(readability-identifier-naming)

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR4D_HH_

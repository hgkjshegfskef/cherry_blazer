#ifndef CHERRY_BLAZER_SRC_VECTOR3D_HH_
#define CHERRY_BLAZER_SRC_VECTOR3D_HH_

#include "vector.hh"

#include <exception>
#include <type_traits>

namespace cherry_blazer {

template <typename T> struct Vector<T, 3> {
    static_assert(std::is_floating_point_v<T>);

    T x, y, z;

    constexpr T& operator[](size_t index) noexcept {
        switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
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
        default:
            std::terminate();
        }
    }
};

using Vec3f = Vector<float, 3>;  // NOLINT(readability-identifier-naming)
using Vec3d = Vector<double, 3>; // NOLINT(readability-identifier-naming)

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR3D_HH_

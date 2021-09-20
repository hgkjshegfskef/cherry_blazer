#ifndef CHERRY_BLAZER_SRC_VECTOR_HH_
#define CHERRY_BLAZER_SRC_VECTOR_HH_

#include <cstddef>
#include <exception>
#include <type_traits>

namespace cherry_blazer {

template <typename T, std::size_t D> struct Vector {
    static_assert(1 <= D && D <= 4, "Vector is only available for 1D-4D.");
};

template <typename... T> Vector(T...) -> Vector<typename std::common_type_t<T...>, sizeof...(T)>;

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

#endif // CHERRY_BLAZER_SRC_VECTOR_HH_

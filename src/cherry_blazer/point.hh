#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_POINT_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_POINT_HH_

#include <cstddef>
#include <exception>
#include <type_traits>

namespace cherry_blazer {

template <typename T, std::size_t D> struct Point {
    static_assert(1 <= D && D <= 4, "Point is only available for 1D-4D.");
};

template <typename... T> Point(T...) -> Point<typename std::common_type_t<T...>, sizeof...(T)>;

template <typename T> struct Point<T, 2> {
    static_assert(std::is_floating_point_v<T>);

    using value_type = T;
    static inline constexpr size_t size = 2;

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

template <typename T> struct Point<T, 3> {
    static_assert(std::is_floating_point_v<T>);

    using value_type = T;
    static inline constexpr size_t size = 3;

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

using Point3f = Point<float, 3>;  // NOLINT(readability-identifier-naming)
using Point3d = Point<double, 3>; // NOLINT(readability-identifier-naming)

template <typename T> struct Point<T, 4> {
    static_assert(std::is_floating_point_v<T>);

    using value_type = T;
    static inline constexpr size_t size = 4;

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

using Point4f = Point<float, 4>;  // NOLINT(readability-identifier-naming)
using Point4d = Point<double, 4>; // NOLINT(readability-identifier-naming)

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_POINT_HH_

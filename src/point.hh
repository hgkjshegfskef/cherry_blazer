#ifndef CHERRY_BLAZER_SRC_POINT_HH_
#define CHERRY_BLAZER_SRC_POINT_HH_

#include <cstddef>

namespace cherry_blazer {

template <std::size_t D> struct Point {
    static_assert(D == 2 || D == 3, "Only 2D and 3D dimensions are supported.");
};

template <typename... T> Point(T...) -> Point<sizeof...(T)>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT_HH_

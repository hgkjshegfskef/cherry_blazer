#ifndef CHERRY_BLAZER_SRC_POINT_HH_
#define CHERRY_BLAZER_SRC_POINT_HH_

#include <cstddef>
#include <type_traits>

namespace cherry_blazer {

template <typename T, std::size_t D> struct Point;

template <typename... T> Point(T...) -> Point<typename std::common_type_t<T...>, sizeof...(T)>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT_HH_

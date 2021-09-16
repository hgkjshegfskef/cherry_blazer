#ifndef CHERRY_BLAZER_SRC_VECTOR_HH_
#define CHERRY_BLAZER_SRC_VECTOR_HH_

#include <array>
#include <cstddef>
#include <type_traits>

namespace cherry_blazer {

template <typename T, std::size_t D> struct Vector;

template <typename... T> Vector(T...) -> Vector<typename std::common_type_t<T...>, sizeof...(T)>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR_HH_

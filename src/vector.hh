#ifndef CHERRY_BLAZER_SRC_VECTOR_HH_
#define CHERRY_BLAZER_SRC_VECTOR_HH_

#include <cstddef>

namespace cherry_blazer {

template <std::size_t D> struct Vector {
    static_assert(D == 2 || D == 3, "Only 2D and 3D dimensions are supported.");
};

template <typename... T> Vector(T...) -> Vector<sizeof...(T)>;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR_HH_

#ifndef CHERRY_BLAZER_SRC_VECTOR2D_HH_
#define CHERRY_BLAZER_SRC_VECTOR2D_HH_

#include "vector.hh"

#include <type_traits>

namespace cherry_blazer {

template <> struct Vector<2> {
    double x;
    double y;
};

using Vector2d = Vector<2>; // NOLINT(readability-identifier-naming)

static_assert(std::is_aggregate_v<Vector2d>);

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR2D_HH_

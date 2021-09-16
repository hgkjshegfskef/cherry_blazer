#ifndef CHERRY_BLAZER_SRC_VECTOR4D_HH_
#define CHERRY_BLAZER_SRC_VECTOR4D_HH_

#include "vector.hh"

#include <type_traits>

namespace cherry_blazer {

template <> struct Vector<4> {
    double x;
    double y;
    double z;
    double w;
};

using Vector4d = Vector<4>; // NOLINT(readability-identifier-naming)

static_assert(std::is_aggregate_v<Vector4d>, "Must be an aggregate for Boost.PFR to work.");

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR4D_HH_

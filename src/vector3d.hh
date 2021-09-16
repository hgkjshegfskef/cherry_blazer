#ifndef CHERRY_BLAZER_SRC_VECTOR3D_HH_
#define CHERRY_BLAZER_SRC_VECTOR3D_HH_

#include "vector.hh"

#include <type_traits>

namespace cherry_blazer {

template <> struct Vector<3> {
    double x;
    double y;
    double z;
};

using Vector3d = Vector<3>; // NOLINT(readability-identifier-naming)

static_assert(std::is_aggregate_v<Vector3d>, "Must be an aggregate for Boost.PFR to work.");

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_VECTOR3D_HH_

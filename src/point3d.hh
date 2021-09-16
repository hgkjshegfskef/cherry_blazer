#ifndef CHERRY_BLAZER_SRC_POINT3D_HH_
#define CHERRY_BLAZER_SRC_POINT3D_HH_

#include "point.hh"

#include <type_traits>

namespace cherry_blazer {

template <> struct Point<3> {
    double x;
    double y;
    double z;
};

using Point3d = Point<3>; // NOLINT(readability-identifier-naming)

static_assert(std::is_aggregate_v<Point3d>, "Must be an aggregate for Boost.PFR to work.");

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT3D_HH_

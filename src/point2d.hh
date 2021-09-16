#ifndef CHERRY_BLAZER_SRC_POINT2D_HH_
#define CHERRY_BLAZER_SRC_POINT2D_HH_

#include "point.hh"

#include <type_traits>

namespace cherry_blazer {

template <> struct Point<2> {
    double x;
    double y;
};

using Point2d = Point<2>; // NOLINT(readability-identifier-naming)

static_assert(std::is_aggregate_v<Point2d>, "Must be an aggregate for Boost.PFR to work.");

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT2D_HH_

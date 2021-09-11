#ifndef CHERRY_BLAZER_SRC_POINT2D_HH_
#define CHERRY_BLAZER_SRC_POINT2D_HH_

#include "point.hh"

namespace cherry_blazer {

template <> struct Point<2> {
    double x;
    double y;
};

using Point2d = Point<2>; // NOLINT(readability-identifier-naming)

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_POINT2D_HH_

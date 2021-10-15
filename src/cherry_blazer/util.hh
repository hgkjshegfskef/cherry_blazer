#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_UTIL_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_UTIL_HH_

#include "point.hh"

namespace cherry_blazer::util {

// https://en.wikipedia.org/wiki/Linear_interpolation
// If the two known points are given by the coordinates (x0,y0) and (x1,y1), the linear interpolant
// is the straight line between these points. Value of x must be in the interval [x0;x1].
constexpr double lerp(double x, Point2d const& left_point, Point2d const& right_point);

} // namespace cherry_blazer::util

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_UTIL_HH_

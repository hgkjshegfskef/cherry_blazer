#pragma once

#include "point.hh"

namespace cherry_blazer::util {

// https://en.wikipedia.org/wiki/Linear_interpolation
// If the two known points are given by the coordinates (x0,y0) and (x1,y1), the linear interpolant
// is the straight line between these points. If the value of x is in the interval [x0;x1], the
// function interpolates, otherwise, the function extrapolates.
constexpr double lerp(double x, Point2d const& left_point, Point2d const& right_point) {
    return left_point[1] + (x - left_point[0]) * (right_point[1] - left_point[1]) /
                               (right_point[0] - left_point[0]);
}

} // namespace cherry_blazer::util

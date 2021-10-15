#include "util.hh"

#include "point_operations.hh"

#include <boost/assert.hpp>

namespace cherry_blazer::util {

constexpr double lerp(double x, Point2d const& left_point, Point2d const& right_point) {
    BOOST_VERIFY(left_point[0] <= x && x <= right_point[0]);
    return left_point[1] + (x - left_point[0]) * (right_point[1] - left_point[1]) /
                               (right_point[0] - left_point[0]);
}

} // namespace cherry_blazer::util

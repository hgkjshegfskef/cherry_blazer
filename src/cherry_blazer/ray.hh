#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_RAY_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_RAY_HH_

#include "point.hh"
#include "vector.hh"

namespace cherry_blazer {

struct Ray {
    Point3d origin;
    Vec3d direction;
};

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_RAY_HH_

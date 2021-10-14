#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_RAY_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_RAY_HH_

#include "intersection.hh"
#include "point.hh"
#include "transformation.hh"
#include "vector.hh"

#include <cstddef>

namespace cherry_blazer {

struct Ray {
    Point3d origin;
    Vec3d direction;

    Ray(Point3d const& origin, Vec3d const& direction) noexcept;

    [[nodiscard]] Point3d position(double time) const noexcept;
};

Ray transform(Ray const& ray, Transformation const& tform) noexcept;

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_RAY_HH_

#include "ray.hh"

#include "point_operations.hh"
#include "vector_operations.hh"

namespace cherry_blazer {

Ray::Ray(Point3d const& origin, Vec3d const& direction) : origin_{origin}, direction_{direction} {}

Point3d Ray::position(double time) const { return origin_ + direction_ * time; }

} // namespace cherry_blazer

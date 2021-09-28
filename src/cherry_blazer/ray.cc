#include "ray.hh"

#include "point_operations.hh"
#include "vector_operations.hh"

namespace cherry_blazer {

Ray::Ray(Point3d const& origin, Vec3d const& direction) : origin{origin}, direction{direction} {}

Point3d Ray::position(double time) const { return origin + direction * time; }

} // namespace cherry_blazer

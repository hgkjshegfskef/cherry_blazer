#include "ray.hh"

#include "matrix_operations.hh"
#include "point_operations.hh"
#include "vector_operations.hh"

#include <fmt/ostream.h>
#include <fmt/printf.h>

namespace cherry_blazer {

Ray::Ray(Point3d const& origin, Vec3d const& direction) noexcept
    : origin{origin}, direction{direction} {}

Point3d Ray::position(double time) const noexcept { return origin + direction * time; }

Ray transform(Ray const& ray, Transformation const& tform) noexcept {
    switch (tform.kind) {
    case Transformation::Kind::Identity:
        return {ray.origin, ray.direction};
    case Transformation::Kind::Translation:
        return {tform.mat * ray.origin, ray.direction};
    case Transformation::Kind::Scaling:
    case Transformation::Kind::Rotation:
    case Transformation::Kind::Shearing:
        return {tform.mat * ray.origin, tform.mat * ray.direction};
    }
    fmt::print(std::cerr, "ERROR: Unexpected enum: {}\n", tform.kind);
    return {};
}

} // namespace cherry_blazer

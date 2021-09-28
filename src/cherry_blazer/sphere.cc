#include "sphere.hh"

#include "optional_double.hh"
#include "point_operations.hh"
#include "util.hh"
#include "vector_operations.hh"

#include <cmath>
#include <utility>

namespace cherry_blazer {

Sphere::Sphere() : id_{++id_counter} {}

unsigned Sphere::id() const { return id_; }

// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
std::pair<optional_double, optional_double> Sphere::intersect(Ray const& ray) {
    // TODO?: sphere center is at world origin, radius = 1
    auto const sphere_center = Point{0., 0., 0.};
    auto const sphere_radius = 1.;

    // Create vector from the sphere center towards ray origin.
    auto const from_sphere_to_ray = ray.origin - sphere_center;

    // Make the ray direction a unit vector (enables better formula).
    auto const ray_direction = normalize(ray.direction);

    auto const b = dot(ray_direction, from_sphere_to_ray);
    auto const discriminant =
        b * b - (dot(from_sphere_to_ray, from_sphere_to_ray) - sphere_radius * sphere_radius);

    if (discriminant < 0.)
        return {};

    auto const sqrt_discriminant = std::sqrt(discriminant);

    if (almost_equal(discriminant, 0.)) {
        return {optional_double{-b - sqrt_discriminant}, {}};
    }

    return {optional_double{-b - sqrt_discriminant}, optional_double{-b + sqrt_discriminant}};
}

bool operator==(Sphere const& lhs, Sphere const& rhs) { return lhs.id() == rhs.id(); }

} // namespace cherry_blazer

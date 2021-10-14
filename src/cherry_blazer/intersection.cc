#include "intersection.hh"

#include "detail/util.hh"
#include "matrix_operations.hh"
#include "point_operations.hh"
#include "ray.hh"
#include "sphere.hh"
#include "vector_operations.hh"

#include <limits>

#if __cpp_lib_ranges >= 202106L
#include <ranges>
#endif

namespace cherry_blazer {

Intersection::Intersection(double t, Sphere const& object) : t{t}, object{object} {}

// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
std::vector<Intersection> intersect(Sphere const& sphere, Ray const& ray) {
    // TODO?: sphere center is at world origin, radius = 1
    auto const sphere_center = Point{0., 0., 0.};
    auto const sphere_radius = 1.;

    // Account for the transformations applied to sphere (so, apply the inverse of them to ray).
    auto transformed_ray = transform(
        ray, Transformation{inverse(sphere.transformation.mat), sphere.transformation.kind});

    transformed_ray.direction = normalize(transformed_ray.direction);

    // Create vector from the sphere center towards ray origin.
    auto const from_sphere_to_transformed_ray = transformed_ray.origin - sphere_center;

    // Make the ray direction a unit vector (enables better formula).
    auto const transformed_ray_direction = normalize(transformed_ray.direction);

    auto const b = dot(transformed_ray_direction, from_sphere_to_transformed_ray);
    auto const discriminant =
        b * b - (dot(from_sphere_to_transformed_ray, from_sphere_to_transformed_ray) -
                 sphere_radius * sphere_radius);

    if (discriminant < 0.)
        return {};

    auto const sqrt_discriminant = std::sqrt(discriminant);

    if (detail::almost_equal(discriminant, 0.)) {
        return {{-b - sqrt_discriminant, sphere}};
    }

    return {{-b - sqrt_discriminant, sphere}, {-b + sqrt_discriminant, sphere}};
}

Intersection const* hit(std::vector<Intersection> const& intersections) {
#if __cpp_lib_ranges >= 202106L
    auto nonnegative =
        std::views::filter(intersections, [](auto const& i) { return i.place >= 0.; });
    auto const smallest_nonnegative =
        std::min_element(std::cbegin(nonnegative), std::cend(nonnegative));
    if (smallest_nonnegative == std::cend(nonnegative))
        return nullptr;
    return &(*smallest_nonnegative);
#else
    Intersection const* first_intersection = nullptr;
    double smallest_nonnegative{std::numeric_limits<double>::max()};
    for (auto const& i : intersections) {
        if (i.t >= 0. && i.t < smallest_nonnegative) {
            smallest_nonnegative = i.t;
            first_intersection = &i;
        }
    }
    return first_intersection;
#endif
}

} // namespace cherry_blazer

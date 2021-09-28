#include "intersection.hh"

#include "point_operations.hh"
#include "ray.hh"
#include "sphere.hh"
#include "util.hh"
#include "vector_operations.hh"

#include <limits>

#if __cpp_lib_ranges >= 202106L
#include <ranges>
#endif

namespace cherry_blazer {

Intersection::Intersection(double place, std::shared_ptr<Sphere> object)
    : place{place}, object{std::move(object)} {}

// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
std::vector<Intersection> intersect(Sphere const& sphere, Ray const& ray) {
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
        return {{-b - sqrt_discriminant, std::make_shared<Sphere>(sphere)}};
    }

    return {{-b - sqrt_discriminant, std::make_shared<Sphere>(sphere)},
            {-b + sqrt_discriminant, std::make_shared<Sphere>(sphere)}};
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
        if (i.place >= 0. && i.place < smallest_nonnegative) {
            smallest_nonnegative = i.place;
            first_intersection = &i;
        }
    }
    return first_intersection;
#endif
}

} // namespace cherry_blazer

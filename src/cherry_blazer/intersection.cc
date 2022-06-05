#include "intersection.hh"

#include "config.hh"
#include "detail/detail.hh"
#include "matrix_operations.hh"
#include "point_operations.hh"
#include "ray.hh"
#include "sphere.hh"
#include "vector_operations.hh"

#if CHERRY_BLAZER_RANGES
#include <algorithm>
#include <ranges>
#else
#include <limits>
#endif

namespace cherry_blazer {

Intersection::Intersection(double t, Sphere const& object) : t{t}, object{object} {}

bool operator<(Intersection const& lhs, Intersection const& rhs) { return lhs.t < rhs.t; }

std::vector<Intersection> intersect(Sphere const& sphere, Ray const& ray) {
    // Account for the transformations applied to sphere (so, apply the inverse of them to ray).
    auto transformed_ray =
        transform(ray, {inverse(sphere.transformation.mat), sphere.transformation.kind});

    // Create vector from the sphere center towards ray origin.
    auto const from_sphere_to_transformed_ray = transformed_ray.origin - Point{0., 0., 0.};

    auto const a = dot(transformed_ray.direction, transformed_ray.direction);
    auto const b = 2. * dot(transformed_ray.direction, from_sphere_to_transformed_ray);
    auto const c = dot(from_sphere_to_transformed_ray, from_sphere_to_transformed_ray) - 1.;
    auto const discriminant = b * b - 4. * a * c;

    auto const two_a = 2. * a;
    if (detail::almost_equal(discriminant, 0.)) {
        return {{-b / two_a, sphere}};
    }

    if (discriminant < 0.) {
        return {};
    }

    auto const sqrt_discriminant = std::sqrt(discriminant);
    return {{(-b - sqrt_discriminant) / two_a, sphere}, {(-b + sqrt_discriminant) / two_a, sphere}};
}

Intersection const* hit(std::vector<Intersection> const& intersections) {
#if CHERRY_BLAZER_RANGES
    auto nonnegative = std::views::filter(intersections, [](auto const& i) { return i.t >= 0.; });
    auto const smallest_nonnegative = std::min_element(nonnegative.begin(), nonnegative.end());
    if (smallest_nonnegative == nonnegative.end())
        return nullptr;
    return &(*smallest_nonnegative);
#else
    Intersection const* first_intersection = nullptr;
    auto smallest_nonnegative{std::numeric_limits<double>::max()};
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

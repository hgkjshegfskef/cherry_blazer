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

std::vector<Intersection> intersect(Sphere const& sphere, Ray const& ray) {
    // Account for the transformations applied to sphere (so, apply the inverse of them to ray).
    auto transformed_ray = transform(
        ray, Transformation{inverse(sphere.transformation.mat), sphere.transformation.kind});

    transformed_ray.direction = normalize(transformed_ray.direction);

    // Create vector from the sphere center towards ray origin.
    auto const from_sphere_to_transformed_ray = transformed_ray.origin - Point{0., 0., 0.};

    auto const a = dot(transformed_ray.direction, transformed_ray.direction);
    auto const b = 2. * dot(transformed_ray.direction, from_sphere_to_transformed_ray);
    auto const c = dot(from_sphere_to_transformed_ray, from_sphere_to_transformed_ray) - 1.;
    auto const discriminant = b * b - 4. * a * c;

    if (discriminant < 0.)
        return {};

    auto const sqrt_discriminant = std::sqrt(discriminant);

    if (detail::almost_equal(discriminant, 0.)) {
        return {{(-b) / (2. * a), sphere}};
    }

    return {{(-b - sqrt_discriminant) / (2. * a), sphere},
            {(-b + sqrt_discriminant) / (2. * a), sphere}};
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

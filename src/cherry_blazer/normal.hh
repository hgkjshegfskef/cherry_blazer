#pragma once

#include "coord.hh"
#include "matrix_operations.hh"
#include "point.hh"
#include "point_operations.hh"
#include "sphere.hh"
#include "vector.hh"

namespace cherry_blazer {

template <typename Precision, std::size_t Dimension>
auto normal(Sphere const& sphere, Point<Precision, Dimension> const& at_world_point) {
    auto const inverted_transformation_matrix = inverse(sphere.transformation.mat);

    auto const object_point = inverted_transformation_matrix * at_world_point;
    auto const world_normal =
        transpose(inverted_transformation_matrix) * object_point - Point{0., 0., 0.};
    //    world_normal[Coord::W] = static_cast<Precision>(0);

    return normalize(world_normal);
}

} // namespace cherry_blazer

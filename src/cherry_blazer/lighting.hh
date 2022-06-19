#pragma once

#include "color.hh"
#include "material.hh"
#include "point.hh"
#include "point_light.hh"
#include "vector.hh"

namespace cherry_blazer {

Color lighting(Material const& material, PointLight const& light, Point3d const& point,
               Vec3d const& eye_vector, Vec3d const& normal_vector);

} // namespace cherry_blazer

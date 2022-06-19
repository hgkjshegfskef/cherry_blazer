#pragma once

#include "color.hh"
#include "point.hh"

namespace cherry_blazer {

struct PointLight {
    Point3d position;
    Color intensity;
};

} // namespace cherry_blazer

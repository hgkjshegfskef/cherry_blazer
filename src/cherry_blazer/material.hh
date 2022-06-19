#pragma once

#include "color.hh"

#include <tuple>

namespace cherry_blazer {

struct Material {
    Color color{1., 1., 1.};
    double ambient{.1};
    double diffuse{.9};
    double specular{.9};
    double shininess{200.};
};

bool operator==(Material const& lhs, Material const& rhs);

} // namespace cherry_blazer

#include "material.hh"

#include <exception>

namespace cherry_blazer {

bool operator==(Material const& lhs, Material const& rhs) {
    return std::tie(lhs.color, lhs.ambient, lhs.diffuse, lhs.specular, lhs.shininess) ==
           std::tie(rhs.color, rhs.ambient, rhs.diffuse, rhs.specular, rhs.shininess);
}

} // namespace cherry_blazer

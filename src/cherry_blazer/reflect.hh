#pragma once

#include "vector.hh"
#include "vector_operations.hh"

namespace cherry_blazer {

template <typename Precision, std::size_t Dimension>
auto reflect(Vector<Precision, Dimension> const& in, Vector<Precision, Dimension> const& normal) {
    return in - normal * 2. * dot(in, normal);
}

} // namespace cherry_blazer

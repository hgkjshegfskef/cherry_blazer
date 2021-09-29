#include "sphere.hh"

namespace cherry_blazer {

Sphere::Sphere() : transformation{Matrix<double, 4, 4>::identity()}, id_{++id_counter} {}

Sphere::Sphere(Matrix<double, 4, 4> const& transformation)
    : transformation{transformation}, id_{++id_counter} {}

unsigned Sphere::id() const { return id_; }

bool operator==(Sphere const& lhs, Sphere const& rhs) { return lhs.id() == rhs.id(); }

} // namespace cherry_blazer

#include "transformation.hh"

namespace cherry_blazer {

Transformation::Transformation(Mat4d const& mat, Kind const& kind) : mat{mat}, kind{kind} {}

} // namespace cherry_blazer

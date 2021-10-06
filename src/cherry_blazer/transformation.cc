#include "transformation.hh"

namespace cherry_blazer {

Transformation::Transformation(std::shared_ptr<Mat4d> mat,
                               cherry_blazer::Transformation::Kind const& kind)
    : mat{std::move(mat)}, kind{kind} {}

} // namespace cherry_blazer

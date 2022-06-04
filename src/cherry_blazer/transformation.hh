#pragma once
#include "square_matrix.hh"

#include <memory>
#include <ostream>

namespace cherry_blazer {

class Transformation {
  public:
    Mat4d mat;
    enum class Kind { Identity, Translation, Scaling, Rotation, Shearing } kind;

    Transformation() : mat{Mat4d::identity()}, kind{Kind::Identity} {}
    Transformation(Mat4d const& mat, Kind const& kind);
};

std::ostream& operator<<(std::ostream& os, Transformation::Kind const& kind);

} // namespace cherry_blazer

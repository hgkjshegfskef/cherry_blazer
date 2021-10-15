#pragma once

#include "square_matrix.hh"

#include <memory>

namespace cherry_blazer {

class Transformation {
  public:
    Mat4d mat;
    enum class Kind { Identity, Translation, Scaling } kind;

    Transformation() : mat{Mat4d::identity()}, kind{Kind::Identity} {}
    Transformation(Mat4d const& mat, Kind const& kind);
};

} // namespace cherry_blazer

#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_TRANSFORMATION_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_TRANSFORMATION_HH_

#include "square_matrix.hh"

#include <memory>

namespace cherry_blazer {

class Transformation {
  public:
    std::shared_ptr<Mat4d> mat;
    enum class Kind { Identity, Translation, Scaling } kind;

    Transformation() = default;
    Transformation(std::shared_ptr<Mat4d> mat, Kind const& kind);
};

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_TRANSFORMATION_HH_

#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_RAY_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_RAY_HH_

#include "point.hh"
#include "vector.hh"

namespace cherry_blazer {

class Ray {
  public:
    Ray(Point3d const& origin, Vec3d const& direction);

    [[nodiscard]] Point3d position(double time) const;

  private:
    Point3d origin_;
    Vec3d direction_;
};

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_RAY_HH_

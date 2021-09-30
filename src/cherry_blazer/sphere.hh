#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_SPHERE_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_SPHERE_HH_

#include "square_matrix.hh"

namespace cherry_blazer {

// Sphere has radius 1 and is centered at the world origin (0,0,0).
class Sphere {
  public:
    Matrix<double, 4, 4> transformation;

    Sphere();
    explicit Sphere(Matrix<double, 4, 4> const& transformation);

    [[nodiscard]] unsigned id() const;

  private:
    unsigned id_;

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline constinit unsigned id_counter = 0;
};

bool operator==(Sphere const& lhs, Sphere const& rhs);

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_SPHERE_HH_

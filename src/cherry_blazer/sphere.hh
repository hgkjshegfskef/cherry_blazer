#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_SPHERE_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_SPHERE_HH_

#include "optional_double.hh"
#include "ray.hh"

#include <optional>
#include <utility>

namespace cherry_blazer {

// Sphere has radius 1 and is centered at the world origin (0,0,0).
class Sphere {
  public:
    Sphere();

    [[nodiscard]] unsigned id() const;

    [[nodiscard]] static std::pair<optional_double, optional_double> intersect(Ray const& ray);

  private:
    unsigned id_;

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline constinit unsigned id_counter = 0;
};

bool operator==(Sphere const& lhs, Sphere const& rhs);

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_SPHERE_HH_

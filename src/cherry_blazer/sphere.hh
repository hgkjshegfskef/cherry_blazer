#pragma once

#include "transformation.hh"

#include <memory>

namespace cherry_blazer {

// Sphere has radius 1 and is centered at the world origin (0,0,0).
class Sphere {
  public:
    Transformation transformation;

    Sphere() : transformation{}, id_{++id_counter} {}
    Sphere(Transformation const& transformation);

    [[nodiscard]] unsigned id() const;

  private:
    unsigned id_;

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline constinit unsigned id_counter = 0;
};

bool operator==(Sphere const& lhs, Sphere const& rhs);

} // namespace cherry_blazer

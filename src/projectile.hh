#ifndef CHERRY_BLAZER_SRC_PROJECTILE_HH_
#define CHERRY_BLAZER_SRC_PROJECTILE_HH_

#include "point.hh"
#include "vector.hh"

namespace cb = cherry_blazer;

struct Projectile {
    cb::Point3d position;
    cb::Vec3d velocity;
};

struct Environment {
    cb::Vec3d gravity;
    cb::Vec3d wind;
};

Projectile tick(Environment const& env, Projectile const& proj);

#endif // CHERRY_BLAZER_SRC_PROJECTILE_HH_

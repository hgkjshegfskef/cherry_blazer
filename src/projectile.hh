#ifndef CHERRY_BLAZER_SRC_PROJECTILE_HH_
#define CHERRY_BLAZER_SRC_PROJECTILE_HH_

#include "point.hh"
#include "vector.hh"

namespace cb = cherry_blazer;

struct Projectile {
    cb::Point position;
    cb::Vector velocity;
};

struct Environment {
    cb::Vector gravity;
    cb::Vector wind;
};

Projectile tick(Environment const& env, Projectile const& proj);

#endif // CHERRY_BLAZER_SRC_PROJECTILE_HH_

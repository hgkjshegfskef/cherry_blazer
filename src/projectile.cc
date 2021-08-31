#include "projectile.hh"

Projectile tick(Environment const& env, Projectile const& proj) {
    return {proj.position + proj.velocity, proj.velocity + env.gravity + env.wind};
}

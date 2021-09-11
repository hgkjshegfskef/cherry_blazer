#include "projectile.hh"

#include "point_operations.hh"
#include "vector_operations.hh"

Projectile tick(Environment const& env, Projectile const& proj) {
    return {proj.position + proj.velocity, proj.velocity + env.gravity + env.wind};
}

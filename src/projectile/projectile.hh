#pragma once

#include <cherry_blazer/point.hh>
#include <cherry_blazer/vector.hh>

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

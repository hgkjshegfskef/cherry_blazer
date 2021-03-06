#pragma once

#include "sphere.hh"

#include <utility>
#include <vector>

namespace cherry_blazer {

struct Ray;

struct Intersection {
    double t;
    Sphere object;

    Intersection(double t, Sphere const& object);
};

bool operator<(Intersection const& lhs, Intersection const& rhs);

std::vector<Intersection> intersect(Sphere const& sphere, Ray const& ray);

Intersection const* hit(std::vector<Intersection> const& intersections);

} // namespace cherry_blazer

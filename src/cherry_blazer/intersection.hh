#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_INTERSECTION_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_INTERSECTION_HH_

#include <memory>
#include <utility>
#include <vector>

namespace cherry_blazer {

class Sphere;
struct Ray;

struct Intersection {
    double place;
    std::shared_ptr<Sphere> object;

    Intersection(double place, std::shared_ptr<Sphere> object);
};

std::vector<Intersection> intersect(Sphere const& sphere, Ray const& ray);

Intersection const*  hit(std::vector<Intersection> const& intersections);

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_INTERSECTION_HH_

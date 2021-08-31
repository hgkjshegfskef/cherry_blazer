#include "point.hh"
#include "projectile.hh"
#include "vector.hh"

#include <iostream>

namespace cb = cherry_blazer;

int main() {
    // projectile starts one unit above the origin.
    // velocity is normalized to 1 unit/tick.
    Projectile proj{{0, 1, 0}, cb::normalize({1, 1, 0})};

    // gravity -0.1 unit/tick, and wind is -0.01 unit/tick.
    Environment env{{0, -0.1, 0}, {-0.01, 0, 0}}; // NOLINT(readability-magic-numbers)

    int counter{};
    Projectile p = tick(env, proj);
    std::cout << "[Tick #" << ++counter << "] "
              << "Projectile position: " << p.position << std::endl;

    do {
        p = tick(env, p);
        std::cout << "[Tick #" << ++counter << "] "
                  << "Projectile position: " << p.position << std::endl;
    } while (p.position.y > 0);

    std::cout << "Projectile hit the ground after " << counter << " ticks." << std::endl;
}

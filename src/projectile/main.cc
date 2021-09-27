#include "projectile.hh"

#include <cherry_blazer/canvas.hh>
#include <cherry_blazer/color.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/types.hh>
#include <cherry_blazer/vector.hh>
#include <cherry_blazer/vector_operations.hh>

#include <cerrno>
#include <iostream>
#include <string>
#include <system_error>

using cherry_blazer::Canvas;
using cherry_blazer::Color;
using cherry_blazer::Matrix;
using cherry_blazer::normalize;
using cherry_blazer::Point;
using cherry_blazer::Vector;

namespace {
std::ofstream open_file(std::string const& file) {
    std::ofstream f{file};
    if (!f)
        throw std::system_error(errno, std::system_category(), "failed to open '" + file + "'");
    return f;
}
} // namespace

int main() {
    // Projectile starts one unit above the origin. Velocity is normalized to 1 unit/tick.
#if __cpp_deduction_guides >= 201907
    Projectile proj{Point{0., 1., 0.}, normalize(Vector{1., 1.8, 0.}) * 11.25};
#else
    Projectile proj{Point{0., 1., 0.}, normalize(Matrix{1., 1.8, 0.}) * 11.25};
#endif

    // gravity -0.1 unit/tick, and wind is -0.01 unit/tick.
    // NOLINTNEXTLINE(readability-magic-numbers)
    Environment env{{0., -0.1, 0.}, {-0.01, 0., 0.}};

    Canvas canvas{900, 550}; // NOLINT(readability-magic-numbers)
    Color red{255, 0, 0};

    int counter{};
    Projectile p = tick(env, proj);
    std::cout << "[Tick #" << ++counter << "] "
              << "Projectile position: " << p.position << std::endl;
    do {
        // convert between canvas' and projectile's Y coordinates: projectile's Y coordinates go up,
        // while canvas' Y coordinates go down
        canvas(p.position[0], canvas.height() - p.position[1]) = red;
        p = tick(env, p);
        std::cout << "[Tick #" << ++counter << "] "
                  << "Projectile position: " << '\n'
                  << p.position << std::endl;
    } while (p.position[1] > 0);

    std::cout << "Projectile hit the ground after " << counter << " ticks." << std::endl;

    std::ofstream image_file;
    try {
        image_file = open_file("image.ppm");
    } catch (std::system_error const& e) {
        std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
        throw;
    }

    const std::string image_contents = canvas.as_ppm();
    image_file.write(image_contents.data(), i32(image_contents.size()));
}

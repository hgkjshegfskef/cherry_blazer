#include "projectile.hh"

#include <cherry_blazer/canvas.hh>
#include <cherry_blazer/color.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/point_properties.hh>
#include <cherry_blazer/types.hh>
#include <cherry_blazer/vector.hh>
#include <cherry_blazer/vector_operations.hh>

#include <cerrno>
#include <iostream>
#include <string>
#include <system_error>

namespace cb = cherry_blazer;

namespace {
std::ofstream open_file(std::string const& file) {
    std::ofstream f{file};
    if (!f)
        throw std::system_error(errno, std::system_category(), "failed to open '" + file + "'");
    return f;
}
} // namespace

int main() {
    // projectile starts one unit above the origin.
    // velocity is normalized to 1 unit/tick.
    // NOLINTNEXTLINE(readability-magic-numbers)
    Projectile proj{cb::Point{0., 1, 0}, cb::normalize(cb::Vector{1, 1.8, 0}) * 11.25};

    // gravity -0.1 unit/tick, and wind is -0.01 unit/tick.
    // NOLINTNEXTLINE(readability-magic-numbers)
    Environment env{cb::Vector{0, -0.1, 0}, cb::Vector{-0.01, 0, 0}};

    cb::Canvas canvas{900, 550}; // NOLINT(readability-magic-numbers)
    cb::Color red{255, 0, 0};

    int counter{};
    Projectile p = tick(env, proj);
    std::cout << "[Tick #" << ++counter << "] "
              << "Projectile position: " << p.position << std::endl;
    do {
        // convert between canvas' and projectile's Y coordinates: projectile's Y coordinates go up,
        // while canvas' Y coordinates go down
        canvas(u16(p.position.x), u16(double(u16(canvas.height())) - p.position.y)) = red;
        p = tick(env, p);
        std::cout << "[Tick #" << ++counter << "] "
                  << "Projectile position: " << p.position << std::endl;
    } while (p.position.y > 0);

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

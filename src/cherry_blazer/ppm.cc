#include "ppm.hh"

#include <cmath>
#include <sstream>
#include <string>

namespace cherry_blazer::ppm {

std::string generate_header(std::size_t width, std::size_t height,
                            std::size_t color_component_max) {
    std::stringstream ss;
    ss << "P3\n" << width << ' ' << height << '\n' << color_component_max << '\n';
    return ss.str();
}

} // namespace cherry_blazer::ppm

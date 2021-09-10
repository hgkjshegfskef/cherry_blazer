#include "ppm.hh"

#include <sstream>
#include <string>

namespace cherry_blazer::ppm {

std::string generate_header(safe_auto<u32> const& width, safe_auto<u32> const& height,
                            safe_auto<u32> const& color_component_max) {
    std::stringstream ss;
    ss << "P3\n" << width << ' ' << height << '\n' << color_component_max << '\n';
    return ss.str();
}

} // namespace cherry_blazer::ppm

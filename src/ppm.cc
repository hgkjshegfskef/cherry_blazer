#include <sstream>
#include <string>

namespace cherry_blazer::ppm {

std::string generate_header(std::size_t width, std::size_t height,
                            std::size_t max_color_component_value) {
    std::stringstream ss;
    ss << "P3\n"
       << std::to_string(width) << ' ' << std::to_string(height) << '\n'
       << std::to_string(max_color_component_value) << '\n';
    return ss.str();
}

} // namespace cherry_blazer::ppm

#include <cerrno>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <system_error>

const int canvas_width = 3;
const int canvas_height = 2;
const int ppm_max_val_per_color = 255;
char const* const image_file_name = "image.ppm";

auto ppm_generate_header(int width, int height) {
    std::stringstream ss = {};
    ss << "P3\n"
       << std::to_string(width) << ' ' << std::to_string(height) << '\n'
       << std::to_string(ppm_max_val_per_color) << '\n';
    return ss.str();
}

auto ppm_write_header(std::string const& file) {
    std::ofstream f(file);
    if (!f)
        throw std::system_error(errno, std::system_category(), "failed to open '" + file + "'");
    f << ppm_generate_header(canvas_width, canvas_height);
    return f;
}

int main() {
    std::ofstream image_file;
    try {
        image_file = ppm_write_header(image_file_name);
    } catch (std::system_error const& e) {
        std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
    }
    for (int i = 0; i < canvas_height; ++i) {
        for (int j = 0; j < canvas_width; ++j) {
            image_file << "0 0 255\n";
        }
    }
}
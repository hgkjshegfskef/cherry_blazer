#include <cerrno>
#include <fstream> // IWYU pragma: keep
#include <iostream>
#include <sstream> // IWYU pragma: keep
#include <string>
#include <system_error>

const int CANVAS_WIDTH = 3;
const int CANVAS_HEIGHT = 2;
const int PPM_MAX_VAL_PER_COLOR = 255;
char const* const IMAGE_FILE_NAME = "image.ppm";

auto ppm_generate_header(int width, int height) {
    std::stringstream ss = {};
    ss << "P3\n"
       << std::to_string(width) << ' ' << std::to_string(height) << '\n'
       << std::to_string(PPM_MAX_VAL_PER_COLOR) << '\n';
    return ss.str();
}

auto ppm_write_header(std::string const& file) {
    std::ofstream f(file);
    if (!f)
        throw std::system_error(errno, std::system_category(), "failed to open '" + file + "'");
    f << ppm_generate_header(CANVAS_WIDTH, CANVAS_HEIGHT);
    return f;
}

int main() {
    std::ofstream image_file;
    try {
        image_file = ppm_write_header(IMAGE_FILE_NAME);
    } catch (std::system_error const& e) {
        std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
    }
    for (int i = 0; i < CANVAS_HEIGHT; ++i) {
        for (int j = 0; j < CANVAS_WIDTH; ++j) {
            image_file << "0 0 255\n";
        }
    }
}

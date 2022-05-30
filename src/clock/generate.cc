#include <cherry_blazer/canvas.hh>
#include <cherry_blazer/color.hh>
#include <cherry_blazer/detail/types.hh>
#include <cherry_blazer/matrix_operations.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/square_matrix.hh>
#include <cherry_blazer/vector.hh>
#include <cherry_blazer/vector_operations.hh>

#include <fmt/core.h>

#include <cerrno>
#include <iostream>
#include <numbers>
#include <ostream>
#include <string>
#include <system_error>
#include <vector>

using namespace std::numbers;

using cherry_blazer::Axis;
using cherry_blazer::Canvas;
using cherry_blazer::Color;
using cherry_blazer::Matrix;
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
    Point clock_origin{0., 0., 0.};

    using T = decltype(clock_origin)::value_type; // NOLINT(readability-identifier-naming)
    constexpr auto D = decltype(clock_origin)::size + 1;
    constexpr auto radius = 20.;
    constexpr auto canvas_size = radius * 2.5;
    constexpr auto offset = canvas_size / 2;

    Canvas canvas{canvas_size + 1, canvas_size + 1};
    Color const black{0., 0., 0.};
    Color const white{255., 255., 255.};
    Color const green{0., 255., 0.};
    canvas.fill(white);

    auto const translation_matrix{Matrix<T, D, D>::translation(Matrix{0., -1., 0.})};
    auto const scaling_matrix{Matrix<T, D, D>::scaling(Matrix{radius, radius, radius})};

    decltype(clock_origin) last_point{};
    for (auto i{0U}; i <= 12; ++i) {
        auto const rotation_matrix = Matrix<T, D, D>::rotation(Axis::Z, i * pi_v<T> / 6);
        auto const point = rotation_matrix * scaling_matrix * translation_matrix * clock_origin;
        if (i != 0) {
            auto const x = point[0] + offset;
            auto const y = point[1] + offset;
            fmt::print("{:02d}: x: {:<7.3f} y: {:<7.3f}\n", i, x, y);
            canvas(x, y) = green;
            if (i != 1) {
                canvas(last_point[0] + offset, last_point[1] + offset) = black;
            }
            last_point = point;
        }

        std::ofstream image_file;
        try {
            image_file = open_file("image" + std::to_string(i) + ".ppm");
        } catch (std::system_error const& e) {
            std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
            std::terminate();
        }

        std::string const image_contents = canvas.as_ppm();
        image_file.write(image_contents.data(), long(image_contents.size()));
    }

    for (auto i{13U}; i <= 24; ++i) {
        auto const rotation_matrix = Matrix<T, D, D>::rotation(Axis::Z, i * pi_v<T> / 6);
        auto const point = rotation_matrix * scaling_matrix * translation_matrix * clock_origin;
        canvas(last_point[0] + offset, last_point[1] + offset) = black;
        canvas(point[0] + offset, point[1] + offset) = green;
        last_point = point;

        std::ofstream image_file;
        try {
            image_file = open_file("image" + std::to_string(i) + ".ppm");
        } catch (std::system_error const& e) {
            std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
            std::terminate();
        }

        std::string const image_contents = canvas.as_ppm();
        image_file.write(image_contents.data(), long(image_contents.size()));
    }
}

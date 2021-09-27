#include <cherry_blazer/canvas.hh>
#include <cherry_blazer/color.hh>
#include <cherry_blazer/matrix.hh>
#include <cherry_blazer/matrix_operations.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/types.hh>
#include <cherry_blazer/vector.hh>
#include <cherry_blazer/vector_operations.hh>

#include <cerrno>
#include <cmath>
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

    using T = decltype(clock_origin)::value_type;        // NOLINT(readability-identifier-naming)
    constexpr auto D = decltype(clock_origin)::size + 1; // NOLINT(readability-identifier-naming)
    constexpr auto clock_radius = 40.;

    std::vector const translation_matrices{
        Matrix<T, D, D>::translation({0., 1., 0.}), Matrix<T, D, D>::translation({1., 0., 0.}),
        Matrix<T, D, D>::translation({0., -1., 0.}), Matrix<T, D, D>::translation({-1., 0., 0.})};

    std::vector const rotation_matrices{Matrix<T, D, D>::rotation(Axis::Z, pi_v<T> / 3),
                                        Matrix<T, D, D>::rotation(Axis::Z, pi_v<T> / 6)};

    auto const scaling_matrix =
        Matrix<T, D, D>::scaling({clock_radius, clock_radius, clock_radius});

    std::vector<decltype(clock_origin)> translated_points;
    translated_points.reserve(translation_matrices.size() + translation_matrices.size());

    // Place hour 12, 3, 6, 9.
    for (auto const& translation_matrix : translation_matrices) {
        translated_points.emplace_back(scaling_matrix * translation_matrix * clock_origin);
    }

    // Place the rest of the clock.
    for (auto const& rotation_matrix : rotation_matrices) {
        for (auto const& translation_matrix : translation_matrices) {
            translated_points.emplace_back(rotation_matrix * scaling_matrix * translation_matrix *
                                           clock_origin);
        }
    }

    auto const canvas_size = clock_radius * 2.5;
    Canvas canvas{canvas_size, canvas_size};
    Color const white{255., 255., 255.};
    auto const offset = double(canvas.width()) / 2.;
    for (auto const& point : translated_points) {
        canvas(point[0] + offset, point[1] + offset) = white;
    }

    std::ofstream image_file;
    try {
        image_file = open_file("image.ppm");
    } catch (std::system_error const& e) {
        std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
        std::terminate();
    }

    std::string const image_contents = canvas.as_ppm();
    image_file.write(image_contents.data(), long(image_contents.size()));
}

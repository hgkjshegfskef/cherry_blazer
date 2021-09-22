#include <cherry_blazer/canvas.hh>
#include <cherry_blazer/color.hh>
#include <cherry_blazer/matrix.hh>
#include <cherry_blazer/matrix_operations.hh>
#include <cherry_blazer/point.hh>
#include <cherry_blazer/point_properties.hh>
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
    auto constexpr D = decltype(clock_origin)::size + 1; // NOLINT(readability-identifier-naming)
    auto constexpr clock_radius = 40.;

    std::vector const translation_matrices{Matrix<T, D, D>::translation(Vector{0., 1., 0.}),
                                           Matrix<T, D, D>::translation(Vector{1., 0., 0.}),
                                           Matrix<T, D, D>::translation(Vector{0., -1., 0.}),
                                           Matrix<T, D, D>::translation(Vector{-1., 0., 0.})};

    std::vector const rotation_matrices{Matrix<T, D, D>::rotation(Axis::Z, pi_v<T> / 3),
                                        Matrix<T, D, D>::rotation(Axis::Z, pi_v<T> / 6)};

    auto const scaling_matrix =
        Matrix<T, D, D>::scaling(Vector{clock_radius, clock_radius, clock_radius});

    std::vector<Point<T, D - 1>> translated_points;
    translated_points.reserve(translation_matrices.size() * 2);
    for (auto const& translation_matrix : translation_matrices) {
        translated_points.emplace_back(
            scale(scaling_matrix, translate(translation_matrix, clock_origin)));
    }

    for (auto const& rotation_matrix : rotation_matrices) {
        for (auto const& translation_matrix : translation_matrices) {
            translated_points.emplace_back(
                rotate(rotation_matrix,
                       scale(scaling_matrix, translate(translation_matrix, clock_origin))));
        }
    }

    auto constexpr canvas_size = clock_radius * 2.5;
    Canvas canvas{static_cast<u32>(std::round(canvas_size)),
                  static_cast<u32>(std::round(canvas_size))};
    Color const white{255., 255., 255.};
    auto const offset = canvas.width() / 2;
    for (auto const& point : translated_points) {
        canvas(static_cast<u32>(point.x) + offset, static_cast<u32>(point.y) + offset) = white;
    }

    std::ofstream image_file;
    try {
        image_file = open_file("image.ppm");
    } catch (std::system_error const& e) {
        std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
        throw;
    }

    std::string const image_contents = canvas.as_ppm();
    image_file.write(image_contents.data(), static_cast<long>(image_contents.size()));
}

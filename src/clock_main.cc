#include "canvas.hh"
#include "color.hh"
#include "matrix.hh"
#include "matrix_operations.hh"
#include "point.hh"
#include "point_properties.hh"
#include "safe_numerics_typedefs.hh"
#include "types.hh"
#include "vector.hh"
#include "vector_operations.hh"

#include <cerrno>
#include <cmath>
#include <iostream>
#include <numbers>
#include <ostream>
#include <string>
#include <system_error>
#include <vector>

namespace {
std::ofstream open_file(std::string const& file) {
    std::ofstream f{file};
    if (!f)
        throw std::system_error(errno, std::system_category(), "failed to open '" + file + "'");
    return f;
}

} // namespace

namespace cb = cherry_blazer;

int main() {
    cb::Point clock_origin{0., 0., 0.};

    using T = decltype(clock_origin)::value_type;        // NOLINT(readability-identifier-naming)
    auto constexpr D = decltype(clock_origin)::size + 1; // NOLINT(readability-identifier-naming)
    auto constexpr clock_radius = 40.;

    std::vector const translation_matrices{
        cb::Matrix<T, D, D>::translation(cb::Vector{0., 1., 0.}),
        cb::Matrix<T, D, D>::translation(cb::Vector{1., 0., 0.}),
        cb::Matrix<T, D, D>::translation(cb::Vector{0., -1., 0.}),
        cb::Matrix<T, D, D>::translation(cb::Vector{-1., 0., 0.})};

    std::vector const rotation_matrices{
        cb::Matrix<T, D, D>::rotation(Axis::Z, std::numbers::pi_v<T> / 3),
        cb::Matrix<T, D, D>::rotation(Axis::Z, std::numbers::pi_v<T> / 6)};

    auto const scaling_matrix =
        cb::Matrix<T, D, D>::scaling(cb::Vector{clock_radius, clock_radius, clock_radius});

    std::vector<cb::Point<T, D - 1>> translated_points;
    translated_points.reserve(translation_matrices.size() * 2);
    for (auto const& translation_matrix : translation_matrices) {
        translated_points.emplace_back(
            cb::scale(scaling_matrix, cb::translate(translation_matrix, clock_origin)));
    }

    for (auto const& rotation_matrix : rotation_matrices) {
        for (auto const& translation_matrix : translation_matrices) {
            translated_points.emplace_back(cb::rotate(
                rotation_matrix,
                cb::scale(scaling_matrix, cb::translate(translation_matrix, clock_origin))));
        }
    }

    auto constexpr canvas_size = clock_radius * 2.5;
    cb::Canvas canvas{static_cast<u32>(std::round(canvas_size)),
                      static_cast<u32>(std::round(canvas_size))};
    cb::Color const white{255., 255., 255.};
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

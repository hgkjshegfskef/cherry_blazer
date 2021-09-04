#include "canvas.hh"

#include <algorithm>
#include <boost/pfr/core.hpp>
#include <cassert>
#include <cerrno>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream> // IWYU pragma: keep
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>

namespace cherry_blazer {

namespace {

struct point2d {
    double x;
    double y;
};

struct range {
    double start;
    double finish;

    range(double start, double finish) : start{start}, finish{finish} {
        if (start > finish)
            throw std::domain_error{"start > finish"};
    }
};

// https://en.wikipedia.org/wiki/Linear_interpolation
// If the two known points are given by the coordinates (x0,y0) and (x1,y1), the linear interpolant
// is the straight line between these points. Value of x must be in the interval [x0;x1].
double linear_interpolation(double x, point2d const& left, point2d const& right) {
    assert(left.x <= x && x <= right.x);
    return left.y + (x - left.x) * (right.y - left.y) / (right.x - left.x);
}

// Scale a number between two (possibly overlapping) ranges.
// Use-case example: given a value in range [0;1], find out its respective value in range [0;255].
// Further reading: https://gamedev.stackexchange.com/a/33445
double scale(double x, range const& source, range const& target) {
    return linear_interpolation(x, {source.start, target.start}, {source.finish, target.finish});
}

} // namespace

Canvas::Canvas(std::size_t width, std::size_t height)
    : canvas_{std::unique_ptr<Color[]>{new Color[width * height]}}, width_{width}, height_{height} {
    if (width == 0 || height == 0)
        throw std::range_error{"width == 0 || height == 0"};
}

std::size_t Canvas::width() const { return width_; }
std::size_t Canvas::height() const { return height_; }

Color& Canvas::operator()(std::size_t x, std::size_t y) {
    assert(y * width_ + x < len()); // NOLINT(hicpp-no-array-decay)
    return canvas_[y * width_ + x];
}

Color const& Canvas::operator()(std::size_t x, std::size_t y) const {
    assert(y * width_ + x < len()); // NOLINT(hicpp-no-array-decay)
    return canvas_[y * width_ + x];
}

Color& Canvas::at(std::size_t x, std::size_t y) {
    if (y * width_ + x >= len())
        throw std::out_of_range{"range is out of bounds"};
    return canvas_[y * width_ + x];
}

Color const& Canvas::at(std::size_t x, std::size_t y) const {
    if (y * width_ + x >= len())
        throw std::out_of_range{"range is out of bounds"};
    return canvas_[y * width_ + x];
}

void Canvas::fill(Color const& color) { std::fill(canvas_.get(), canvas_.get() + len(), color); }

std::string ppm_generate_header(std::size_t width, std::size_t height,
                                std::size_t max_component_value) {
    std::stringstream ss;
    ss << "P3\n"
       << std::to_string(width) << ' ' << std::to_string(height) << '\n'
       << std::to_string(max_component_value) << '\n';
    return ss.str();
}

// std::ofstream ppm_write_header(std::string const& file, std::size_t width, std::size_t height) {
//     std::ofstream f{file};
//     if (!f)
//         throw std::system_error(errno, std::system_category(), "failed to open '" + file + "'");
//     f << ppm_generate_header(width, height);
//     return f;
// }

std::string Canvas::as_ppm() const {
    // TODO: when writing file saving routines, they should use binary write, not <<
    //    std::ofstream image_file;
    //    try {
    //        image_file = ppm_write_header(file_name, width_, height_);
    //    } catch (std::system_error const& e) {
    //        std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
    //        throw;
    //    }

    // Max length of line in PPM file
    constexpr std::size_t ppm_line_length = 70;
    // How much text space one color component (r, g, or b) occupies
    constexpr std::size_t component_width = 4;
    // How many primary colors does one color have
    constexpr std::size_t component_count = 3;
    // How much text space one color (r, g, and b) occupies
    constexpr std::size_t color_width = component_width * component_count;
    // How many colors fully fit into one line
    constexpr std::size_t batch_size = ppm_line_length / color_width;
    // How many full batches of colors are there to print?
    const std::size_t batch_count = len() / batch_size;

    // Range of canvas color colomponents' values (we clamp to it first).
    const range source{0, 1};
    // Range of PPM color values.
    const range target{0, 255};

    std::stringstream ss;
    ss << ppm_generate_header(width_, height_, target.finish);

    // Print out batch_count batches of batch_size amount of colors each, one batch per line.
    for (std::size_t nth_batch = 0; nth_batch < batch_count; ++nth_batch) {
        for (std::size_t nth_color = 0; nth_color < batch_size; ++nth_color) {
            // Clamp color components to the source range, then scale them to the target range.
            boost::pfr::for_each_field(
                canvas_[nth_batch * batch_size + nth_color], [&](auto const& component) {
                    ss << std::setw(component_width)
                       << std::round(scale(std::clamp(component, source.start, source.finish),
                                           source, target));
                });
        }
        ss << "\n";
    }

    if (len() - batch_count * batch_size) { // Any colors remaining to be processed?
        const std::size_t already_processed = batch_count * batch_size;
        for (std::size_t leftover = already_processed; leftover < len(); ++leftover) {
            boost::pfr::for_each_field(canvas_[leftover], [&](auto const& component) {
                ss << std::setw(component_width)
                   << std::round(scale(std::clamp(component, source.start, source.finish), source,
                                       target));
            });
        }
        ss << "\n";
    }

    return ss.str();
}

bool operator==(Canvas const& lhs, Canvas const& rhs) {
    if (std::tie(lhs.width_, lhs.height_) != std::tie(rhs.width_, rhs.height_))
        return false;
    std::size_t len = lhs.len();
    for (std::size_t i = 0; i < len; ++i) {
        if (lhs.canvas_[i] != rhs.canvas_[i])
            return false;
    }
    return true;
}

bool operator!=(Canvas const& lhs, Canvas const& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& os, Canvas const& c) {
    // In order to format the output nicely, it is necessary to print n-1 items with one delimiter,
    // and the nth item with a different delimiter.
    std::size_t penultimate_row = c.height_ - 1;
    std::size_t penultimate_col = c.width_ - 1;

    // for each row except last row
    for (std::size_t y = 0; y < penultimate_row; ++y) {
        os << "[ ";
        // for each column in row except last column
        for (std::size_t x = 0; x < penultimate_col; ++x) {
            // calculate offset in the 1D array
            os << c.canvas_[y * c.width_ + x] << ", ";
        }
        // print last column in row (with different delimiter)
        os << c.canvas_[y * c.width_ + penultimate_col] << " ]\n";
    }

    // print last row (with different delimiter)
    os << "[ ";
    // for each column in last row except last column
    for (std::size_t x = 0; x < penultimate_col; ++x) {
        os << c.canvas_[penultimate_row * c.width_ + x] << ", ";
    }
    // print last column in last row (with different delimiter)
    return os << c.canvas_[penultimate_row * c.width_ + penultimate_col] << " ]";
}

std::size_t Canvas::len() const { return width_ * height_; }

} // namespace cherry_blazer

#ifdef CHERRY_BLAZER_DEBUG
#include <memory>
// Explicitly instantiate unique_ptr to be able to debug it.
// https://stackoverflow.com/a/25506293/10986054
template class std::unique_ptr<cherry_blazer::Color[]>;
#endif

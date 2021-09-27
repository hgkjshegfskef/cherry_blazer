#include "canvas.hh"

#include "point.hh"
#include "ppm.hh"

#include <boost/assert.hpp>
#include <boost/pfr/core.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iomanip>
#include <string>
#include <utility>

namespace cherry_blazer {

namespace {

// https://en.wikipedia.org/wiki/Linear_interpolation
// If the two known points are given by the coordinates (x0,y0) and (x1,y1), the linear interpolant
// is the straight line between these points. Value of x must be in the interval [x0;x1].
constexpr double linear_interpolation(double x, Point2d const& left_point,
                                      Point2d const& right_point) {
    BOOST_VERIFY(left_point[0] <= x && x <= right_point[0]);
    return left_point[1] + (x - left_point[0]) * (right_point[1] - left_point[1]) /
                               (right_point[0] - left_point[0]);
}

// Scale a number between two (possibly overlapping) ranges.
// Use-case example: given a value in range [0;1], find out its respective value in range [0;255].
// Further reading: https://gamedev.stackexchange.com/a/33445
constexpr double scale(double value, Point2d const& source_range, Point2d const& target_range) {
    return linear_interpolation(value, {source_range[0], target_range[0]},
                                {source_range[1], target_range[1]});
}

} // namespace

Canvas::Canvas(std::size_t width, std::size_t height) {
    if (width == 0 || height == 0)
        throw std::logic_error{"Canvas: width and height must be non-zero."};
    if (width > 4096)
        throw std::logic_error{"Canvas: width should be <= 4096."};
    if (height > 2160)
        throw std::logic_error{"Canvas: height should be <= 2160."};

    canvas_ = std::make_unique<Color[]>(width * height);
    width_ = width;
    height_ = height;
}

Canvas::Canvas(unsigned width, unsigned height) : Canvas(std::size_t(width), std::size_t(height)) {}

Canvas::Canvas(int width, int height) : Canvas(std::size_t(width), std::size_t(height)) {}

Canvas::Canvas(double width, double height)
    : Canvas(std::size_t(std::round(width)), std::size_t(std::round(height))) {}

unsigned Canvas::width() const { return unsigned(width_); }

unsigned Canvas::height() const { return unsigned(height_); }

Color& Canvas::operator()(std::size_t x, std::size_t y) {
    BOOST_VERIFY(x < width_);
    BOOST_VERIFY(y < height_);
    return canvas_[y * width_ + x];
}

Color& Canvas::operator()(unsigned x, unsigned y) {
    return this->operator()(std::size_t(x), std::size_t(y));
}

Color& Canvas::operator()(int x, int y) { return this->operator()(std::size_t(x), std::size_t(y)); }

Color& Canvas::operator()(double x, double y) {
    return this->operator()(std::size_t(std::round(x)), std::size_t(std::round(y)));
}

Color const& Canvas::operator()(std::size_t x, std::size_t y) const {
    BOOST_VERIFY(x < width_);
    BOOST_VERIFY(y < height_);
    return canvas_[y * width_ + x];
}

Color const& Canvas::operator()(unsigned x, unsigned y) const {
    return this->operator()(std::size_t(x), std::size_t(y));
}

Color const& Canvas::operator()(int x, int y) const {
    return this->operator()(std::size_t(x), std::size_t(y));
}

Color const& Canvas::operator()(double x, double y) const {
    return this->operator()(std::size_t(std::round(x)), std::size_t(std::round(y)));
}

std::size_t Canvas::size() const { return width_ * height_; }

void Canvas::fill(Color const& color) { std::fill(canvas_.get(), canvas_.get() + size(), color); }

std::string Canvas::as_ppm() const {
    // Max length of line in PPM file.
    constexpr auto ppm_line_length = 70;
    // How much text space one color component (r, g, or b) occupies?
    constexpr auto component_width = 4;
    // How many primary colors does one color have?
    constexpr auto component_count = 3;
    // How much text space one color (r, g, and b) occupies?
    constexpr auto color_width = component_width * component_count;
    // How many colors fully fit into one line?
    constexpr auto batch_size = ppm_line_length / color_width;
    // How many full batches of colors are there to print?
    const auto batch_count = size() / batch_size;

    // Range of canvas color components' values.
    constexpr Point component_range{0.0, 1.0};
    // Range of PPM color values.
    constexpr Point ppm_range{0.0, 255.0};

    std::stringstream ss;
    ss << ppm::generate_header(width_, height_, std::size_t(std::round(ppm_range[1])));

    // Print out batch_count batches of batch_size amount of colors each, one batch per line.
    for (auto nth_batch{0U}; nth_batch < batch_count; ++nth_batch) {
        for (auto nth_color{0U}; nth_color < batch_size; ++nth_color) {
            // Clamp color components to the source range, then scale them to the target range.
            boost::pfr::for_each_field(
                canvas_[nth_batch * batch_size + nth_color], [&](auto const& component) {
                    auto const clamped =
                        std::clamp(component, component_range[0], component_range[1]);
                    auto const scaled = scale(clamped, component_range, ppm_range);
                    auto const rounded = std::round(scaled);
                    ss << std::setw(component_width) << rounded;
                });
        }
        ss << "\n";
    }

    if ((size() - batch_count * batch_size) != 0) { // Any colors remaining to be processed?
        auto already_processed = batch_count * batch_size;
        for (auto leftover{already_processed}; leftover < size(); ++leftover) {
            boost::pfr::for_each_field(canvas_[leftover], [&](auto const& component) {
                auto const clamped = std::clamp(component, component_range[0], component_range[1]);
                auto const scaled = scale(clamped, component_range, ppm_range);
                auto const rounded = std::round(scaled);
                ss << std::setw(component_width) << rounded;
            });
        }
        ss << "\n";
    }

    return ss.str();
}

bool operator==(Canvas const& lhs, Canvas const& rhs) {
    if (std::tie(lhs.width_, lhs.height_) != std::tie(rhs.width_, rhs.height_))
        return false;
    auto size = lhs.size();
    for (auto i{0U}; i < size; ++i) {
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
    for (auto y{0U}; y < penultimate_row; ++y) {
        os << "[ ";
        // for each column in row except last column
        for (auto x{0U}; x < penultimate_col; ++x) {
            // calculate offset in the 1D array
            os << c.canvas_[y * c.width_ + x] << ", ";
        }
        // print last column in row (with different delimiter)
        os << c.canvas_[y * c.width_ + penultimate_col] << " ]\n";
    }

    // print last row (with different delimiter)
    os << "[ ";
    // for each column in last row except last column
    for (auto x{0U}; x < penultimate_col; ++x) {
        os << c.canvas_[penultimate_row * c.width_ + x] << ", ";
    }
    // print last column in last row (with different delimiter)
    return os << c.canvas_[penultimate_row * c.width_ + penultimate_col] << " ]";
}

} // namespace cherry_blazer

#ifdef CHERRY_BLAZER_DEBUG
#include <memory>
// Explicitly instantiate unique_ptr to be able to debug it.
// https://stackoverflow.com/a/25506293/10986054
template class std::unique_ptr<cherry_blazer::Color[]>;
#endif

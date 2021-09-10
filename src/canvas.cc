#include "canvas.hh"

#include "ppm.hh"
#include "types.hh"

#include <algorithm>
#include <boost/pfr/core.hpp>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
#include <tuple>
#include <utility>

namespace cherry_blazer {

namespace {

// https://en.wikipedia.org/wiki/Linear_interpolation
// If the two known points are given by the coordinates (x0,y0) and (x1,y1), the linear interpolant
// is the straight line between these points. Value of x must be in the interval [x0;x1].
constexpr double linear_interpolation(double x, std::pair<double, double> const& left_point,
                                      std::pair<double, double> const& right_point) {
    assert(left_point.first <= x && x <= right_point.first);
    return left_point.second + (x - left_point.first) * (right_point.second - left_point.second) /
                                   (right_point.first - left_point.first);
}

// Scale a number between two (possibly overlapping) ranges.
// Use-case example: given a value in range [0;1], find out its respective value in range [0;255].
// Further reading: https://gamedev.stackexchange.com/a/33445
constexpr double scale(double value, std::pair<double, double> const& source_range,
                       std::pair<double, double> const& target_range) {
    return linear_interpolation(value, {source_range.first, target_range.first},
                                {source_range.second, target_range.second});
}

} // namespace

Canvas::Canvas(width_t const& width, height_t const& height)
    : canvas_{new Color[u32(width * height)]()}, width_{width}, height_{height} {}

Canvas::width_t const& Canvas::width() const { return width_; }

Canvas::height_t const& Canvas::height() const { return height_; }

Color& Canvas::operator()(col_t const& x, row_t const& y) { return canvas_[y * width_ + x]; }

Color const& Canvas::operator()(col_t const& x, row_t const& y) const {
    return canvas_[y * width_ + x];
}

auto Canvas::size() const { return width_ * height_; }

void Canvas::fill(Color const& color) {
    std::fill(canvas_.get(), canvas_.get() + u32(size()), color);
}

std::string Canvas::as_ppm() const {
    // Max length of line in PPM file.
    constexpr safe_uliteral_auto_trap<70> ppm_line_length;
    // How much text space one color component (r, g, or b) occupies?
    constexpr safe_uliteral_auto_trap<4> component_width;
    // How many primary colors does one color have?
    constexpr safe_uliteral_auto_trap<3> component_count;
    // How much text space one color (r, g, and b) occupies?
    constexpr auto color_width = component_width * component_count;
    // How many colors fully fit into one line?
    constexpr safe_auto<uint_fast8_t> batch_size = ppm_line_length / color_width;
    // How many full batches of colors are there to print?
    const auto batch_count = size() / batch_size;

    // Range of canvas color components' values.
    constexpr auto component_min{0.0};
    constexpr auto component_max{1.0};
    constexpr std::pair component_range{component_min, component_max};

    //    constexpr Point2d component_range{component_min, component_max};

    //    constexpr safe_uliteral_auto_trap<0> component_min{};
    //    constexpr safe_uliteral_auto_trap<1> component_max{};
    //    constexpr safe_urange_auto_trap<component_min, component_max>
    //    component_range{component_min};

    // Range of PPM color values.
    constexpr auto ppm_min{0.0};
    constexpr auto ppm_max{255.0};
    constexpr std::pair ppm_range{ppm_min, ppm_max};

    //    constexpr Point2d ppm_range{ppm_min, ppm_max};

    //    constexpr safe_uliteral_auto_trap<0> ppm_min{};
    //    constexpr safe_uliteral_auto_trap<255> ppm_max{};
    //    constexpr safe_urange_auto_trap<ppm_min, ppm_max> ppm_range{ppm_min};

    std::stringstream ss;
    ss << ppm::generate_header(width_, height_, u32(ppm_max));

    // Print out batch_count batches of batch_size amount of colors each, one batch per line.
    for (auto nth_batch{0U}; nth_batch < batch_count; ++nth_batch) {
        for (auto nth_color{0U}; nth_color < batch_size; ++nth_color) {
            // Clamp color components to the source range, then scale them to the target range.
            boost::pfr::for_each_field(
                canvas_[nth_batch * batch_size + nth_color], [&](auto const& component) {
                    ss << std::setw(component_width)
                       << std::round(scale(std::clamp(component, component_min, component_max),
                                           component_range, ppm_range));
                });
        }
        ss << "\n";
    }

    if ((size() - batch_count * batch_size) != 0) { // Any colors remaining to be processed?
        auto already_processed = batch_count * batch_size;
        for (auto leftover{already_processed}; leftover < size(); ++leftover) {
            boost::pfr::for_each_field(canvas_[leftover], [&](auto const& component) {
                ss << std::setw(component_width)
                   << std::round(scale(std::clamp(component, component_min, component_max),
                                       component_range, ppm_range));
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

    const safe_auto<u16> penultimate_row = c.height_ - 1;
    const safe_auto<u16> penultimate_col = c.width_ - 1;

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

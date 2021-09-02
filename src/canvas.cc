#include "canvas.hh"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <ostream>
#include <tuple>

namespace cherry_blazer {

Canvas::Canvas(std::size_t width, std::size_t height)
    : canvas_{std::unique_ptr<Color[]>{new Color[width * height]}}, width_{width}, height_{height} {
    if (width == 0 || height == 0)
        throw std::range_error{"width == 0 || height == 0"};
}

std::size_t Canvas::width() const { return width_; }
std::size_t Canvas::height() const { return height_; }

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

} // namespace cherry_blazer

#ifdef CHERRY_BLAZER_DEBUG
#include <memory>
// Explicitly instantiate unique_ptr to be able to debug it.
// https://stackoverflow.com/a/25506293/10986054
template class std::unique_ptr<cherry_blazer::Color[]>;
#endif

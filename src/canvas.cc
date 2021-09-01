#include "canvas.hh"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <ostream>
#include <tuple>

namespace cherry_blazer {

Canvas::Canvas(int width, int height) : canvas_(height, std::vector<Color>(width)) {}

Canvas::size_type Canvas::width() const {
    assert(!canvas_.empty()); // NOLINT(hicpp-no-array-decay)

    return canvas_[0].size();
}

Canvas::size_type Canvas::height() const {
    assert(!canvas_.empty()); // NOLINT(hicpp-no-array-decay)

    // Due to zero-initialization of inner container in default constructor the outer vector will
    // always contain 1 element (that is, inner vector) during default initialization.
    auto outer_size = canvas_.size();
    return outer_size == 1 ? 0 : outer_size;
}

std::ostream& operator<<(std::ostream& os, std::vector<Color> const& canvas_row) {
    os << "[ ";
    std::copy(std::cbegin(canvas_row), std::prev(std::cend(canvas_row)),
              std::ostream_iterator<Color>{os, ", "});
    return os << *std::prev(std::cend(canvas_row)) << " ]";
}

std::ostream& operator<<(std::ostream& os, std::vector<std::vector<Color>> const& canvas) {
    std::copy(std::cbegin(canvas), std::prev(std::cend(canvas)),
              std::ostream_iterator<std::vector<Color>>{os, "\n"});
    return os << *std::prev(std::cend(canvas));
}

std::ostream& operator<<(std::ostream& os, Canvas const& c) { return os << c.canvas_; }

} // namespace cherry_blazer

#include "color.hh"

#include "detail/util.hh"

#include <iomanip>

namespace cherry_blazer {

Color operator*(Color const& c, double scalar) {
    return {c.r * scalar, c.g * scalar, c.b * scalar};
}

Color operator*(double scalar, Color const& c) { return c * scalar; }

Color operator/(Color const& c, double scalar) {
    return {c.r / scalar, c.g / scalar, c.b / scalar};
}

Color& operator+=(Color& lhs, Color const& rhs) {
    lhs.r += rhs.r;
    lhs.g += rhs.g;
    lhs.b += rhs.b;
    return lhs;
}

Color& operator-=(Color& lhs, Color const& rhs) {
    lhs.r -= rhs.r;
    lhs.g -= rhs.g;
    lhs.b -= rhs.b;
    return lhs;
}

Color& operator*=(Color& lhs, Color const& rhs) {
    lhs.r *= rhs.r;
    lhs.g *= rhs.g;
    lhs.b *= rhs.b;
    return lhs;
}

Color operator+(Color lhs, Color const& rhs) { return lhs += rhs; }

Color operator-(Color lhs, Color const& rhs) { return lhs -= rhs; }

Color operator*(Color lhs, Color const& rhs) { return lhs *= rhs; }

bool operator==(Color const& lhs, Color const& rhs) {
    // floating-point comparison through epsilon
    return detail::almost_equal(lhs.r, rhs.r) && detail::almost_equal(lhs.g, rhs.g) &&
           detail::almost_equal(lhs.b, rhs.b);
}

bool operator!=(Color const& lhs, Color const& rhs) { return !(lhs == rhs); }

std::ostream& operator<<(std::ostream& os, Color const& c) {
    return os << "(" << std::setw(3) << c.r << std::setw(4) << c.g << std::setw(4) << c.b << ")";
}

} // namespace cherry_blazer

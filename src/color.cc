#include "color.hh"

#include "util.hh"

namespace cherry_blazer {

Color::Color(double r, double g, double b) : r{r}, g{g}, b{b} {}

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
    return almost_equal(lhs.r, rhs.r) && almost_equal(lhs.g, rhs.g) && almost_equal(lhs.b, rhs.b);
}

bool operator!=(Color const& lhs, Color const& rhs) { return !(lhs == rhs); }

} // namespace cherry_blazer

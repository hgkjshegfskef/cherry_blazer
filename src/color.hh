#ifndef CHERRY_BLAZER_SRC_COLOR_HH_
#define CHERRY_BLAZER_SRC_COLOR_HH_

#include <ostream>

namespace cherry_blazer {

struct Color {
    double r;
    double g;
    double b;
};

// Color*scalar
Color operator*(Color const& c, double scalar);

// scalar*Color
Color operator*(double scalar, Color const& c);

// Color/scalar
Color operator/(Color const& c, double scalar);

// scalar/Color (= ERROR)

// Color += Color (= Color)
Color& operator+=(Color& lhs, Color const& rhs);

// Color -= Color (= Color)
Color& operator-=(Color& lhs, Color const& rhs);

// Color *= Color (= Color)
Color& operator*=(Color& lhs, Color const& rhs);

// Color + Color = Color
Color operator+(Color lhs, Color const& rhs);

// Color - Color = Color
Color operator-(Color lhs, Color const& rhs);

// Color * Color = Color
Color operator*(Color lhs, Color const& rhs);

// Colors can be compared for equality.
bool operator==(Color const& lhs, Color const& rhs);

// Colors can be compared for inequality.
bool operator!=(Color const& lhs, Color const& rhs);

std::ostream& operator<<(std::ostream& os, Color const& c);

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_COLOR_HH_

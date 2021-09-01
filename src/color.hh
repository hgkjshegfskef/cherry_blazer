#ifndef CHERRY_BLAZER_SRC_COLOR_HH_
#define CHERRY_BLAZER_SRC_COLOR_HH_

#include <ostream>
#include <type_traits>

namespace cherry_blazer {

struct Color {
    double r{};
    double g{};
    double b{};

    Color() = default;
    Color(double r, double g, double b);
};

static_assert(
    std::is_trivially_copyable_v<Color>,
    "Being trivially copyable means that compiler can optimize a bit better when this struct is "
    "used. For example, it can memcpy it instead of calling copy constructor. Or it can pass the "
    "struct inside a register, instead of passing a pointer to the struct. Considering how often "
    "this struct will be used in the project, it is preferable to keep it trivially copyable.");

static_assert(std::is_nothrow_move_constructible_v<Color>,
              "It is preferable to have move ctor to be noexcept, otherwise e.g. std::vector "
              "cannot move this type.");
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

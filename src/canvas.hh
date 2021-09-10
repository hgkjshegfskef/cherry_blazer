#ifndef CHERRY_BLAZER_SRC_CANVAS_HH_
#define CHERRY_BLAZER_SRC_CANVAS_HH_

#include "color.hh"
#include "safe_numerics_typedefs.hh"
#include "types.hh"

#include <boost/safe_numerics/automatic.hpp>
#include <boost/safe_numerics/safe_integer.hpp>
#include <boost/safe_numerics/safe_integer_literal.hpp>
#include <boost/safe_numerics/safe_integer_range.hpp>
#include <fstream>
#include <memory>
#include <string>

namespace cherry_blazer {

// Canvas is a 2D buffer to write colors into, either one color at a time via "canvas(x, y) =
// color" or one color in bulk via fill(). Canvas coordinate system: X grows from left to right,
// Y grows from up to down.
class Canvas {
    // Smallest valid image is 1x1.
    static inline constexpr safe_uliteral_auto_trap<1> width_min{};
    static inline constexpr safe_uliteral_auto_trap<1> height_min{};
    // We will not render images bigger than 4K.
    static inline constexpr safe_uliteral_auto_trap<3840> width_max{};
    static inline constexpr safe_uliteral_auto_trap<2160> height_max{};

    // Reading/writing canvas via operator() is 0-based, hence we need the offset:
    static inline constexpr safe_uliteral_auto_trap<1> access_offset{};
    // Minimum access bounds:
    static inline constexpr safe_auto<u8> col_min{width_min - access_offset};
    static inline constexpr safe_auto<u8> row_min{height_min - access_offset};
    // Maximum access bounds:
    static inline constexpr safe_auto<u16> col_max{width_max - access_offset};
    static inline constexpr safe_auto<u16> row_max{height_max - access_offset};

  public:
    // We define two ranges of accepted values:

    // 1. Range of canvas sizes: width and height:
    using width_t = safe_urange_auto<width_min, width_max>;
    using height_t = safe_urange_auto<height_min, height_max>;

    // 2. Range of canvas underlying container access bounds: same as width and height minus 1:
    using col_t = safe_urange_auto<col_min, col_max>;
    using row_t = safe_urange_auto<row_min, row_max>;

    // Create canvas of certain width (x) and height (y).
    Canvas(width_t const& width, height_t const& height);

    [[nodiscard]] width_t const& width() const;
    [[nodiscard]] height_t const& height() const;

    // Read at [x, y]
    [[nodiscard]] Color const& operator()(col_t const& x, row_t const& y) const;

    // Write at [x, y]
    Color& operator()(col_t const& x, row_t const& y);

    // Fill whole canvas with a single color.
    void fill(Color const& color);

    [[nodiscard]] std::string as_ppm() const;

    friend bool operator==(Canvas const& lhs, Canvas const& rhs);
    friend bool operator!=(Canvas const& lhs, Canvas const& rhs);

    friend std::ostream& operator<<(std::ostream& os, Canvas const& c);

  private:
    std::unique_ptr<Color[]> canvas_; // 2D, but in 1D array
    width_t width_;
    height_t height_;

    [[nodiscard]] auto size() const;
};

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CANVAS_HH_

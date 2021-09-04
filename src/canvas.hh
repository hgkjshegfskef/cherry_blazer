#ifndef CHERRY_BLAZER_SRC_CANVAS_HH_
#define CHERRY_BLAZER_SRC_CANVAS_HH_

#include "color.hh"

#include <fstream>
#include <memory>
#include <type_traits>

namespace cherry_blazer {

class Canvas {
  public:
    // Default meember initialization makes no sense for this class.
    Canvas() = delete;

    // Create canvas of certain width (x) and height (y).
    Canvas(std::size_t width, std::size_t height);

    [[nodiscard]] std::size_t width() const;
    [[nodiscard]] std::size_t height() const;

    // Read/write at [x, y]
    Color& operator()(std::size_t x, std::size_t y);
    Color const& operator()(std::size_t x, std::size_t y) const;

    // Checked read/write at [x, y]
    Color& at(std::size_t x, std::size_t y);
    [[nodiscard]] Color const& at(std::size_t x, std::size_t y) const;

    // Fill whole canvas with a single color.
    void fill(Color const& color);

    [[nodiscard]] std::string as_ppm() const;

    friend bool operator==(Canvas const& lhs, Canvas const& rhs);
    friend bool operator!=(Canvas const& lhs, Canvas const& rhs);

    friend std::ostream& operator<<(std::ostream& os, Canvas const& c);

  private:
    std::unique_ptr<Color[]> canvas_; // 2D, but in 1D array
    std::size_t width_;
    std::size_t height_;

    [[nodiscard]] std::size_t len() const { return width_ * height_; }
};

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CANVAS_HH_

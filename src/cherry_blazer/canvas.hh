#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_CANVAS_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_CANVAS_HH_

#include "color.hh"
#include "types.hh"

#include <fstream>
#include <memory>
#include <string>

namespace cherry_blazer {

// Canvas is a 2D buffer to write colors into, either one color at a time via "canvas(x, y) =
// color" or one color in bulk via fill(). Canvas coordinate system: X grows from left to right,
// Y grows from up to down.
class Canvas {
  public:
    Canvas(stdsize_t width, stdsize_t height);

    [[nodiscard]] stdsize_t width() const;
    [[nodiscard]] stdsize_t height() const;

    [[nodiscard]] Color const& operator()(stdsize_t x, stdsize_t y) const;
    Color& operator()(stdsize_t x, stdsize_t y);

    // Fill whole canvas with a single color.
    void fill(Color const& color);

    [[nodiscard]] std::string as_ppm() const;

    friend bool operator==(Canvas const& lhs, Canvas const& rhs);
    friend bool operator!=(Canvas const& lhs, Canvas const& rhs);

    friend std::ostream& operator<<(std::ostream& os, Canvas const& c);

  private:
    std::unique_ptr<Color[]> canvas_; // 2D, but in 1D array
    stdsize_t width_;
    stdsize_t height_;

    [[nodiscard]] u64 size() const;
};

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_CANVAS_HH_

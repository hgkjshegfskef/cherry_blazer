#ifndef CHERRY_BLAZER_SRC_CHERRY_BLAZER_CANVAS_HH_
#define CHERRY_BLAZER_SRC_CHERRY_BLAZER_CANVAS_HH_

#include "color.hh"

#include <fstream>
#include <memory>
#include <string>

namespace cherry_blazer {

// Canvas is a 2D buffer to write colors into, either one color at a time via "canvas(x, y) =
// color" or one color in bulk via fill(). Canvas coordinate system: X grows from left to right,
// Y grows from up to down.
class Canvas {
  public:
    Canvas(std::size_t width, std::size_t height);
    Canvas(unsigned width, unsigned height);
    Canvas(int width, int height);
    Canvas(double width, double height);

    [[nodiscard]] std::size_t width() const;
    [[nodiscard]] std::size_t height() const;

    [[nodiscard]] Color const& operator()(std::size_t x, std::size_t y) const;
    [[nodiscard]] Color const& operator()(unsigned x, unsigned y) const;
    [[nodiscard]] Color const& operator()(int x, int y) const;
    [[nodiscard]] Color const& operator()(double x, double y) const;

    Color& operator()(std::size_t x, std::size_t y);
    Color& operator()(unsigned x, unsigned y);
    Color& operator()(int x, int y);
    Color& operator()(double x, double y);

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

    [[nodiscard]] std::size_t size() const;
};

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CHERRY_BLAZER_CANVAS_HH_

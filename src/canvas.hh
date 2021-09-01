#ifndef CHERRY_BLAZER_SRC_CANVAS_HH_
#define CHERRY_BLAZER_SRC_CANVAS_HH_

#include "color.hh"

#include <ostream>
#include <type_traits>
#include <vector>

#ifdef CHERRY_BLAZER_TEST
#include <gtest/gtest.h>
#endif

namespace cherry_blazer {

class Canvas {
    template <typename CanvasVector> class Canvas1d {
      public:
        Canvas1d(CanvasVector& canvas, std::size_t i) : canvas_{canvas}, i_{i} {}

        Canvas1d() = delete;
        ~Canvas1d() = default;
        // Attempt to prevent user from storing Canvas1d.
        Canvas1d(const Canvas1d&) = delete;
        Canvas1d& operator=(const Canvas1d&) = delete;
        Canvas1d(Canvas1d&&) noexcept = default;
        Canvas1d& operator=(Canvas1d&&) noexcept = default;

        Color& operator[](std::size_t j) { return canvas_[i_][j]; }
        Color const& operator[](std::size_t j) const { return canvas_[i_][j]; }

      private:
        CanvasVector& canvas_;
        std::size_t i_;
    };

  public:
    using size_type = std::vector<Color>::size_type;

    Canvas() = default;
    Canvas(int width, int height);

    static_assert(std::is_trivially_copyable_v<Canvas1d<const std::vector<std::vector<Color>>>>);

    Canvas1d<std::vector<std::vector<Color>>> operator[](std::size_t idx) { return {canvas_, idx}; }

    Canvas1d<const std::vector<std::vector<Color>>> operator[](std::size_t idx) const {
        return {canvas_, idx};
    }

    [[nodiscard]] size_type width() const;
    [[nodiscard]] size_type height() const;

    // On the benefits of hidden friends:
    // https://www.justsoftwaresolutions.co.uk/cplusplus/hidden-friends.html
    friend bool operator==(Canvas const& lhs, Canvas const& rhs) {
        return lhs.canvas_ == rhs.canvas_;
    }
    friend bool operator!=(Canvas const& lhs, Canvas const& rhs) { return !(lhs == rhs); }

    // Not inline because it uses helper overloads declared below.
    friend std::ostream& operator<<(std::ostream& os, Canvas const& c);

  private:
    std::vector<std::vector<Color>> canvas_{{}};

#ifdef CHERRY_BLAZER_TEST
    friend class CanvasTest;
    FRIEND_TEST(CanvasTest, CanvasDefaultConstructable);
    FRIEND_TEST(CanvasTest, CanvasWidthHeightCtor);
#endif
};

std::ostream& operator<<(std::ostream& os, std::vector<Color> const& canvas_row);

std::ostream& operator<<(std::ostream& os, std::vector<std::vector<Color>> const& canvas);

} // namespace cherry_blazer

#endif // CHERRY_BLAZER_SRC_CANVAS_HH_

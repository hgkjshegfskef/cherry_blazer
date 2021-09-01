#ifndef CHERRY_BLAZER_SRC_CANVAS_HH_
#define CHERRY_BLAZER_SRC_CANVAS_HH_

#include "color.hh"

#include <ostream>
#include <vector>

#ifdef CHERRY_BLAZER_TEST
#include <gtest/gtest.h>
#endif

namespace cherry_blazer {

class Canvas {
  public:
    using size_type = std::vector<Color>::size_type;
    using reference = std::vector<std::vector<Color>>::reference;
    using const_reference = std::vector<std::vector<Color>>::const_reference;

    Canvas() = default;
    Canvas(int width, int height);

    reference operator[](std::size_t idx);
    const_reference operator[](std::size_t idx) const;

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

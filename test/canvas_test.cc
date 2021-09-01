#include "canvas.hh"

#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

namespace cherry_blazer {

class CanvasTest : public testing::Test {
  protected:
    void SetUp() override {
        ASSERT_TRUE(!c1.canvas_.empty());
        ASSERT_TRUE(!c2.canvas_.empty());
    }

    Canvas c1;
    Canvas c2{3, 5};
    Color red{1, 0, 0};
};

TEST_F(CanvasTest, CanvasDefaultConstructable) { // NOLINT
    EXPECT_EQ(c1.width(), 0);
    EXPECT_EQ(c1.height(), 0);
    for (const auto& row : c1.canvas_) {
        for (const auto& color : row) {
            EXPECT_EQ(color, Color(0, 0, 0));
        }
    }
}

TEST_F(CanvasTest, CanvasWidthHeightCtor) { // NOLINT
    EXPECT_EQ(c2.width(), 3);
    EXPECT_EQ(c2.height(), 5);
    for (const auto& row : c2.canvas_) {
        for (const auto& color : row) {
            EXPECT_EQ(color, Color(0, 0, 0));
        }
    }
}

TEST_F(CanvasTest, CanvasSubscriptOperator) { // NOLINT
    EXPECT_ANY_THROW(c1[0].at(0));            // NOLINT
    c2[0][0] = red;
    EXPECT_EQ(c2[0][0], Color(1, 0, 0));
}

TEST_F(CanvasTest, CanvasPrintOut) { // NOLINT
    for (Canvas::size_type i = 0; i < c2.height(); ++i) {
        for (Canvas::size_type j = 0; j < c2.width(); ++j)
            c2[i][j] = red;
    }
    std::string s{"[ (  1   0   0), (  1   0   0), (  1   0   0) ]\n"
                  "[ (  1   0   0), (  1   0   0), (  1   0   0) ]\n"
                  "[ (  1   0   0), (  1   0   0), (  1   0   0) ]\n"
                  "[ (  1   0   0), (  1   0   0), (  1   0   0) ]\n"
                  "[ (  1   0   0), (  1   0   0), (  1   0   0) ]"};
    std::stringstream ss;
    ss << s;
    EXPECT_EQ(ss.str(), s);
}

} // namespace cherry_blazer

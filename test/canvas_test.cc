#include "canvas.hh"

#include <algorithm>
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace cherry_blazer {

TEST(CanvasCtorTest, CanvasCtor) { // NOLINT
    Canvas c{1, 2};
    EXPECT_THROW({ Canvas cc(0, 1); }, std::range_error);
    EXPECT_THROW({ Canvas cc(1, 0); }, std::range_error);
    EXPECT_THROW({ Canvas cc(0, 0); }, std::range_error);
}

class CanvasTest : public testing::Test {
  protected:
    void fill_c1() {
        std::size_t c1_filled_len = c1_filled.width() * c1_filled.height();
        std::fill(c1_filled.canvas_.get(), c1_filled.canvas_.get() + c1_filled_len, red);
        // alternative way to fill:
        //        for (std::size_t y = 0; y < c1_filled.height(); ++y) {
        //            for (std::size_t x = 0; x < c1_filled.width(); ++x)
        //                c1_filled(x, y) = red;
        //        }
    }

    void SetUp() override { fill_c1(); }

    Canvas c1{3, 5};
    Canvas c1_filled{3, 5};
    Color red{1, 0, 0};
};

TEST_F(CanvasTest, CanvasWidth) { // NOLINT
    EXPECT_EQ(c1.width(), 3);
}

TEST_F(CanvasTest, CanvasHeight) { // NOLINT
    EXPECT_EQ(c1.height(), 5);
}

TEST_F(CanvasTest, CanvasCallOperator) { // NOLINT
    c1(0, 0) = red;
    Color const& color = c1(0, 0);
    EXPECT_EQ(color, red);
}

TEST_F(CanvasTest, CanvasAtOperator) { // NOLINT
    c1.at(0, 0) = red;
    Color const& color = c1.at(0, 0);
    EXPECT_EQ(color, red);
    EXPECT_THROW(c1.at(123, 123), std::out_of_range);
}

TEST_F(CanvasTest, CanvasEquality) { // NOLINT
    Canvas c2{3, 5};
    EXPECT_EQ(c1, c2);
}

TEST_F(CanvasTest, CanvasInequality) { // NOLINT
    Canvas c2{1, 2};
    EXPECT_NE(c1, c2);

    Canvas c3{3, 5};
    c3(2, 4) = Color{1, 2, 3};
    EXPECT_NE(c1, c3);
}

TEST_F(CanvasTest, CanvasPrintOut) { // NOLINT
    std::string s{"[ (  1   0   0), (  1   0   0), (  1   0   0) ]\n"
                  "[ (  1   0   0), (  1   0   0), (  1   0   0) ]\n"
                  "[ (  1   0   0), (  1   0   0), (  1   0   0) ]\n"
                  "[ (  1   0   0), (  1   0   0), (  1   0   0) ]\n"
                  "[ (  1   0   0), (  1   0   0), (  1   0   0) ]"};
    std::stringstream ss;
    ss << c1_filled;
    EXPECT_EQ(ss.str(), s) << c1_filled;
}

/*

TEST_F(CanvasTest, CanvasDefaultConstructable) { // NOLINT
    EXPECT_EQ(c1.width(), 0);
    EXPECT_EQ(c1.height(), 0);
    //    EXPECT_EQ(c1[0][0], Color(0, 0, 0)); -- segfaults
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
    c2[0][0] = red;
    EXPECT_EQ(c2[0][0], Color(1, 0, 0));
}


 */

} // namespace cherry_blazer

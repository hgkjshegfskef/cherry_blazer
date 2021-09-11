#include "canvas.hh"

#include <gtest/gtest.h>
#include <string>
#include <system_error>

namespace cherry_blazer {

TEST(CanvasCtorTest, CanvasCtor) { // NOLINT
    Canvas c{1, 2};
}

TEST(CanvasCtorTest, CanvasCtorOutOfRange) {                     // NOLINT
    EXPECT_THROW({ Canvas cc(0, 1); }, std::system_error);       // NOLINT
    EXPECT_THROW({ Canvas cc(1, 0); }, std::system_error);       // NOLINT
    EXPECT_THROW({ Canvas cc(0, 0); }, std::system_error);       // NOLINT
    EXPECT_THROW({ Canvas cc(3841, 1); }, std::system_error);    // NOLINT
    EXPECT_THROW({ Canvas cc(1, 2161); }, std::system_error);    // NOLINT
    EXPECT_THROW({ Canvas cc(3841, 2161); }, std::system_error); // NOLINT
}

TEST(CanvasCtorTest, CanvasIsZeroInitialized) { // NOLINT
    Canvas c{1, 2};
    Color black{};
    for (auto y{0U}; y < c.height(); ++y) {
        for (auto x{0U}; x < c.width(); ++x)
            EXPECT_EQ(c(x, y), black);
    }
}

class CanvasTest : public testing::Test {
  protected:
    void SetUp() override { c1_filled.fill(red); }

    Canvas c1{3, 5};
    Canvas c1_filled{3, 5};
    Color red{255, 0, 0};
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

TEST_F(CanvasTest, CanvasExportPpmSimple) { // NOLINT
    Canvas c2{5, 3};
    Color col1{1.5, 0, 0};
    Color col2{0, 0.5, 0};
    Color col3{-0.5, 0, 1};
    c2(0, 0) = col1;
    c2(2, 1) = col2;
    c2(4, 2) = col3;

    std::string const image = c2.as_ppm();
    std::string expected{"P3\n"
                         "5 3\n"
                         "255\n"
                         " 255   0   0   0   0   0   0   0   0   0   0   0   0   0   0\n"
                         "   0   0   0   0   0   0   0 128   0   0   0   0   0   0   0\n"
                         "   0   0   0   0   0   0   0   0   0   0   0   0   0   0 255\n"};
    EXPECT_EQ(image, expected) << image;
}

TEST_F(CanvasTest, CanvasExportPpmLongLines) { // NOLINT
    Canvas c2{10, 2};
    Color col1{1, 0.8, 0.6};
    c2.fill(col1);

    std::string const image = c2.as_ppm();
    std::string expected{"P3\n"
                         "10 2\n"
                         "255\n"
                         " 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         " 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         " 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         " 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"};
    EXPECT_EQ(image, expected) << image;
}

TEST_F(CanvasTest, CanvasExportPpmTail) { // NOLINT
    Canvas c2{3, 6};
    Color col1{1, 0.8, 0.6};
    c2.fill(col1);

    const std::string image = c2.as_ppm();
    std::string expected{"P3\n"
                         "3 6\n"
                         "255\n"
                         " 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         " 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         " 255 204 153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                         " 255 204 153 255 204 153 255 204 153\n"};
    EXPECT_EQ(image, expected) << image;
}

} // namespace cherry_blazer

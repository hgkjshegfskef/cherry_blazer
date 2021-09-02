#include "color.hh"

#include <gtest/gtest.h>
#include <sstream>
#include <string>

using cherry_blazer::Color;

// Color is default constructable and zero-initialized.
TEST(ColorTest, ColorDefaultConstructableZeroInit) { // NOLINT
    Color c;
    EXPECT_EQ(c, (Color{0, 0, 0}));
}

// scalar*Color
TEST(ColorTest, ScalarTimesColor) { // NOLINT
    Color c1{1, -2, 3};
    double scalar = 3.5; // NOLINT(readability-magic-numbers)
    Color c2 = scalar * c1;
    EXPECT_EQ(c2, (Color{3.5, -7, 10.5}));
}

// Color*scalar
TEST(ColorTest, ColorTimesScalar) { // NOLINT
    Color c1{1, -2, 3};
    double scalar = 3.5; // NOLINT(readability-magic-numbers)
    Color c2 = c1 * scalar;
    EXPECT_EQ(c2, (Color{3.5, -7, 10.5}));
}

// Color/scalar
TEST(ColorTest, ColorDividedByScalar) { // NOLINT
    Color c1{1, -2, 3};
    double scalar = 2;
    Color c2 = c1 / scalar;
    EXPECT_EQ(c2, (Color{0.5, -1, 1.5}));
}

// scalar/Vector (= ERROR)

// Color += Color
TEST(ColorTest, ColorPlusEqualsColor) { // NOLINT
    Color c1{3, -2, 5};
    Color c2{-2, 3, 1};
    c1 += c2;
    EXPECT_EQ(c1, (Color{1, 1, 6}));
}

// Color + Color = Color
TEST(ColorTest, ColorPlusColor) { // NOLINT
    Color c1{3, -2, 5};
    Color c2{-2, 3, 1};
    Color c3 = c1 + c2;
    EXPECT_EQ(c3, (Color{1, 1, 6}));
}

// Color -= Color
TEST(ColorTest, ColorMinusEqualsColor) { // NOLINT
    Color c1{3, 2, 1};
    Color c2{5, 6, 7};
    c1 -= c2;
    EXPECT_EQ(c1, (Color{-2, -4, -6}));
}

// Color - Color = Color
TEST(ColorTest, ColorMinusColor) { // NOLINT
    Color c1{3, 2, 1};
    Color c2{5, 6, 7};
    Color c3 = c1 - c2;
    EXPECT_EQ(c3, (Color{-2, -4, -6}));
}

// Color *= Color
TEST(ColorTest, ColorTimesEqualsColor) { // NOLINT
    Color c1{1, 0.2, 0.4};               // NOLINT(readability-magic-numbers)
    Color c2{0.9, 1, 0.1};               // NOLINT(readability-magic-numbers)
    c1 *= c2;
    EXPECT_EQ(c1, (Color{0.9, 0.2, 0.04}));
}

// Color * Color = Color
TEST(ColorTest, ColorTimesColor) { // NOLINT
    Color c1{1, 0.2, 0.4};         // NOLINT(readability-magic-numbers)
    Color c2{0.9, 1, 0.1};         // NOLINT(readability-magic-numbers)
    Color c3 = c1 * c2;
    EXPECT_EQ(c3, (Color{0.9, 0.2, 0.04}));
}

// Color can be printed out.
TEST(ColorTest, ColorPrintOut) { // NOLINT
    Color c{1, 22, 255};         // NOLINT(readability-magic-numbers)
    std::stringstream ss;
    ss << c;
    EXPECT_EQ(ss.str(), std::string{"(  1  22 255)"});
}

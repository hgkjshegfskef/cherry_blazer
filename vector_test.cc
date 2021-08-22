#pragma clang diagnostic push
// Ignore googletest-related warnings:
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-non-const-global-variables"
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#pragma ide diagnostic ignored "cert-err58-cpp"
// It's ok to use magic numbers in tests:
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

#include <gtest/gtest.h>

#include "vector.hh"

// vector += vector (= vector)
TEST(TupleTest, VectorPlusEqualsVector) {
    Vector v1{3, -2, 5};
    Vector v2{-2, 3, 1};
    v1 += v2;
    EXPECT_EQ(v1, Vector(1, 1, 6));
}

// vector + vector = vector
TEST(TupleTest, VectorPlusVector) {
    Vector v1{3, -2, 5};
    Vector v2{-2, 3, 1};
    auto v3 = v1 + v2;
    EXPECT_EQ(v3, Vector(1, 1, 6));
}

#pragma clang diagnostic pop

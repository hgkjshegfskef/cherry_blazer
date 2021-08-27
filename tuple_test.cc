#pragma clang diagnostic push
// Ignore googletest-related warnings:
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-non-const-global-variables"
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#pragma ide diagnostic ignored "cert-err58-cpp"
// It's ok to use magic numbers in tests:
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

#include <gtest/gtest.h>

#include "tuple.hh"

// Tuple is default constructable and zero-initialized.
TEST(TupleTest, TupleDefaultConstructable) {
    Tuple t;
    EXPECT_EQ(t, Tuple(0, 0, 0));
}

// Tuple += Tuple (= Tuple)
TEST(TupleTest, TuplePlusEqualsTuple) {
    Tuple t1{3, -2, 5};
    Tuple t2{-2, 3, 1};
    t1 += t2;
    EXPECT_EQ(t1, Tuple(1, 1, 6));
}

// Tuple + Tuple = Tuple
TEST(TupleTest, TuplePlusTuple) {
    Tuple t1{3, -2, 5};
    Tuple t2{-2, 3, 1};
    auto t3 = t1 + t2;
    EXPECT_EQ(t3, Tuple(1, 1, 6));
}

#pragma clang diagnostic pop

// * TODO: disallow comparisons of point,vector with tuple, get rid of 4th member?
// * TODO: compile-time test that adding point and tuple is not allowed
// * TODO: compile-time test that adding vector and tuple is not allowed
// * TODO: compile-time test that adding two points is not allowed (including +=)
// * TODO: compile-time test that vector += point is not allowed
// * TODO: compile-time test that point doesn't compare to vector
// * TODO: maybe explicit conversion operator to Tuple?

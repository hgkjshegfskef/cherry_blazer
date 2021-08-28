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

// Tuple has operator==.
TEST(TupleTest, TupleComparedToTupleEquals) {
    Tuple t1{3, -2, 5};
    Tuple t2{3, -2, 5};
    EXPECT_EQ(t1, t2);
}

// Tuple has operator!=.
TEST(TupleTest, TupleComparedToTupleDoesntEqual) {
    Tuple t1{3, -2, 5};
    Tuple t2{-2, 3, 1};
    EXPECT_NE(t1, t2);
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

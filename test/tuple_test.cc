#include <gtest/gtest.h>

#include "tuple.hh"

// Tuple is default constructable and zero-initialized.
TEST(TupleTest, TupleDefaultConstructable) { // NOLINT
    Tuple t;
    EXPECT_EQ(t, Tuple(0, 0, 0));
}

// Tuple += Tuple (= Tuple)
TEST(TupleTest, TuplePlusEqualsTuple) { // NOLINT
    Tuple t1{3, -2, 5};
    Tuple t2{-2, 3, 1};
    t1 += t2;
    EXPECT_EQ(t1, Tuple(1, 1, 6));
}

// Tuple + Tuple = Tuple
TEST(TupleTest, TuplePlusTuple) { // NOLINT
    Tuple t1{3, -2, 5};
    Tuple t2{-2, 3, 1};
    auto t3 = t1 + t2;
    EXPECT_EQ(t3, Tuple(1, 1, 6));
}

// Tuple -= Tuple (= Tuple)
TEST(TupleTest, TupleMinusEqualsTuple) { // NOLINT
    Tuple t1{3, 2, 1};
    Tuple t2{5, 6, 7};
    t1 -= t2;
    EXPECT_EQ(t1, Tuple(-2, -4, -6));
}

// Tuple - Tuple = Tuple
TEST(TupleTest, TupleMinusTuple) { // NOLINT
    Tuple t1{3, 2, 1};
    Tuple t2{5, 6, 7};
    auto t3 = t1 - t2;
    EXPECT_EQ(t3, Tuple(-2, -4, -6));
}

// Tuples can be compared for equality.
TEST(TupleTest, TupleComparedToTupleEquals) { // NOLINT
    Tuple t1{3, -2, 5};
    Tuple t2{3, -2, 5};
    EXPECT_EQ(t1, t2);
}

// Tuples can be compared for inequality.
TEST(TupleTest, TupleComparedToTupleDoesntEqual) { // NOLINT
    Tuple t1{3, -2, 5};
    Tuple t2{-2, 3, 1};
    EXPECT_NE(t1, t2);
}

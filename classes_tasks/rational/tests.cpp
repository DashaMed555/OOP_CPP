#include <gtest/gtest.h>
#include "rational.h"

TEST(RationalTest, BasicOperations) {
    Rational a(5, 3);
    Rational b(3, 10);
    Rational c(1, 2);
    ASSERT_EQ(a * b, c);
    // ASSERT_EQ(a / b, c);
    Rational d(59, 30);
    ASSERT_EQ(a + b, d);
    Rational e(41, 30);
    ASSERT_EQ(a - b, e);
    Rational f(50, 9);
    ASSERT_EQ(a / b, f);
}

TEST(RationalTest, Equality) {
    Rational a(5, 3);
    Rational b(15, 9);
    ASSERT_TRUE(a == b);
    // ASSERT_TRUE(a == (b + a));
    Rational c(9, 8);
    Rational d(63, 56);
    ASSERT_TRUE(c == d);
}

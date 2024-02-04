#ifndef RATIONAL_RATIONAL_H
#define RATIONAL_RATIONAL_H

#include <iostream>

class Rational {
public:
    Rational();
    Rational(int numerator, int denominator);

    [[nodiscard]]
    int Numerator() const;
    [[nodiscard]]
    int Denominator() const;

    friend Rational operator+(const Rational& a, const Rational& b);
    friend Rational operator*(const Rational& a, const Rational& b);
    friend Rational operator/(const Rational& a, const Rational& b);
    friend Rational operator-(const Rational& a, const Rational& b);
    friend bool operator==(const Rational& a, const Rational& b);
    friend std::ostream &operator<<(std::ostream& out, const Rational& a);
    friend std::istream &operator>>(std::istream& in, Rational& a);

private:
    int num, denom;
    static int nod(int a, int b);
    static int nok(int a, int b);
    static void swap(int& a, int& b);
};

#endif

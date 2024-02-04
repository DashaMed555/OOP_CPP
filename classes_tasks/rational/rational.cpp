#include "rational.h"
#include <sstream>

Rational::Rational() {
    num = 1;
    denom = 1;
}
Rational::Rational(int numerator, int denominator) {
    if (denominator == 0) {
        std::stringstream ss;
        ss << "Argument must not be zero";
        throw std::invalid_argument(ss.str());
    }
    num = numerator;
    denom = denominator;
}

[[nodiscard]]
int Rational::Numerator() const {
    return this->num;
}
[[nodiscard]]
int Rational::Denominator() const {
    return this->denom;
}

Rational operator+(const Rational& a, const Rational& b) {
    int nod_ = Rational::nod(a.denom, b.denom);
    int new_num = a.num * (nod_ / a.denom) + b.num * (nod_ / b.denom);
    int nok_ = Rational::nok(new_num, nod_);
    Rational c(new_num / nok_, nod_ / nok_);
    return c;
}
Rational operator*(const Rational& a, const Rational& b) {
    int new_num = a.num * b.num;
    int new_denom = a.denom * b.denom;
    int nok_ = Rational::nok(new_num, new_denom);
    Rational c(new_num / nok_, new_denom / nok_);
    return c;
}
Rational operator/(const Rational& a, const Rational& b) {
    Rational c(b.num, b.denom);
    Rational::swap(c.num, c.denom);
    c = a * c;
    return c;
}
Rational operator-(const Rational& a, const Rational& b) {
    Rational c(-b.num, b.denom);
    c = a + c;
    return c;
}
bool operator==(const Rational& a, const Rational& b) {
    int nod_ = Rational::nod(a.denom, b.denom);
    int new_num_a = a.num * (nod_ / a.denom);
    int new_num_b = b.num * (nod_ / b.denom);
    return new_num_a == new_num_b;
}
std::ostream &operator<<(std::ostream& out, const Rational& a) {
    out << a.num << '/' << a.denom << std::endl;
    return out;
}
std::istream &operator>>(std::istream& in, Rational& a) {
    in >> a.num;
    in.ignore(1);
    in >> a.denom;
    return in;
}

int Rational::nod(const int a, const int b) {
    int nod_;
    for (nod_ = std::min(a, b); nod_ <= a * b; ++nod_)
        if (nod_ % a == 0 && nod_ % b == 0)
            break;
    return nod_;
}
int Rational::nok(const int a, const int b) {
    int nok_;
    for (nok_ = std::min(a, b); nok_ > 0; --nok_)
        if (a % nok_ == 0 && b % nok_ == 0)
            break;
    return nok_;
}
void Rational::swap(int& a, int& b) {
    int c = a;
    a = b;
    b = c;
}

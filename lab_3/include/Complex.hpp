#ifndef COMPLEX
#define COMPLEX

#include <iostream>
#include "Errors.hpp"

class Complex {
    private:
        double Re;
        double Im;
    public:
        Complex() : Re(0.0), Im(0.0) {}
        Complex(double re, double im) : Re(re), Im(im) {}

        double GetRe() const { return Re; }
        double GetIm() const { return Im; }
};

std::ostream& operator<< (std::ostream& os, const Complex& number) {
    os << number.GetRe();
    if (number.GetIm() >= 0) os << "+";
    os << number.GetIm() << "i";
    return os;
}

std::istream& operator>> (std::istream& is, Complex& number) {
    double re, im;
    char sign, i;
    is >> re >> sign >> im >> i;

    if (i != 'i' || (sign != '+' && sign != '-')) throwError(INVALID_ARGUMENT);

    number = Complex(re, (sign == '-') ? -im : im);
    return is;
}

inline bool operator==(const Complex& lhs, const Complex& rhs)
{
    return lhs.GetRe() == rhs.GetRe() && lhs.GetIm() == rhs.GetIm();
}

inline bool operator!=(const Complex& lhs, const Complex& rhs)
{
    return !(lhs == rhs);
}
#endif
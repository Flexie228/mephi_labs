#ifndef COMPLEX
#define COMPLEX

#include <iostream>
#include <limits>
#include "Errors.hpp"

class Complex {
    private:
        double Re;
        double Im;
    public:
        Complex() : Re(0.0), Im(0.0) {}
        Complex(double re, double im) : Re(re), Im(im) {}

        bool operator<(const Complex& other) const { return (Re*Re + Im*Im) < (other.Re*other.Re + other.Im*other.Im); }
        bool operator>(const Complex& other) const { return (Re*Re + Im*Im) > (other.Re*other.Re + other.Im*other.Im); }

        double GetRe() const { return Re; }
        double GetIm() const { return Im; }
};

inline Complex operator+ (const Complex& lhs, const Complex& rhs)
{
    return Complex(lhs.GetRe() + rhs.GetRe(), lhs.GetIm() + rhs.GetIm());
}

inline Complex operator* (const Complex& lhs, const Complex& rhs)
{
    double re = lhs.GetRe() * rhs.GetRe() - lhs.GetIm() * rhs.GetIm();
    double im = lhs.GetRe() * rhs.GetIm() + lhs.GetIm() * rhs.GetRe();
    return Complex(re, im);
}

inline std::ostream& operator<< (std::ostream& os, const Complex& number)
{
    os << number.GetRe();
    if (number.GetIm() >= 0) os << "+";
    os << number.GetIm() << "i";
    return os;
}

inline std::istream& operator>> (std::istream& is, Complex& number) {
    double re = 0.0, im = 0.0;
    char sign = '+', i = 'i';
    
    is >> re;
    if (is.peek() == '+' || is.peek() == '-') {
        is >> sign;
    }
    is >> im >> i;
    
    if (i != 'i') {
        is.setstate(std::ios::failbit);
        return is;
    }
    
    number = Complex(re, (sign == '-') ? -im : im);
    return is;
}

inline bool operator== (const Complex& lhs, const Complex& rhs)
{
    return lhs.GetRe() == rhs.GetRe() && lhs.GetIm() == rhs.GetIm();
}

inline bool operator!= (const Complex& lhs, const Complex& rhs)
{
    return !(lhs == rhs);
}

namespace std {
    template<>
    struct numeric_limits<Complex> {
        static Complex min() { return Complex(-1e100, -1e100); }
        static Complex max() { return Complex(1e100, 1e100); }
    };
}

#endif
#ifndef PHONE
#define PHONE

#include <iostream>
#include <string>
#include <stdexcept>
#include "Errors.hpp"

class Phone {
private:
    std::string number;
    void validate(const std::string& num) const
    {
        if (num.length() != 11 || 
            num.substr(0, 2) != "89" || 
            num.find_first_not_of("0123456789") != std::string::npos) {
            throwError(INVALID_ARGUMENT);
        }
    }

public:
    Phone() : number("89000000000") {}
    Phone(const std::string& num) { SetNumber(num); }
    std::string GetNumber() const { return number; }
    void SetNumber(const std::string& num)
    {
        validate(num);
        number = num;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Phone& p)
{
    os << p.GetNumber();
    return os;
}

inline std::istream& operator>>(std::istream& is, Phone& p)
{
    std::string num;
    if (!(is >> num)) throwError(INVALID_ARGUMENT);
    p.SetNumber(num);
    return is;
}

inline bool operator==(const Phone& lhs, const Phone& rhs)
{
    return lhs.GetNumber() == rhs.GetNumber();
}

inline bool operator!=(const Phone& lhs, const Phone& rhs)
{
    return !(lhs == rhs);
}

#endif
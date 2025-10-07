#ifndef STUDENT
#define STUDENT

#include <iostream>
#include <string>
#include "Phone.hpp"
#include "Errors.hpp"

class Student {
private:
    std::string surname;
    std::string name;
    Phone phone;
    size_t group;

public:
    Student() : surname(""), name(""), group(0) {} 
    Student(const std::string& surname, const std::string& name,const Phone& phone, size_t group)
        : surname(surname), name(name), group(group) {SetPhone(phone);}

    std::string GetSurname() const { return surname; }
    std::string GetName() const { return name; }
    Phone GetPhone() const { return phone; }
    size_t GetGroup() const { return group; }

    void SetPhone(const Phone& p) { phone = p; }
    void SetGroup(size_t g) { group = g; }
};

std::ostream& operator<<(std::ostream& os, const Student& s)
{
    os << s.GetName() << " " << s.GetSurname() 
       << ", phone: " << s.GetPhone().GetNumber() 
       << ", group: " << s.GetGroup();
    return os;
}

std::istream& operator>>(std::istream& is, Student& s)
{
    std::string name, surname, phoneStr;
    size_t group;
    
    if (!(is >> name >> surname >> phoneStr >> group)) throwError(INVALID_ARGUMENT);

    Phone phone;
    phone.SetNumber(phoneStr);
    
    s = Student(surname, name, phone, group);
    return is;
}

inline bool operator==(const Student& lhs, const Student& rhs)
{
    return lhs.GetSurname() == rhs.GetSurname() &&
           lhs.GetName() == rhs.GetName() &&
           lhs.GetPhone() == rhs.GetPhone() &&
           lhs.GetGroup() == rhs.GetGroup();
}

inline bool operator!=(const Student& lhs, const Student& rhs)
{
    return !(lhs == rhs);
}


#endif
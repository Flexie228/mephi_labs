#ifndef STUDENT
#define STUDENT

#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <cctype>
using namespace std;

struct Phone {
    std::string number; // формат: "*79*******"

    Phone() = default;
    explicit Phone(std::string  num) : number(std::move(num)) {}

    static Phone generate() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 9);

        std::string num = "*79";

        for (int i = 0; i < 8; i++) {
            num += std::to_string(dist(gen));
        }

        return Phone(num);
    }

    friend std::ostream& operator<<(std::ostream& os, const Phone& phone) {
        if (phone.number.length() == 11) {
            os << "+7-9" << phone.number[2] << phone.number[3]
               << "-" << phone.number[4] << phone.number[5] << phone.number[6]
               << "-" << phone.number[7] << phone.number[8]
               << "-" << phone.number[9] << phone.number[10];
        } else {
            os << phone.number;
        }
        return os;
    }
};

struct Student {
    string name;
    string surname;
    size_t age{0};
    Phone phone;
    string id;

    Student() = default;
    Student(string  n, string  s, const size_t a, Phone  ph, string id) : name(std::move(n)), surname(std::move(s)), age(a), phone(std::move(ph)), id(std::move(id)) {}

    bool operator== (const Student& other) const {
        return id == other.id;
    }

    bool operator< (const Student& other) const {
        return id < other.id;
    }

    static string generatePrefix() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> char_type_dist(0, 1); // 0 - цифра, 1 - буква
        std::uniform_int_distribution<int> digit_dist(0, 9);
        std::uniform_int_distribution<int> letter_dist(0, 25);

        const string letters = "abcdefghijklmnopqrstuvwxyz";
        string prefix;

        for (int i = 0; i < 5; ++i) {
            if (char_type_dist(gen) == 0) {
                // Цифра
                prefix += std::to_string(digit_dist(gen));
            } else {
                char letter = letters[letter_dist(gen)];
                if (std::uniform_int_distribution<int>(0, 1)(gen) == 0) {
                    letter = static_cast<char>(std::toupper(letter));
                }
                prefix += letter;
            }
        }

        return prefix;
    }

    static Student generate() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> age_dist(14, 80);
        std::uniform_int_distribution<int> digit_dist(0, 9);

        // Базовые имена и фамилии
        const std::vector<std::string> firstNames = {"Ivan", "Stepan", "Alexey", "Kirill", "Dmitry", "Timofey", "Vladimir", "Mihail", "Sergey", "Alexander", "Egor", "Anatoliy", "Zahar"};
        const std::vector<std::string> lastNames = {"Ivanov", "Petrov", "Sidorov", "Smirnov", "Kuznetsov", "Popov", "Zargebin", "Malinin", "Fomichev", "Bukalin", "Afanasiev", "Heineken", "Carlsberg"};

        std::uniform_int_distribution<size_t> name_dist(0, firstNames.size() - 1);

        Student student;

        student.name = firstNames[name_dist(gen)];

        const string surnamePrefix = generatePrefix();
        student.surname = surnamePrefix + "_" + lastNames[name_dist(gen)];

        student.age = age_dist(gen);
        student.phone = Phone::generate();

        std::string id;
        for (int i = 0; i < 15; ++i) {
            id += std::to_string(digit_dist(gen));
        }
        student.id = id;

        return student;
    }

    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os << "Student: " << student.name << " " << student.surname
           << ", Age: " << student.age
           << ", Phone: " << student.phone
           << ", ID: " << student.id;
        return os;
    }
};

#endif
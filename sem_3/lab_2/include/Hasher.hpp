#ifndef HASHER
#define HASHER

#include <string>
#include <cstring>
#include <cmath>

template<typename Key>
class Hasher;

template<>
class Hasher<std::string> {
public:
    size_t operator()(const std::string& str) const {
        size_t hash = 2166136261u;
        for (const char c : str) {
            hash ^= static_cast<unsigned char>(c);
            hash *= 16777619u;
        }
        return hash;
    }
};

template<>
class Hasher<int> {
public:
    size_t operator()(const int num) const {
        return static_cast<size_t>(num);
    }
};

template<>
class Hasher<double> {
public:
    size_t operator()(const double num) const {
        if (std::isnan(num) || num == 0.0) return 0;

        int exp;
        const double mantissa = std::frexp(num, &exp);
        size_t mantissaBits;
        std::memcpy(&mantissaBits, &mantissa, sizeof(mantissa));

        return (mantissaBits ^ exp);
    }
};

#endif
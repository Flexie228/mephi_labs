#ifndef PAIR_WORKS
#define PAIR_WORKS

#include <functional>
#include <utility>
using namespace std;

struct PairHash {
    size_t operator()(const pair<size_t, size_t>& p) const {
        auto [a, b] = p;
        if (a > b) swap(a, b); // Одинаковый хеш для любой ориентации
        return hash<size_t>()(a) ^ (hash<size_t>()(b) << 1);
    }
};

// Компаратор для пар (чтобы (1,2) и (2,1) считались равными в неориентированном графе)
struct PairEqual {
    bool operator()(const pair<size_t, size_t>& a, const pair<size_t, size_t>& b) const {
        return (a.first == b.first && a.second == b.second) ||
               (a.first == b.second && a.second == b.first);
    }
};

#endif
#ifndef HASHTABLE
#define HASHTABLE

#include <vector>
#include <list>
#include <utility>
#include "Exceptions.hpp"
#include "Hasher.hpp"
using namespace std;

template<typename Key, typename Value>
class HashTable {
private:
    struct Pair {
        Key first;
        Value second;
        size_t release{0};

        Pair() = default;
        Pair(const Key& k, const Value& v, const size_t r = 0) : first(k), second(v), release(r) {}
        Pair(const Pair&) = default;
        Pair(Pair&&) = default;
        Pair& operator= (const Pair&) = default;
        Pair& operator= (Pair&&) = default;
        ~Pair() = default;
    };
    static constexpr float LoadFactor{0.75f};
    static constexpr size_t capacities[] = {53, 97, 193, 389, 769, 1543, 3079, 6151,                                    // Массив простых чисел, с "удвоением" размера
                                            12289, 24593, 49157, 98317, 196613, 393241,
                                            786433, 1572869, 3145739, 6291469, 12582917,
                                            25165843, 50331653, 100663319, 201326611,
                                            402653189, 805306457, 1610612741};
    size_t capacityIndex{0};
    size_t size{0};
    size_t capacity{capacities[capacityIndex]};
    vector<list<Pair>> items;

public:
    HashTable() : items(capacity) {}
    HashTable(const HashTable& other) : capacityIndex(other.capacityIndex), size(other.size), capacity(other.capacity), items(other.items) {}
    explicit HashTable(const std::vector<std::pair<Key, Value>>& dataBase) {
        const size_t expectedSize = dataBase.size();
        constexpr size_t capacitiesSize = std::size(capacities);
        capacityIndex = 0;

        for (; capacityIndex < capacitiesSize; capacityIndex++) {
            if (static_cast<float>(expectedSize) < static_cast<float>(capacities[capacityIndex]) * LoadFactor)
                break;
        }
        if (capacityIndex >= capacitiesSize) throwError(CAPACITY_MAXED);

        capacity = capacities[capacityIndex];
        items.resize(capacity);

        Hasher<Key> hasher;
        for (const auto& pair : dataBase) {
            insert(pair.first, pair.second);
        }
    }
    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            capacityIndex = other.capacityIndex;
            size = other.size;
            capacity = other.capacity;
            items = other.items;
        }
        return *this;
    }
    ~HashTable() = default;

    // Получение информации о хеш-таблице
    [[nodiscard]] size_t getSize() const { return size; }
    [[nodiscard]] size_t getCapacity() const { return capacity; }
    [[nodiscard]] static float getLoadFactor() { return LoadFactor; }
    [[nodiscard]] bool isEmpty() const { return (size == 0); }

    vector<Value> find(const Key& key) const {
        vector<Value> result;
        size_t index = getItemIndex(key);
        for (const auto& pair : items[index]) {
            if (pair.first == key) {
                result.push_back(pair.second);
                if (pair.release == 0) break;
            }
        }
        return result;
    }
    void insert(const Key& key, const Value& value) {
        size_t index = getItemIndex(key);
        size_t rel = 0;

        for (auto& pair : items[index]) {
            if (pair.first == key) {
                rel = pair.release + 1;
                break;
            }
        }
        items[index].push_front({key, value, rel});
        size++;
        rehash();
    }
    void erase(const Key& key) {
        size_t index = getItemIndex(key);
        auto& list = items[index];

        for (auto it = list.begin(); it != list.end(); ) {
            if (it->first == key) {
                const bool isLast = (it->release == 0);
                it = list.erase(it);
                size--;
                if (isLast) break;
            } else { ++it; }
        }
    }
    void erase(const Key& key, const Value& value) {
        size_t index = getItemIndex(key);
        auto& list = items[index];

        for (auto it = list.begin(); it != list.end(); ) {
            if (it->first == key && it->second == value) {
                const bool lastRelease = (it->release == 0);
                it = list.erase(it);
                size--;
                if (lastRelease) return;
            } else { ++it; }
        }
    }
    vector<Value> operator[](const Key& key) {
        return find(key);
    }

    bool contains(const Key& key) const {
        size_t index = getItemIndex(key);
        for (const auto& pair : items[index]) {
            if (pair.first == key) {
                return true;
            }
        }
        return false;
    }

    void clear() {
        vector<list<Pair>> newItems(capacity);
        capacityIndex = 0;
        capacity = capacities[capacityIndex];
        items = std::move(newItems);
        size = 0;
    }

private:
    size_t getItemIndex(const Key& key) const {
        Hasher<Key> hasher;
        const size_t hash = hasher(key);
        return hash % capacity;
    }
    void rehash() {
        if (static_cast<float>(size)/static_cast<float>(capacity) < LoadFactor) return;

        size_t newCapacity;
        if (capacityIndex + 1 < std::size(capacities)) {
            capacityIndex++;
            newCapacity = capacities[capacityIndex];
        } else {
            throwError(CAPACITY_MAXED);
        }

        vector<list<Pair>> newItems(newCapacity);
        Hasher<Key> hasher;

        for (auto& list : items) {
            for (auto it = list.rbegin(); it != list.rend(); ++it) {
                auto& pair = *it;
                size_t newIndex = hasher(pair.first) % newCapacity;
                newItems[newIndex].push_front(std::move(pair));
            }
        }

        items = std::move(newItems);
        capacity = newCapacity;
    }
};

#endif

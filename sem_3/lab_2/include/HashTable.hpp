#ifndef HASHTABLE
#define HASHTABLE

#include <vector>
#include <list>
#include <utility>
#include "Exceptions.hpp"
#include "Hasher.hpp"

template<typename Key, typename Value>
class HashTable {
private:
    static constexpr float LoadFactor{0.75f};
    static constexpr size_t capacities[] = {53, 97, 193, 389, 769, 1543, 3079, 6151,                                    // Массив простых чисел, с "удвоением" размера
                                            12289, 24593, 49157, 98317, 196613, 393241,
                                            786433, 1572869, 3145739, 6291469, 12582917,
                                            25165843, 50331653, 100663319, 201326611,
                                            402653189, 805306457, 1610612741};
    size_t capacityIndex{0};
    size_t size{0};
    size_t capacity{capacities[capacityIndex]};
    std::vector<std::list<std::pair<Key, Value>>> items;

public:
    HashTable() : items(capacity) {}
    HashTable(const HashTable& other) : capacityIndex(other.capacityIndex), size(other.size), capacity(other.capacity), items(other.items) {}
    explicit HashTable(const std::vector<std::pair<Key, Value>> dataBase) {
        size = dataBase.size();
        constexpr size_t capacitiesSize = std::size(capacities);
        capacityIndex = 0;
        for (; capacityIndex < capacitiesSize; capacityIndex++) {
            if (static_cast<float>(size) < static_cast<float>(capacities[capacityIndex]) * LoadFactor) break;
        }
        if (capacityIndex >= capacitiesSize) throwError(CAPACITY_MAXED);

        capacity = capacities[capacityIndex];
        items.resize(capacity);

        Hasher<Key> hasher;
        for (size_t i = 0; i < size; i++) {
            size_t index = hasher(dataBase[i].first) % capacity;
            items[index].push_back(dataBase[i]);
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

    const Value* find(const Key& key) const {
        size_t index = getItemIndex(key);
        for (const auto& pair : items[index]) {
            if (pair.first == key) return &(pair.second);
        }
        return nullptr;
    }
    void insert(const Key& key, const Value& value) {
        size_t index = getItemIndex(key);

        for (auto& pair : items[index]) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }
        items[index].push_front({key, value});
        size++;
        rehash();
    }
    void erase(const Key& key) {
        size_t index = getItemIndex(key);
        auto& list = items[index];

        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->first == key) {
                list.erase(it);
                size--;
                return;
            }
        }
    }
    Value& operator[](const Key& key) {
        size_t index = getItemIndex(key);
        const size_t currentCapacity = capacity;
        for (auto& pair : items[index]) {
            if (pair.first == key) return pair.second;
        }
        items[index].push_front({key, Value{}});
        rehash();
        size++;

        if (static_cast<float>(size) / static_cast<float>(currentCapacity) >= LoadFactor) {                             //Если произойдет рехеширование, то обновляем индекс
            const size_t newIndex = getItemIndex(key);
            index = newIndex;
        }
        Value& result = items[index].front().second;
        return result;
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

        std::vector<std::list<std::pair<Key, Value>>> newItems(newCapacity);
        Hasher<Key> hasher;

        for (auto& list : items) {
            for (auto& pair : list) {
                size_t newIndex = hasher(pair.first) % newCapacity;
                newItems[newIndex].push_front(std::move(pair));
            }
        }

        items = std::move(newItems);
        capacity = newCapacity;
    }
};

#endif

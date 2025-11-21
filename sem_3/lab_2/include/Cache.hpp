#ifndef CACHE
#define CACHE

#include "IDictionary.hpp"
using namespace std;

template<typename Key, typename Value>
class Cache {
private:
    size_t maxSize;  // Максимальный размер кэша (для списка)
    list<pair<Key, Value>> cacheList;  // Двусвязный список для порядка использования
    IDictionaryHT<Key, typename list<pair<Key, Value>>::iterator> cacheMap;  // Хранит итераторы на элементы списка

public:
    explicit Cache(const size_t size) : maxSize(size), cacheMap() {}

    // Получить значение по ключу (обновляет позицию в LRU)
    vector<Value> Get(const Key& key) {
        auto iterators = cacheMap.Get(key);
        vector<Value> result;

        if (iterators.empty()) return result;

        for (auto listIter : iterators) {
            result.push_back(listIter->second);
            cacheList.splice(cacheList.begin(), cacheList, listIter);
        }
        return result;
    }

    void Put(const Key& key, const Value& value) {
        auto iterators = cacheMap.Get(key);
        if (iterators.empty()) {
            if (cacheList.size() == maxSize) {
                auto last = prev(cacheList.end());
                cacheMap.Remove(last->first);
                cacheList.pop_back();
            }
            cacheList.push_front({key, value});
            cacheMap.Add(key, cacheList.begin());
        } else {
            auto listIter = iterators[0];                       // Только первый
            listIter->second = value;
            cacheList.splice(cacheList.begin(), cacheList, listIter);
        }
    }

    void Remove(const Key& key) {                                                       // Удаляем все элементы с одинаковым ключом
        auto iterators = cacheMap.Get(key);
        if (iterators.empty()) return;

        for (auto listIter : iterators) {
            cacheList.erase(listIter);
        }

        cacheMap.Remove(key);
    }

    void Clear() { cacheList.clear(); cacheMap.Clear(); }

    bool ContainsKey(const Key& key) const { return cacheMap.ContainsKey(key); }
    [[nodiscard]] size_t Size() const { return cacheList.size(); }
    [[nodiscard]] size_t GetMaxSize() const { return maxSize; }

    vector<pair<Key, Value>> getAll() const {
        vector<pair<Key, Value>> result;
        for (const auto& pair : cacheList) {
            result.push_back(pair);
        }
        return result;
    }
};

#endif
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
    Value* Get(const Key& key) {
        auto* listIterPtr = cacheMap.Get(key);  // указатель на итератор
        if (!listIterPtr) return nullptr;
        auto listIter = *listIterPtr;
        cacheList.splice(cacheList.begin(), cacheList, listIter);
        return &(listIter->second);
    }

    void Put(const Key& key, const Value& value) {
        auto* listIterPtr = cacheMap.Get(key);  // указатель на итератор
        if (!listIterPtr) {
            if (cacheList.size() == maxSize) {
                auto last = cacheList.end();
                --last;
                cacheMap.Remove(last->first);
                cacheList.pop_back();
            }
            cacheList.push_front({key, value});
            cacheMap.Add(key, cacheList.begin());
            return;
        }
        auto listIter = *listIterPtr;
        listIter->second = value;
        cacheList.splice(cacheList.begin(), cacheList, listIter);
    }

    void Remove(const Key& key) {
        auto* listIterPtr = cacheMap.Get(key);
        if (!listIterPtr) return;
        auto listIter = *listIterPtr;
        cacheList.erase(listIter);  // Удаляем из списка
        cacheMap.Remove(key);  // Удаляем из словаря
    }

    void Clear() { cacheList.clear(); cacheMap.Clear(); }

    bool ContainsKey(const Key& key) const { return cacheMap.ContainsKey(key); }
    [[nodiscard]] size_t Size() const { return cacheList.size(); }
    [[nodiscard]] size_t GetMaxSize() const { return maxSize; }
};

#endif
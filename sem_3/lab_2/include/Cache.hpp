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
    

    void Put(const Key& key, const Value& value) {
        if (cacheMap.ContainsKey(key)) {
            auto listIter = cacheMap.Get(key);
            listIter->second = value;
            cacheList.splice(cacheList.begin(), cacheList, listIter);  // Перемещаем в начало
            return;
        }

        if (cacheList.size() >= maxSize) {
            auto last = cacheList.end();
            --last;
            cacheMap.Remove(last->first);
            cacheList.pop_back();
        }

        cacheList.push_front({key, value});
        cacheMap.Add(key, cacheList.begin());
    }

    void Remove(const Key& key) {
        if (cacheMap.ContainsKey(key)) {
            auto listIter = cacheMap.Get(key);
            cacheList.erase(listIter);  // Удаляем из списка
            cacheMap.Remove(key);  // Удаляем из словаря
        }
    }

    void Clear() { cacheList.clear(); cacheMap.Clear(); }

    bool Contains(const Key& key) const { return cacheMap.ContainsKey(key); }
    size_t Size() const { return cacheList.size(); }
    size_t GetMaxSize() const { return maxSize; }
};






#endif
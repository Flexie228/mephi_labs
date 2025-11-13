#ifndef IDICTIONARY
#define IDICTIONARY

#include "HashTable.hpp"
#include "AVLTree.hpp"
#include "Exceptions.hpp"
using namespace std;

template<typename Key, typename Value>
class IDictionary {
public:
    virtual ~IDictionary() = default;

    virtual Value Get(const Key& key) const = 0;
    [[nodiscard]] virtual bool ContainsKey(const Key& key) const = 0;
    virtual void Add(const Key& key, const Value& value) = 0;
    virtual void Remove(const Key& key) = 0;
    virtual void Clear() = 0;

    [[nodiscard]] virtual size_t GetCount() const = 0;
    [[nodiscard]] virtual size_t GetCapacity() const = 0;
    [[nodiscard]] virtual bool IsEmpty() const = 0;
};

template<typename Key, typename Value>
class IDictionaryHT final : public IDictionary<Key, Value> {
private:
    HashTable<Key, Value> hashTable;

public:
    IDictionaryHT() : hashTable() {}
    explicit IDictionaryHT(const vector<pair<Key, Value>> dataBase) : hashTable(dataBase) {}
    ~IDictionaryHT() override = default;

    Value Get(const Key& key) const override {
        Value* v = hashTable.find(key);
        if (v == nullptr) throwError(ELEMENT_NOT_FOUND);
        return *v;
    }
    bool ContainsKey(const Key& key) const override { return hashTable.find(key); }
    void Add(const Key& key, const Value& value) override { hashTable.insert(key, value); }
    void Remove(const Key& key) override { hashTable.erase(key); }
    void Clear() override { hashTable.clear(); }

    size_t GetCount() const override { return hashTable.getSize(); }
    size_t GetCapacity() const override { return hashTable.getCapacity(); }
    bool IsEmpty() const override { return hashTable.isEmpty(); }
};



#endif
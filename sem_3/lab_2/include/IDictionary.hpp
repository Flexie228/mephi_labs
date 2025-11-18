#ifndef IDICTIONARY
#define IDICTIONARY

#include "HashTable.hpp"
#include "AVLTree.hpp"
using namespace std;

template<typename Key, typename Value>
class IDictionary {
public:
    virtual ~IDictionary() = default;

    virtual bool ContainsKey(const Key& key) const = 0;
    virtual void Add(const Key& key, const Value& value) = 0;
    virtual void Remove(const Key& key) = 0;
    virtual void Remove(const Key& key, const Value& value) = 0;
    virtual void Clear() = 0;

    [[nodiscard]] virtual size_t GetCount() const = 0;
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

    vector<Value> Get(const Key& key) const { return hashTable.find(key); }
    bool ContainsKey(const Key& key) const override { return !hashTable.find(key).empty(); }
    void Add(const Key& key, const Value& value) override { hashTable.insert(key, value); }
    void Remove(const Key& key) override { hashTable.erase(key); }
    void Remove(const Key& key, const Value& value) override { hashTable.erase(key, value); }
    void Clear() override { hashTable.clear(); }

    [[nodiscard]] size_t GetCount() const override { return hashTable.getSize(); }
    [[nodiscard]] size_t GetCapacity() const { return hashTable.getCapacity(); }
    [[nodiscard]] bool IsEmpty() const override { return hashTable.isEmpty(); }
};

template<typename Key, typename Value>
class IDictionaryAVL final : public IDictionary<Key, Value> {
private:
    AVLTree<Key, Value> searchTree;

public:
    IDictionaryAVL() : searchTree() {}
    explicit IDictionaryAVL(const vector<pair<Key, Value>> dataBase) : searchTree(dataBase) {}
    ~IDictionaryAVL() override = default;

    const list<Value>* Get(const Key& key) const { return searchTree.find(key); }
    vector<Value> GetRange(const Key& minKey, const Key& maxKey) const { return searchTree.rangedSearch(minKey, maxKey); }
    bool ContainsKey(const Key& key) const override {return searchTree.find(key) != nullptr; }
    void Add(const Key& key, const Value& value) override { searchTree.insert(key, value); }
    void Remove(const Key& key) override { searchTree.removeAll(key); }
    void Remove(const Key& key, const Value& value) override { searchTree.removeOne(key, value); }
    void Clear() override { searchTree.clear(); }


    [[nodiscard]] size_t GetCount() const override { return searchTree.getValuesCount(); }
    [[nodiscard]] size_t GetUniqueKeysCount() const { return searchTree.getTreeSize(); }
    [[nodiscard]] size_t GetKeyDuplicatesCount(const Key& key) const { return searchTree.getListSizeByKey(key); }
    [[nodiscard]] bool IsEmpty() const override { return searchTree.isEmpty(); }
};

#endif
#ifndef AVLTREE
#define AVLTREE

#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <algorithm>
using namespace std;

template<typename Key, typename Value>
class AVLTree {
private:
    struct Node {
        Key key;
        list<Value> values;
        unique_ptr<Node> left{nullptr}, right{nullptr};
        size_t height{1};
        size_t listSize{1};

        Node(const Key& k, const Value& v) : key(k) { values.push_back(v); }
    };

    unique_ptr<Node> root{nullptr};
    size_t treeSize{0};      // Количество узлов (уникальных ключей)
    size_t valuesCount{0};   // Общее количество всех значений

    int getHeight(const unique_ptr<Node>& node) const { return node ? node->height : 0; }
    int getBalance(const unique_ptr<Node>& node) const { return node ? (getHeight(node->left) - getHeight(node->right)) : 0; }
    void updateHeight(const unique_ptr<Node>& node) {
        if (node) node->height = max(getHeight(node->left), getHeight(node->right)) + 1 ;
    }

    unique_ptr<Node> rotateRight(unique_ptr<Node> y) {
        auto x = move(y->left);
        y->left = move(x->right);
        x->right = move(y);

        updateHeight(x->right);
        updateHeight(x);

        return x;
    }

    unique_ptr<Node> rotateLeft(unique_ptr<Node> x) {
        auto y = move(x->right);
        x->right = move(y->left);
        y->left = move(x);

        updateHeight(y->left);
        updateHeight(y);

        return y;
    }

    unique_ptr<Node> balance(unique_ptr<Node> node) {
        updateHeight(node);
        const int balanceFactor = getBalance(node);

        if (balanceFactor > 1 && getBalance(node->left) >= 0)
            return rotateRight(move(node));

        if (balanceFactor < -1 && getBalance(node->right) <= 0)
            return rotateLeft(move(node));

        if (balanceFactor > 1 && getBalance(node->left) < 0) {      //LR ROTATE
            node->left = rotateLeft(move(node->left));
            return rotateRight(move(node));
        }

        if (balanceFactor < -1 && getBalance(node->right) > 0) {     //RL ROTATE
            node->right = rotateRight(move(node->right));
            return rotateLeft(move(node));
        }
        return node;
    }

    unique_ptr<Node> insert(unique_ptr<Node> node, const Key& key, const Value& value) { //функция принимает и возвращает root-узел
        if (!node) {
            treeSize++;
            valuesCount++;
            return make_unique<Node>(key, value);
        }

        if (key < node->key) {
            node->left = insert(move(node->left), key, value);
        } else if (key > node->key) {
            node->right = insert(move(node->right), key, value);
        } else {
            // Ключ уже существует ==> добавляем значение в список
            node->values.push_back(value);
            node->listSize = node->values.size();
            valuesCount++;
            return node;
        }

        return balance(move(node));
    }

    static Node* findMinNode(Node* node) {
        while (node && node->left) {
            node = node->left.get();
        }
        return node;
    }

    unique_ptr<Node> removeAll(unique_ptr<Node> node, const Key& key) {
        if (!node) return nullptr;

        if (key < node->key) {
            node->left = removeAll(move(node->left), key);
        } else if (key > node->key) {
            node->right = removeAll(move(node->right), key);
        } else {
            valuesCount -= node->listSize;
            if (!node->left || !node->right) {                                   // 0-1 Ребенка
                treeSize--;
                return move(node->left ? node->left : node->right);
            } else {
                Node* minNode = findMinNode(node->right.get());

                node->key = minNode->key;
                node->values = move(minNode->values);
                node->listSize = minNode->listSize;
                minNode->listSize = 0;                                           // Зануляем, чтобы не вычесть из valuesCount еще раз

                node->right = removeAll(move(node->right), minNode->key);      // Удаляем узел, данные из которого теперь там, где "удалили"
            }
        }

        return balance(move(node));
    }

    unique_ptr<Node> removeOne(unique_ptr<Node> node, const Key& key, const Value& value) {
        if (!node) return nullptr;

        if (key < node->key) {
            node->left = removeOne(move(node->left), key, value);
        } else if (key > node->key) {
            node->right = removeOne(move(node->right), key, value);
        } else {
            // Удаляем одно значение из списка
            auto it = node->values.begin();
            while (it != node->values.end()) {
                if (*it == value) {
                    node->values.erase(it);
                    node->listSize = node->values.size();
                    valuesCount--;
                    break;
                }
                ++it;
            }

            // Если список значений оказывает пустым после удаления одного элемента - удаляем узел
            if (node->values.empty()) {
                if (!node->left || !node->right) {
                    treeSize--;
                    return move(node->left ? node->left : node->right);
                } else {
                    Node* minNode = findMinNode(node->right.get());

                    node->key = minNode->key;
                    node->values = move(minNode->values);
                    node->listSize = minNode->listSize;
                    minNode->listSize = 0;

                    node->right = removeAll(move(node->right), minNode->key);
                }
            }
        }

        return balance(move(node));
    }

    Node* findNode(const Key& key) const {
        Node* current = root.get();
        while (current) {
            if (key < current->key) {
                current = current->left.get();
            } else if (key > current->key) {
                current = current->right.get();
            } else {
                return current;
            }
        }
        return nullptr;
    }

    void rangedSearch(const unique_ptr<Node>& node, const Key& minKey, const Key& maxKey, vector<Value>& result) const {
        if (!node) return;

        if (node->key > minKey) {
            rangedSearch(node->left, minKey, maxKey, result);
        }

        if (node->key >= minKey && node->key <= maxKey) {
            for (const auto& value : node->values)
                result.push_back(value);
        }

        if (node->key < maxKey) {
            rangedSearch(node->right, minKey, maxKey, result);
        }
    }

    void preOrderTraversal(const unique_ptr<Node>& node, vector<pair<Key, list<Value>>>& result) const {
        if (!node) return;
        result.push_back({node->key, node->values});
        preOrderTraversal(node->left, result);
        preOrderTraversal(node->right, result);
    }

    void inOrderTraversal(const unique_ptr<Node>& node, vector<pair<Key, list<Value>>>& result) const {
        if (!node) return;
        inOrderTraversal(node->left, result);
        result.push_back({node->key, node->values});
        inOrderTraversal(node->right, result);
    }

    void postOrderTraversal(const unique_ptr<Node>& node, vector<pair<Key, list<Value>>>& result) const {
        if (!node) return;
        postOrderTraversal(node->left, result);
        postOrderTraversal(node->right, result);
        result.push_back({node->key, node->values});
    }

public:
    AVLTree() : root(nullptr) {}
    explicit AVLTree(const vector<pair<Key, Value>>& dataBase) {
        for (const auto& pair : dataBase) {
            insert(pair.first, pair.second);
        }
    }
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    ~AVLTree() = default;

    void insert(const Key& key, const Value& value) {
        root = insert(move(root), key, value);
    }

    void removeAll(const Key& key) {
        root = removeAll(move(root), key);
    }

    void removeOne(const Key& key, const Value& value) {
        root = removeOne(move(root), key, value);
    }

    const list<Value>* find(const Key& key) const {
        Node* node = findNode(key);
        if (node == nullptr) return nullptr;
        return &(node->values);
    }

    const list<Value>* operator[](const Key& key) const {
        Node* node = findNode(key);
        if (node == nullptr) return nullptr;
        return &(node->values);
    }

    vector<Value> rangedSearch(const Key& minKey, const Key& maxKey) const {
        vector<Value> result;
        if (minKey > maxKey) return result;
        if (minKey == maxKey) {
            auto found = find(minKey);
            if (!found) return result;
            for (const auto& value : *found) {
                result.push_back(value);
            }
            return result;
        }
        rangedSearch(root, minKey, maxKey, result);
        return result;
    }

    // Прямой обход (pre-order): корень -> левое поддерево -> правое поддерево
    vector<pair<Key, list<Value>>> preOrder() const {
        vector<pair<Key, list<Value>>> result;
        preOrderTraversal(root, result);
        return result;
    }

    // Симметричный обход (in-order): левое поддерево -> корень -> правое поддерево
    vector<pair<Key, list<Value>>> inOrder() const {
        vector<pair<Key, list<Value>>> result;
        inOrderTraversal(root, result);
        return result;
    }

    // Обратный обход (post-order): левое поддерево -> правое поддерево -> корень
    vector<pair<Key, list<Value>>> postOrder() const {
        vector<pair<Key, list<Value>>> result;
        postOrderTraversal(root, result);
        return result;
    }

    [[nodiscard]] size_t getTreeSize() const { return treeSize; }
    [[nodiscard]] size_t getValuesCount() const { return valuesCount; }
    [[nodiscard]] bool isEmpty() const { return treeSize == 0; }
    [[nodiscard]] size_t getListSizeByKey(const Key& key) const {
        Node* node = findNode(key);
        if (node == nullptr) return 0;
        return node->listSize;
    }
    void clear() { root.reset(); treeSize = 0; valuesCount = 0; }

};

#endif
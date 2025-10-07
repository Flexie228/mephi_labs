#ifndef BST_HPP
#define BST_HPP

#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include <queue>
#include "Errors.hpp"
#include "Complex.hpp"

template <typename T>
struct Node {
    T data;
    Node* left;
    Node* right;
};

template <typename T>
class BST {
    private:
        Node<T>* root;
        size_t nodeCount;

        void Clear(Node<T>* node)
        {
            if(node) {
                Clear(node->left);
                Clear(node->right);
                delete node;
            }
        }

        Node<T>* Insert(Node<T>* node, const T& value)
        {
            if(!node) {
                Node<T>* newNode = new Node<T>;
                newNode->data = value;
                newNode->left = nullptr;
                newNode->right = nullptr;
                nodeCount++;
                return newNode;
            }
            if(value < node->data) {
                node->left = Insert(node->left, value);
            } else if(value > node->data) {
                node->right = Insert(node->right, value);
            }
            return node;
        }

        Node<T>* FindMin(Node<T>* node)
        {
            while (node && node->left) {
                node = node->left;
            }
            return node;
        }
    
        Node<T>* Remove(Node<T>* node, const T& value)
        {
            if (!node) return nullptr;
    
            if (value < node->data) {
                node->left = Remove(node->left, value);
            } else if (value > node->data) {
                node->right = Remove(node->right, value);
            } else {
                if (!node->left) {
                    Node<T>* temp = node->right;
                    delete node;
                    nodeCount--;
                    return temp;
                } else if (!node->right) {
                    Node<T>* temp = node->left;
                    delete node;
                    nodeCount--;
                    return temp;
                }

                Node<T>* temp = FindMin(node->right);
                node->data = temp->data;
                node->right = Remove(node->right, temp->data);
            }
            return node;
        }

        bool IsContains(Node<T>* node, const T& value) const
        {
            if (!node) return false;
            if (value == node->data) return true;
            return (value < node->data) ? IsContains(node->left, value) : IsContains(node->right, value);
        }

        void Inorder(Node<T>* node, std::vector<T>& result) const
        {
            if (!node) return;
            Inorder(node->left, result);
            result.push_back(node->data);
            Inorder(node->right, result);
        }
    
        void Preorder(Node<T>* node, std::vector<T>& result) const
        {
            if (!node) return;
            result.push_back(node->data);
            Preorder(node->left, result);
            Preorder(node->right, result);
        }
    
        void Postorder(Node<T>* node, std::vector<T>& result) const
        {
            if (!node) return;
            Postorder(node->left, result);
            Postorder(node->right, result);
            result.push_back(node->data);
        }

        Node<T>* FindNode(Node<T>* node, const T& value) const
        {
            if (!node) return nullptr;
            if (value == node->data) return node;
            if (value < node->data) return FindNode(node->left, value);
            return FindNode(node->right, value);
        }

        bool IsPathValid(const std::string& path) const
        {
            for (char c : path) {
                if (c != 'l' && c != 'r' && c != 'L' && c != 'R') {
                    return false;
                }
            }
            return true;
        }

        void CopySubtree(Node<T>* source, Node<T>*& destination) const
        {
            if (!source) return;
            destination = new Node<T>{source->data, nullptr, nullptr};
            CopySubtree(source->left, destination->left);
            CopySubtree(source->right, destination->right);
        }

        size_t CountNodes(Node<T>* node) const {
            if (!node) return 0;
            return 1 + CountNodes(node->left) + CountNodes(node->right);
        }

        Node<T>* BuildFromInorder(const std::vector<T>& arr, int start, int end)
        {
            if (start > end) return nullptr;
            
            int mid = start + (end - start) / 2;
            Node<T>* node = new Node<T>;
            node->data = arr[mid];
            node->left = BuildFromInorder(arr, start, mid - 1);
            node->right = BuildFromInorder(arr, mid + 1, end);
            
            return node;
        }
    
        Node<T>* BuildFromPreorder(const std::vector<T>& arr, int& index, T min, T max)
        {
            if (index >= arr.size()) return nullptr;
            
            T val = arr[index];
            if (val < min || val > max) return nullptr;
            
            Node<T>* node = new Node<T>;
            node->data = val;
            index++;
            
            node->left = BuildFromPreorder(arr, index, min, val);
            node->right = BuildFromPreorder(arr, index, val, max);
            
            return node;
        }
    
        Node<T>* BuildFromPostorder(const std::vector<T>& arr, int& index, T min, T max)
        {
            if (index < 0) return nullptr;
            
            T val = arr[index];
            if (val < min || val > max) return nullptr;
            
            Node<T>* node = new Node<T>;
            node->data = val;
            index--;
            
            node->right = BuildFromPostorder(arr, index, val, max);
            node->left = BuildFromPostorder(arr, index, min, val);
            
            return node;
        }

        Node<T>* BuildFromInorderPreorder(const std::vector<T>& inorder, 
                                        const std::vector<T>& preorder,
                                        int inStart, int inEnd,
                                        int& preIndex)
        {
            if (inStart > inEnd) return nullptr;

            Node<T>* node = new Node<T>;
            node->data = preorder[preIndex++];

            if (inStart == inEnd) {
                node->left = node->right = nullptr;
                return node;
            }

            int inIndex = std::find(inorder.begin() + inStart, 
                                   inorder.begin() + inEnd + 1, 
                                   node->data) - inorder.begin();

            node->left = BuildFromInorderPreorder(inorder, preorder, 
                                                inStart, inIndex - 1, 
                                                preIndex);
            node->right = BuildFromInorderPreorder(inorder, preorder, 
                                                 inIndex + 1, inEnd, 
                                                 preIndex);

            return node;
        }

        void PrintTree(Node<T>* node, const std::string& prefix = "", bool isLeft = true, bool isRoot = true) const {
            if (!node) return;
        
            std::string newPrefix = prefix + (isLeft && !isRoot ? "│   " : "    ");
            PrintTree(node->right, newPrefix, false, false);
        
            std::cout << prefix;
            if (isRoot) {
                std::cout << "└── ";
            } else {
                std::cout << (isLeft ? "└── " : "┌── ");
            }
            std::cout << node->data << std::endl;
        
            newPrefix = prefix + (isLeft || isRoot ? "    " : "│   ");
            PrintTree(node->left, newPrefix, true, false);
        }

    public:
        BST() : root(nullptr), nodeCount(0) {}

        BST(const std::vector<T>& elements, size_t Type) : root(nullptr), nodeCount(elements.size())
        {
            if (elements.empty()) return;
            
            switch(Type) {
                case 1: {
                    root = BuildFromInorder(elements, 0, elements.size() - 1);
                    break; }
                case 2: {
                    int index = 0;
                    root = BuildFromPreorder(elements, index, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
                    break; }
                case 3: {
                    int index = elements.size() - 1;
                    root = BuildFromPostorder(elements, index, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
                    break; }
                case 4: {
                    if (elements.size() % 2 != 0) {
                        throwError(INVALID_ARGUMENT);
                    }
                    size_t half = elements.size() / 2;
                    std::vector<T> inorder(elements.begin(), elements.begin() + half);
                    std::vector<T> preorder(elements.begin() + half, elements.end());
                    int preIndex = 0;
                    root = BuildFromInorderPreorder(inorder, preorder, 0, inorder.size() - 1, preIndex);
                    nodeCount = inorder.size();
                    break; }
                default:
                    throwError(INVALID_ARGUMENT);
            }
        }

        ~BST() { Clear(root); }

        void Insert(const T& val) { root = Insert(root, val); }
        void Remove(const T& val) { root = Remove(root, val); }
        bool IsContains(const T& val) const { return IsContains(root, val); }

        std::vector<T> GetInorder() const {
            std::vector<T> result;
            Inorder(root, result);
            return result;
        }

        std::vector<T> GetPreorder() const {
            std::vector<T> result;
            Preorder(root, result);
            return result;
        }

        std::vector<T> GetPostorder() const {
            std::vector<T> result;
            Postorder(root, result);
            return result;
        }

        Node<T>* FindByPath(const std::string& path) {
            if (path.empty()) {
                if (!root) throwError(INVALID_ARGUMENT);
                return root;
            }

            if (!IsPathValid(path)) throwError(INVALID_ARGUMENT);
    
            Node<T>* current = root;
            if (!current) throwError(INVALID_ARGUMENT);

            std::string pathCopied = path;

            while (!pathCopied.empty()) {
                char direction = pathCopied[0];
                pathCopied.erase(0, 1);
                if (direction == 'l' || direction == 'L') {
                    current = current->left;
                } else if (direction == 'r' || direction == 'R') {
                    current = current->right;
                }
                if (!current) throwError(INVALID_ARGUMENT);
            }
            return current;
        }

        BST<T> GetSubtree(const T& value) const
        {
            BST<T> subtree;
            Node<T>* startNode = FindNode(root, value);
            if (startNode) {
                CopySubtree(startNode, subtree.root);
                subtree.nodeCount = CountNodes(subtree.root);
            }
            return subtree;
        }

        size_t size() const {return nodeCount;}

        void printInorder() const
        {
            std::vector<T> result;
            Inorder(root, result);
            for (const T& val : result) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }

        void printPreorder() const
        {
            std::vector<T> result;
            Preorder(root, result);
            for (const T& val : result) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    
        void printPostorder() const
        {
            std::vector<T> result;
            Postorder(root, result);
            for (const T& val : result) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }

        void printTree() const {
            PrintTree(root);
        }

        BST(const BST& other) : root(nullptr), nodeCount(0) {
            CopySubtree(other.root, root);
            nodeCount = other.nodeCount;
        }
        
        BST& operator=(const BST& other) {
            if (this != &other) {
                Clear(root);
                CopySubtree(other.root, root);
                nodeCount = other.nodeCount;
            }
            return *this;
        }
};
#endif
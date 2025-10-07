#ifndef LINKED_LIST
#define LINKED_LIST

#include "Errors.hpp"

template <class T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    size_t nodeCount;

public:

    LinkedList() : head(nullptr), nodeCount(0) {}

    LinkedList(const T* array, size_t count) : head(nullptr), nodeCount(0) {  
        for (size_t i = 0; i < count; i++) {
            Append(array[i]);
        }
    }

    LinkedList(const LinkedList<T>& otherList) : head(nullptr), nodeCount(0) {
        Node* current = otherList.head;
        while (current != nullptr) {
            Append(current->data);
            current = current->next;
        }
    }

    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    const T GetFirst() const {
        if (head == nullptr) THROW_ERROR(SEQUENCE_EMPTY);
        return head->data;
    } 

    const T GetLast() const {
        if (head == nullptr) THROW_ERROR(SEQUENCE_EMPTY);
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        return current->data;
    } 

    const T Get(size_t index) const {
        if (index >= nodeCount) THROW_ERROR(INDEX_OUT_OF_RANGE);
        
        Node* current = head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }
    
    size_t GetLength() const {
        return nodeCount;
    }

    void Append(T item) {
        Node* newNode = new Node(item);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while(current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++nodeCount;
    }

    void Prepend(T item) {
        Node* newNode = new Node(item);
        newNode->next = head;
        head = newNode;
        ++nodeCount;
    }

    void InsertAt(T item, size_t index) {
        if (index > nodeCount) THROW_ERROR(INVALID_ARGUMENT);

        if (index == 0) {
            Prepend(item);
        } else {
            Node* current = head;
            for (size_t i = 0; i < index - 1; i++) {
                current = current->next;
            }
            Node* newNode = new Node(item);
            newNode->next = current->next;
            current->next = newNode;
            ++nodeCount;
        }
    }

    void DeleteByIndex(size_t index) {
        if (index >= nodeCount) THROW_ERROR(INDEX_OUT_OF_RANGE);
        
        if (index == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
        } else {
            Node* current = head;
            for (size_t i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            Node* temp = current->next;
            current->next = temp->next;
            delete temp;
        }
        nodeCount--;
    }

    LinkedList<T>* GetSubList(size_t start, size_t end) {
        if (start > end || end >= nodeCount) THROW_ERROR(INVALID_ARGUMENT);

        LinkedList<T>* subList = new LinkedList<T>();

        Node* current = head;
        for (size_t i = 0; i < start; i++) {
            current = current->next;
        }

        for (size_t i = start; i <= end; i++) {
            subList->Append(current->data);
            current = current->next;
        }
        return subList;
    }

    LinkedList<T>* Concat(LinkedList<T>* otherList) {
        LinkedList<T>* result = new LinkedList<T>(*this);

        if (otherList->head == nullptr) return result;

        Node* last = result->head;
        if (last == nullptr) {
            result->head = otherList->head;
        } else {
            while (last->next != nullptr) {
                last = last->next;
            }
            last->next = otherList->head;
        }
        
        result->nodeCount += otherList->nodeCount;
        return result;
    }
};   
#endif
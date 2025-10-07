#ifndef LINKED_LIST
#define LINKED_LIST

#include <stdexcept>
#include <algorithm>
#include "Errors.hpp"



template <typename T>
class LinkedList {
    private:
        struct Node {
            T data;
            Node* next;
        };
        Node* head;
        Node* tail;
        size_t size;

    public:
        // Конструкторы и Деструктор

        LinkedList() : head(nullptr), tail(nullptr), size(0) {}

        LinkedList(const T* array, size_t count) : head(nullptr), tail(nullptr), size(0)
        {
            for (size_t i = 0; i < count; i++) {
                Append(array[i]);
            }
        }

        LinkedList(const LinkedList<T>& otherList) : head(nullptr), tail(nullptr), size(0)
        {
            Node* current = otherList.head;
            while (current != nullptr) {
                Append(current->data);
                current = current->next;
            }
        }

        ~LinkedList()
        {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }

        // Операторы


        // Методы
        const T GetFirst() const
        {
            if (head == nullptr) throwError(SEQUENCE_EMPTY);
            return head->data;
        } 

        const T Get(size_t index) const
        {
            if (index >= size) throwError(INDEX_OUT_OF_RANGE);
            if (index + 1 == size) return tail->data;

            Node* current = head;
            for (size_t i = 0; i < index; i++) {
                current = current->next;
            }
            return current->data;
        }

        const T GetLast() const
        {
            if (size == 0) throwError(SEQUENCE_EMPTY);
            return Get(size - 1);
        } 

        size_t GetSize() const { return size; }

        LinkedList& Append(const T& value)
        {
            Node* newNode = new Node;
            newNode->data = value;
            newNode->next = nullptr;

            if (head == nullptr) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = tail->next;
            }
            ++size;
            return *this;
        }

        LinkedList& Prepend(const T& value)
        {
            Node* newNode = new Node;
            newNode->data = value;
            newNode->next = nullptr;

            newNode->next = head;
            if (head == nullptr) tail = newNode;
            head = newNode;
            ++size;
            return *this;
        }

        void InsertAt(const T& value, size_t index) {
            if (index > size) throwError(INVALID_ARGUMENT);
    
            if (index == 0) {
                Prepend(value);
            } else if (index == size) {
                Append(value);
            } else {
                Node* current = head;
                for (size_t i = 0; i < index - 1; i++) {
                    current = current->next;
                }
                Node* newNode = new Node;
                newNode->data = value;
                newNode->next = current->next;
                current->next = newNode;
                ++size;
            }
        }

        void Remove(size_t index)
        {
            if (index >= size) throwError(INDEX_OUT_OF_RANGE);
        
            if (index == 0) {
                Node* temp = head;
                head = head->next;
                if (size == 1) tail = nullptr;
                delete temp;
            } else {
                Node* current = head;
                for (size_t i = 0; i < index - 1; ++i) {
                    current = current->next;
                }
                Node* temp = current->next;
                current->next = temp->next;
                if (tail == temp) tail = current;
                delete temp;
            }
            --size;
        }
};
#endif
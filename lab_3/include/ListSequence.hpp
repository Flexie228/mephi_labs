#ifndef LIST_SEQUENCE
#define LIST_SEQUENCE

#include "Sequence.hpp"
#include "LinkedList.hpp"

template <typename T>
class ListSequence : public Sequence<T> {
    private:
        LinkedList<T>* list;

    public:
        // Конструкторы и Деструктор

        ListSequence() : list(new LinkedList<T>()) {}
        ListSequence(const LinkedList<T>& list) : list(new LinkedList<T>(list)) {}
        ListSequence(const T* items, size_t elemsCount) : list(new LinkedList<T>(items, elemsCount)) {}
        ListSequence(const ListSequence<T>& other) : list(new LinkedList<T>(*other.list)) {}
        ListSequence(ListSequence<T>&& other) : list(other.list) { other.list = nullptr; }
        ~ListSequence() { delete list; }

        // Операторы

        bool operator == (const Sequence<T>& other) const override
        {
            if (GetSize() != other.GetSize()) return false;
            for (size_t i = 0; i < GetSize(); i++) {
                if (Get(i) != other.Get(i)) return false;
            }
            return true;
        }

        ListSequence<T>& operator= (const Sequence<T>& other)
        {
            if (this != &other) {
                delete list;
                list = new LinkedList<T>(*other.list);
            }
            return *this;
        }

        ListSequence<T>& operator=(ListSequence<T>&& other) {
            if (this != &other) {
                delete list;
                list = other.list;
                other.list = nullptr;
            }
            return *this;
        }

        // Методы

        T GetFirst() const override { return list->GetFirst(); }
        T GetLast() const override { return list->GetLast(); }
        T Get(size_t index) const override { return list->Get(index); }
        size_t GetSize() const override { return list->GetSize(); }

        Sequence<T>* Append(const T& value) override
        {
            list->Append(value);
            return this;
        }
    
        Sequence<T>* Prepend(const T& value) override
        {
            list->Prepend(value);
            return this;
        }
    
        Sequence<T>* InsertAt(const T& value, size_t index) override
        {
            list->InsertAt(value, index);
            return this;
        }
    
        Sequence<T>* RemoveAt(size_t index) override
        {
            list->Remove(index);
            return this;
        }

        Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override 
        {
            if (startIndex > endIndex || endIndex >= list->GetSize()) throwError(INVALID_ARGUMENT);

            ListSequence<T>* subSequence = new ListSequence<T>();
            for (size_t i = startIndex; i <= endIndex; i++) {
                subSequence->Append(list->Get(i));
            }
            return subSequence;
        
        }

        Sequence<T>* Concat(const Sequence<T>& other) const override
        {
            if (other.GetSize() == 0) throwError(INVALID_ARGUMENT);
        
            ListSequence<T>* result = new ListSequence<T>(*this);
            for (size_t i = 0; i < other.GetSize(); i++) {
                result->Append(other.Get(i));
            }
            return result;
        }
};
#endif
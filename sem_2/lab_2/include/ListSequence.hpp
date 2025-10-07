#ifndef LIST_SEQUENCE
#define LIST_SEQUENCE

#include "Sequence.hpp"
#include "LinkedList.hpp"

template <typename T> class MutableListSequence;
template <typename T> class ImmutableListSequence;


template <typename T>
class ListSequence : public Sequence<T> {
    protected:
        LinkedList<T>* list;
    
    public:

        // КОНСТРУКТОРЫ И ДЕСТРУКТОР - ОБЩИЕ

        ListSequence() : list(new LinkedList<T>()) {}
        ListSequence(const LinkedList<T>& list) : list(new LinkedList<T>(list)) {}
        ListSequence(const T* items, size_t count) : list(new LinkedList<T>(items, count)) {}
        ListSequence(const ListSequence<T>& other) : list(new LinkedList<T>(*other.list)) {}
        ListSequence(ListSequence<T>&& other) : list(other.list) { other.list = nullptr; }
        ~ListSequence() { delete list; }


        // Методы доступа - ОБЩИЕ

        T GetFirst() const override { return list->GetFirst(); }
        T GetLast() const override { return list->GetLast(); }
        T Get(size_t index) const override { return list->Get(index); }
        size_t GetLength() const override { return list->GetLength(); }


        // МЕТОДЫ РАБОТЫ - РАЗЛИЧНЫЕ

        virtual Sequence<T>* Append(const T& item) = 0;
        virtual Sequence<T>* Prepend(const T& item) = 0;
        virtual Sequence<T>* InsertAt(const T& item, size_t index) = 0;
        virtual Sequence<T>* DeleteByIndex(size_t index) = 0;

        // ОПЕРАТОРЫ

        bool operator == (const Sequence<T>* other) const override {
            if (this->GetLength() != other->GetLength()) return false;
            size_t nodeCountCommon = this->GetLength();
            for (size_t i = 0; i < nodeCountCommon; i++) {
                if (this->Get(i) != other->Get(i)) return false;
            }
            return true;
        }

        // МЕТОДЫ СОЗДАНИЯ НОВЫХ ПОСЛЕДОВАТЕЛЬНОСТЕЙ - ОБЩИЕ

        Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
            LinkedList<T>* subList = list->GetSubList(startIndex, endIndex);
            Sequence<T>* result = new MutableListSequence<T>(*subList); // Всегда создаем mutable версию
            delete subList;
            return result;
        }

        Sequence<T>* Concat(Sequence<T>* other) const override {
            if (!other) THROW_ERROR(INVALID_ARGUMENT);
            
            LinkedList<T> tempList(*this->list);
            
            for (size_t i = 0; i < other->GetLength(); i++) {
                tempList.Append(other->Get(i));
            }
            
            return new MutableListSequence<T>(tempList);
        }

        ListSequence<T>& operator=(const ListSequence<T>& other) {
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

};

template <typename T>
class MutableListSequence : public ListSequence<T> {
public:
    MutableListSequence() : ListSequence<T>() {}
    MutableListSequence(const LinkedList<T>& list) : ListSequence<T>(list) {}
    MutableListSequence(const T* items, size_t count) : ListSequence<T>(items, count) {}
    MutableListSequence(const ListSequence<T>& other) : ListSequence<T>(other) {}
    
    Sequence<T>* Append(const T& item) override {
        this->list->Append(item);
        return this;
    }

    Sequence<T>* Prepend(const T& item) override {
        this->list->Prepend(item);
        return this;
    }

    Sequence<T>* InsertAt(const T& item, size_t index) override {
        this->list->InsertAt(item, index);
        return this;
    }

    Sequence<T>* DeleteByIndex(size_t index) override {
        this->list->DeleteByIndex(index);
        return this;
    }
};

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(const LinkedList<T>& list) : ListSequence<T>(list) {}
    ImmutableListSequence(const T* items, size_t count) : ListSequence<T>(items, count) {}
    ImmutableListSequence(const ListSequence<T>& other) : ListSequence<T>(other) {}
    
    Sequence<T>* Append(const T& item) override {
        ImmutableListSequence<T>* newSequence = new ImmutableListSequence<T>(*this);
        newSequence->list->Append(item);
        return newSequence;
    }

    Sequence<T>* Prepend(const T& item) override {
        ImmutableListSequence<T>* newSequence = new ImmutableListSequence<T>(*this);
        newSequence->list->Prepend(item);
        return newSequence;
    }

    Sequence<T>* InsertAt(const T& item, size_t index) override {
        ImmutableListSequence<T>* newSequence = new ImmutableListSequence<T>(*this);
        newSequence->list->InsertAt(item, index);
        return newSequence;
    }

    Sequence<T>* DeleteByIndex(size_t index) override {
        ImmutableListSequence<T>* newSequence = new ImmutableListSequence<T>(*this);
        newSequence->list->DeleteByIndex(index);
        return newSequence;
    }
};
#endif
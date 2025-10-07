#ifndef LIST_DEQUE
#define LIST_DEQUE

#include "Deque.hpp"
#include "ListSequence.hpp"
#include "Errors.hpp"

template <typename T>
class ListDeque : public Deque<T> {
    private:
        ListSequence<T> ListSeq;
        
    public:
        // Конструкторы и Деструктор
        ListDeque() = default;
        ListDeque(const T* items, size_t count) : ListSeq(items, count) {}
        ListDeque(const ListDeque& other) : ListSeq(other.ListSeq) {}
        ListDeque(ListDeque&& other) : ListSeq(std::move(other.ListSeq)) {}
        ~ListDeque() = default;

        // Операторы
        ListDeque& operator= (const ListDeque& other)
        {
            if (this != &other) {
                ListSeq = other.ListSeq;
            }
            return *this;
        }
        
        ListDeque& operator=(ListDeque&& other) 
        {
            if (this != &other) {
                ListSeq = std::move(other.ListSeq);
            }
            return *this;
        }

        // Методы
        void PushFront(const T& value) override { ListSeq.Prepend(value); }
        void PushBack(const T& value) override { ListSeq.Append(value); }
        
        T PopFront() override
        {
            if (IsEmpty()) throwError(DEQUE_EMPTY);
            T item = ListSeq.GetFirst();
            ListSeq.RemoveAt(0);
            return item;
        }
        
        T PopBack() override
        {
            if (IsEmpty()) throwError(DEQUE_EMPTY);
            T item = ListSeq.GetLast();
            ListSeq.RemoveAt(ListSeq.GetSize() - 1);
            return item;
        }
        
        T GetFront() const override
        { 
            if (IsEmpty()) throwError(DEQUE_EMPTY);
            return ListSeq.GetFirst();
        }

        T GetBack() const override 
        { 
            if (IsEmpty()) throwError(DEQUE_EMPTY);
            return ListSeq.GetLast();
        }
        size_t GetSize() const override { return ListSeq.GetSize();}
        bool IsEmpty() const override { return ListSeq.GetSize() == 0; }

        T Get(size_t index) const override
        {
            return ListSeq.Get(index);
        }

        void Print() const override
        {
            std::cout << "Содержимое ListDeque: ";

            size_t size = GetSize();
            
            for (size_t i = 0; i < size; ++i) {
                std::cout << Get(i) << " ";
            }
            
            std::cout << std::endl;
        }
};
#endif
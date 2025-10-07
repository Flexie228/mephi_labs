#ifndef ARRAY_DEQUE
#define ARRAY_DEQUE

#include <vector>
#include "Deque.hpp"
#include "ArraySequence.hpp"
#include "Errors.hpp"

template <typename T>
class ArrayDeque : public Deque<T> {
private:
    ArraySequence<T> frontSeq; // Элементы в обратном порядке (растет налево)
    ArraySequence<T> backSeq; // Элементы в прямом порядке (растет направо)

    void Balance() {
        size_t total = frontSeq.GetSize() + backSeq.GetSize();
        if (frontSeq.GetSize() > backSeq.GetSize() * 2) {
            size_t moveCount = frontSeq.GetSize() / 2;
            ArraySequence<T> newFront, newBack;
    
            for (size_t i = 0; i < frontSeq.GetSize() - moveCount; ++i) {
                newFront.Append(frontSeq.Get(i));
            }
            for (size_t i = frontSeq.GetSize() - moveCount; i < frontSeq.GetSize(); ++i) {
                newBack.Append(frontSeq.Get(i));
            }
            for (size_t i = 0; i < backSeq.GetSize(); ++i) {
                newBack.Append(backSeq.Get(i));
            }
    
            frontSeq = std::move(newFront);
            backSeq = std::move(newBack);
        } else if (backSeq.GetSize() > frontSeq.GetSize() * 2) {
            size_t moveCount = backSeq.GetSize() / 2;
            ArraySequence<T> newFront, newBack;
    
            for (size_t i = 0; i < frontSeq.GetSize(); ++i) {
                newFront.Append(frontSeq.Get(i));
            }
            for (size_t i = 0; i < moveCount; ++i) {
                newFront.Append(backSeq.Get(i));
            }
            for (size_t i = moveCount; i < backSeq.GetSize(); ++i) {
                newBack.Append(backSeq.Get(i));
            }
    
            frontSeq = std::move(newFront);
            backSeq = std::move(newBack);
        }
    }

public:
    // Конструкторы и Деструктор
    ArrayDeque() = default;
    ArrayDeque(const T* items, size_t count)
    {
        for (size_t i = 0; i < count; ++i) {
            PushBack(items[i]);
        }
    }
    ArrayDeque(const ArrayDeque& other) : frontSeq(other.frontSeq), backSeq(other.backSeq) {}
    ArrayDeque(ArrayDeque&& other) : frontSeq(std::move(other.frontSeq)), backSeq(std::move(other.backSeq)) {}
    ~ArrayDeque() = default;

    // Операторы
    ArrayDeque& operator= (const ArrayDeque& other)
    {
        if (this != &other) {
            frontSeq = other.frontSeq;
            backSeq = other.backSeq;
        }
        return *this;
    }
    
    ArrayDeque& operator= (ArrayDeque&& other) {
        if (this != &other) {
            frontSeq = std::move(other.frontSeq);
            backSeq = std::move(other.backSeq);
        }
        return *this;
    }

    // Методы
    void PushFront(const T& value) override {
        frontSeq.Append(value);
        Balance();
    }
    
    void PushBack(const T& value) override {
        backSeq.Append(value);
        Balance();
    }
    
    T PopFront() override {
        if (IsEmpty()) throwError(DEQUE_EMPTY);
    
        T item;
        if (frontSeq.GetSize() > 0) {
            item = frontSeq.GetLast();
            frontSeq.RemoveAt(frontSeq.GetSize() - 1);
        } else {
            item = backSeq.GetFirst();
            backSeq.RemoveAt(0);
        }
        Balance();
        return item;
    }
    
    T PopBack() override {
        if (IsEmpty()) throwError(DEQUE_EMPTY);
    
        T item;
        if (backSeq.GetSize() > 0) {
            item = backSeq.GetLast();
            backSeq.RemoveAt(backSeq.GetSize() - 1);
        } else {
            item = frontSeq.GetFirst();
            frontSeq.RemoveAt(0);
        }
        Balance();
        return item;
    }

    T GetFront() const override
    {
        if (IsEmpty()) throwError(DEQUE_EMPTY);
        return frontSeq.GetSize() == 0 ? backSeq.GetFirst() : frontSeq.GetLast();
    }

    T GetBack() const override
    {
        if (IsEmpty()) throwError(DEQUE_EMPTY);
        return backSeq.GetSize() == 0 ? frontSeq.GetFirst() : backSeq.GetLast();
    }

    size_t GetSize() const override { return frontSeq.GetSize() + backSeq.GetSize(); }
    bool IsEmpty() const override { return (frontSeq.GetSize() == 0) && (backSeq.GetSize() == 0); }

    T Get(size_t index) const override
    {
        if (index >= GetSize()) throwError(INDEX_OUT_OF_RANGE);
        if (index < frontSeq.GetSize()) {
            return frontSeq.Get(frontSeq.GetSize() - 1 - index);
        } else {
            return backSeq.Get(index - frontSeq.GetSize());
        }
    }

    void Print() const override
    {
        std::cout << "Содержимое ArrayDeque: ";
        
        // Печатаем элементы из frontSeq в обратном порядке
        for (size_t i = frontSeq.GetSize(); i > 0; --i) {
            std::cout << frontSeq.Get(i - 1) << " ";
        }
        
        // Печатаем элементы из backSeq в прямом порядке
        for (size_t i = 0; i < backSeq.GetSize(); ++i) {
            std::cout << backSeq.Get(i) << " ";
        }
        
        std::cout << std::endl;
    }
};

#endif
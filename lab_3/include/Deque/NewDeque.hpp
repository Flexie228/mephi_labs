#ifndef NEW_DEQUE
#define NEW_DEQUE

#include "DynamicArray.hpp"
#include "Deque.hpp"
#include "Errors.hpp"

template <typename T>
class NewDeque : public Deque<T> {
    private:
        DynamicArray<T> array;
        size_t frontIndex;
        size_t backIndex;
        size_t dequeSize;
        size_t capacity;

        void resize() {
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            DynamicArray<T> newArray(newCapacity);
            
            if (!IsEmpty()) {
                if (frontIndex <= backIndex) {
                    for (size_t i = frontIndex; i <= backIndex; ++i) {
                        newArray[i - frontIndex] = array[i];
                    }
                } else {
                    size_t firstPart = capacity - frontIndex;
                    for (size_t i = 0; i < firstPart; ++i) {
                        newArray[i] = array[frontIndex + i];
                    }
                    for (size_t i = 0; i <= backIndex; ++i) {
                        newArray[firstPart + i] = array[i];
                    }
                }
            }
            
            array = std::move(newArray);
            frontIndex = 0;
            backIndex = dequeSize > 0 ? dequeSize - 1 : 0;
            capacity = newCapacity;
        }

    public:
        // Конструкторы и Деструктор
        
        NewDeque() : frontIndex(0), backIndex(0), dequeSize(0), capacity(0) {}
        NewDeque(const NewDeque& other) : array(other.array), frontIndex(other.frontIndex), backIndex(other.backIndex), dequeSize(other.dequeSize), capacity(other.capacity) {}
        NewDeque(NewDeque&& other) : array(std::move(other.array)), frontIndex(other.frontIndex), backIndex(other.backIndex), dequeSize(other.dequeSize), capacity(other.capacity)
        {
            other.frontIndex = 0;
            other.backIndex = 0;
            other.dequeSize = 0;
            other.capacity = 0;
        }
        NewDeque(const T* items, size_t count) : frontIndex(0), backIndex(count > 0 ? count - 1 : 0), dequeSize(count), capacity(count > 0 ? count : 0)
        {
            if (count > 0) {
                array = DynamicArray<T>(count);
                for (size_t i = 0; i < count; ++i) {
                    array[i] = items[i];
                }
            }
        }
        ~NewDeque() = default;

        // Операторы
        NewDeque& operator= (const NewDeque& other)
        {
            if (this != &other) {
                array = other.array;
                frontIndex = other.frontIndex;
                backIndex = other.backIndex;
                dequeSize = other.dequeSize;
                capacity = other.capacity;
            }
            return *this;
        }

        NewDeque& operator= (NewDeque&& other)
        {
            if (this != &other) {
                array = std::move(other.array);
                frontIndex = other.frontIndex;
                backIndex = other.backIndex;
                dequeSize = other.dequeSize;
                capacity = other.capacity;

                other.frontIndex = 0;
                other.backIndex = 0;
                other.dequeSize = 0;
                other.capacity = 0;
            }
            return *this;
        }


        // Методы
        void PushFront(const T& value) override {
            if (dequeSize == capacity) {
                resize();
            }
            
            if (IsEmpty()) {
                frontIndex = 0;
                backIndex = 0;
            } else {
                frontIndex = (frontIndex == 0) ? capacity - 1 : frontIndex - 1;
            }
            
            array[frontIndex] = value;
            dequeSize++;
        }

        void PushBack(const T& value) override {
            if (dequeSize == capacity) {
                resize();
            }
            
            if (IsEmpty()) {
                frontIndex = backIndex = 0;
            } else {
                backIndex = (backIndex + 1) % capacity;
            }
            
            array[backIndex] = value;
            dequeSize++;
        }

        T PopFront() override {
            if (IsEmpty()) throwError(DEQUE_EMPTY);
            
            T value = array[frontIndex];
            if (dequeSize == 1) {
                frontIndex = backIndex = 0;
            } else {
                frontIndex = (frontIndex + 1) % capacity;
            }
            
            dequeSize--;
            return value;
        }

        T PopBack() override {
            if (IsEmpty()) throwError(DEQUE_EMPTY);
            
            T value = array[backIndex];
            if (dequeSize == 1) {
                frontIndex = backIndex = 0;
            } else {
                backIndex = (backIndex == 0) ? capacity - 1 : backIndex - 1;
            }
            
            dequeSize--;
            return value;
        }

        T GetFront() const override {
            if (IsEmpty()) throwError(DEQUE_EMPTY);
            return array[frontIndex];
        }

        T GetBack() const override {
            if (IsEmpty()) {
                throwError(DEQUE_EMPTY);
            }
            return array[backIndex];
        }

        size_t GetSize() const override {
            return dequeSize;
        }

        bool IsEmpty() const override {
            return dequeSize == 0;
        }

        T Get(size_t index) const override {
            if (index >= dequeSize) throwError(INDEX_OUT_OF_RANGE);

            return array[(frontIndex + index) % capacity];
        }

        void Print() const override {
            std::cout << "[";
            for (size_t i = 0; i < dequeSize; ++i) {
                if (i != 0) std::cout << ", ";
                std::cout << Get(i);
            }
            std::cout << "]" << std::endl;
        }
};

#endif
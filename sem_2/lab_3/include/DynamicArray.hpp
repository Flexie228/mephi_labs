#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include <stdexcept>
#include <algorithm>
#include "Errors.hpp"


template <typename T>
class DynamicArray {
    private:
        T* data;
        size_t size;

    public:
        // Конструкторы и Деструктор

        DynamicArray() : data(nullptr), size(0) {}

        DynamicArray(const T* otherData, size_t otherSize) : data(nullptr), size(otherSize)
        {
            if (otherSize > 0 && otherData == nullptr) throwError(INVALID_ARGUMENT);
            if (size == 0) return;

            data = new T[size];
            std::copy(otherData, otherData + otherSize, data);
        }

        DynamicArray(size_t size) : data(nullptr), size(size)
        {
            if (size > 0) data = new T[size]();
        }

        DynamicArray(const DynamicArray<T>& other) : data(nullptr), size(other.size)
        {
            if (size > 0) {
                data = new T[size];
                std::copy(other.data, other.data + size, data);
            }
        }

        DynamicArray(DynamicArray<T>&& other) : data(other.data), size(other.size)
        {
            other.data = nullptr;
            other.size = 0;
        }

        ~DynamicArray()
        {
            delete[] data;
        }

        // Операторы

        DynamicArray& operator=(DynamicArray<T>&& other)
        {
            if (this != &other) {
                delete[] data;
                data = other.data;
                size = other.size;
                other.data = nullptr;
                other.size = 0;
            }
            return *this;
        }

        DynamicArray& operator = (const DynamicArray& other)
        {
            if (this != &other) {
                delete[] data;
                size = other.size;
                data = size > 0 ? new T[size] : nullptr;
                if(data) std::copy(other.data, other.data + size, data);
            }
            return *this;
        }

        const T& operator[](size_t index) const { return data[index]; }

        T& operator[](size_t index) { return data[index]; }

        // Методы 

        T Get(size_t index) const { return data[index]; }

        size_t GetSize() const { return size; }

        void Set(const T& value, size_t index)
        {
            if (index >= size) throwError(INDEX_OUT_OF_RANGE);
            data[index] = value;
        }
        
        void Resize(size_t newSize) {
            if (newSize == size) return;
            if (newSize > 0) {
                T* newData = new T[newSize];
                std::copy(data, data + std::min(size, newSize), newData);
                delete[] data;

                if (newSize > size) {
                    for (size_t i = size; i < newSize; i++) {
                        newData[i] = T();
                    }
                }

                data = newData;
                size = newSize;
            } else {
                delete[] data;
                data = nullptr;
                size = 0;
            }
        }
};

#endif